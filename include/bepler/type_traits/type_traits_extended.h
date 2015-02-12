#ifndef INCLUDED_BEPLER_TYPE_TRAITS_EXTENDED_H
#define INCLUDED_BEPLER_TYPE_TRAITS_EXTENDED_H

#include <string>
#include <type_traits>
#include <typeinfo>
#include <memory>
#ifndef _MSC_VER
    #include <cxxabi.h>
#endif

namespace type_traits{

    template< typename T > T& lvalue();
    template< typename T > T rvalue();

    template< typename T > struct identity{ typedef T type; };

    // from http://stackoverflow.com/questions/81870/print-variable-type-in-c 
    template< typename T >
    std::string type_name(){
        using base_t = typename std::remove_reference<T>::type;

        std::unique_ptr<char, void(*)(void*)> own(
#ifndef _MSC_VER
            abi::__cxa_demangle( typeid( base_t ).name(), NULL, NULL, NULL ),
#else
            NULL,
#endif
            std::free
        );

        std::string s = own != NULL ? own.get() : typeid( base_t ).name();
        if( std::is_const< base_t >::value ){
            s += " const";
        }
        if( std::is_volatile< base_t >::value ){
            s += " volatile";
        }
        if( std::is_lvalue_reference<T>::value ){
            s += "&";
        }else if( std::is_rvalue_reference<T>::value ){
            s += "&&";
        }
        return s;
    }   

    template< typename K, typename V >
    struct static_case{
        using key = K;
        using type = V;
    };

    template< typename T >
    struct default_case;

    template< typename T, typename... Cases >
    struct static_switch;

    template< typename T, typename Case, typename... Cases >
    struct static_switch< T, Case, Cases... >{
        using type = typename std::conditional<
            std::is_same< T, typename Case::key >::value,
            typename Case::type,
            typename static_switch<T,Cases...>::type
        >::type;
    };

    template< typename T, typename V, typename... Cases >
    struct static_switch< T, default_case<V>, Cases...>{
        using type = typename static_switch< T, Cases..., default_case<V> >::type;
    };

    template< typename T, typename V >
    struct static_switch< T, default_case<V> >{
        using type = V;
    };

    template< typename T >
    struct static_switch< T >{
        using type = void;
    };

    template< typename T >
    struct promote_type{
        using type = T;
    };
    template<> struct promote_type<char>{ using type = int; };
    template<> struct promote_type<unsigned char>{ using type = unsigned int; };
    template<> struct promote_type<int>{ using type = long; };
    template<> struct promote_type<unsigned int>{ using type = unsigned long; };
    template<> struct promote_type<long>{ using type = long long; };
    template<> struct promote_type<unsigned long>{ using type = unsigned long long; };
    template<> struct promote_type<float>{ using type = double; };

    template< typename T >
    using promote = typename promote_type<T>::type;

    template< typename T >
    struct safe_make_signed{
        using type = typename std::conditional<
            !std::is_signed<T>::value && std::is_integral<T>::value,
            std::make_signed< promote<T> >,
            identity< T >
        >::type::type;
    };

    template< typename T, typename U = T >
    using diff_type = decltype( rvalue<T>() - rvalue<U>() );

    template< typename T, typename U = T >
    using safe_diff_t = typename std::conditional<
        std::is_same< T,diff_type<T,U> >::value || std::is_same< U,diff_type<T,U> >::value,
        promote< diff_type<T,U> >,
        diff_type<T,U>
    >::type;

    template< typename T, typename U = T >
    using signed_diff_t = typename std::conditional<
        std::is_integral< diff_type<T,U> >::value,
        std::make_signed< diff_type<T,U> >,
        identity< diff_type<T,U> >
    >::type::type;

    template< typename T, typename U = T >
    using safe_signed_diff_t = typename std::conditional<
        std::is_integral< safe_diff_t<T,U> >::value,
        std::make_signed< safe_diff_t<T,U> >,
        identity< safe_diff_t<T,U> >
    >::type::type;

