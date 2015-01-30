#ifndef INCLUDED_BEPLER_TYPE_TRAITS_EXTENDED_H
#define INCLUDED_BEPLER_TYPE_TRAITS_EXTENDED_H

namespace types{

    template< typename T > T& lvalue();
    template< typename T > T rvalue();

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

} //namespace types


#endif