    template< typename T >
    struct is_incrementable{
        template< typename U > static char test( char(*)[ sizeof(
            lvalue<U>()++, ++lvalue<U>() ) ] );
        //template< typename U, typename Post = decltype( lvalue_of_type<U>()++ ), typename Pre = decltype( ++lvalue_of_type<U>() ) > static char test( int );
        template< typename U > static long test(...);

        enum { value = 1 == sizeof test<T>(0) };

    }; //struct is_incrementable


    template< typename T >
    struct is_decrementable{
        template< typename U > static char test( char(*)[ sizeof(
            lvalue<U>()--, --lvalue<U>() ) ] );
        //template< typename U, typename Post = decltype( lvalue_of_type<U>()++ ), typename Pre = decltype( ++lvalue_of_type<U>() ) > static char test( int );
        template< typename U > static long test(...);

        enum { value = 1 == sizeof test<T>(0) };

    }; //struct is_incrementable

    template< typename A, typename B = A >
    struct is_equals_comparable{
        template< typename U, typename V > static char test( char(*)[ sizeof(
            rvalue<U>() == rvalue<V>(), rvalue<U>() != rvalue<V>() ) ] );
        template< typename U, typename V > static long test(...);

        enum { value = 1 == sizeof test<A,B>(0) };

    }; //struct is_equals_comparable

    template< typename A, typename B = A >
    struct is_inequality_comparable{
        template< typename U, typename V > static char test( char(*)[ sizeof(
            rvalue<U>() < rvalue<V>(), rvalue<U>() <= rvalue<V>(),
            rvalue<U>() > rvalue<V>(), rvalue<U>() >= rvalue<V>() ) ] );
        template< typename U, typename V > static long test(...);

        enum { value = 1 == sizeof test<A,B>(0) };

    }; //struct is_inequality_comparable

    template< typename T >
    struct has_dereference_operator{
        template< typename U > static char test( char(*)[ sizeof(
            *rvalue<U>() ) ] );
        template< typename U > static long test(...);

        enum { value = 1 == sizeof test<T>(0) };

    }; //struct has_dereference_operator

    template< typename T >
    struct has_arrow_operator{
        template< typename U > static char test( char(*)[ sizeof(
            rvalue<U>().operator->() ) ] );
        template< typename U > static long test(...);

        enum { value = 1 == sizeof test<T>(0) };
    
    }; //struct has_arrow_operator

    template< typename T >
    struct has_arrow_operator<T*>{
        enum { value = true };
    }; //struct has_arrow_operator pointer specialization

    template< typename A, typename B = A >
    struct is_addable{
        template< typename U, typename V > static char test( char(*)[ sizeof(
            rvalue<U>() + rvalue<V>() ) ] );
        template< typename U, typename V > static long test(...);
    
        enum { value = 1 == sizeof test<A,B>(0) };

    }; //struct is_addable

    template< typename A, typename B = A >
    struct is_subtractable{
        template< typename U, typename V > static char test( char(*)[ sizeof(
            rvalue<U>() - rvalue<V>() ) ] );
        template< typename U, typename V > static long test(...);

        enum { value = 1 == sizeof test<A,B>(0) };

    }; //struct is_subtractable

    template< typename T, typename N = T >
    struct is_compound_assignable{
        template< typename U, typename V > static char test( char(*)[ sizeof(
            lvalue<U>() += rvalue<V>(), lvalue<U>() -= rvalue<V>() ) ] );
        template< typename U, typename V > static long test(...);

        enum { value = 1 == sizeof test<T,N>(0) };

    }; //struct is_compound_assignable

    template< typename T, typename N >
    struct is_indexable_by{
        template< typename U, typename V > static char test( char(*)[ sizeof(
            rvalue<U>()[ rvalue<V>() ] ) ] );
        template< typename U, typename V > static long test(...);
    
        enum { value = 1 == sizeof test<T,N>(0) };

    }; //struct is_indexable_by

} //namespace type_traits


#endif
