#ifndef INCLUDED_BEPLER_ITERABLE_TRAITS_H
#define INCLUDED_BEPLER_ITERABLE_TRAITS_H

#include "bepler/type_traits/type_traits_extended.h"
#include "bepler/type_traits/iterator_traits.h"
#include <type_traits>
#include <iterator>
#include <utility>

namespace type_traits{

    template< typename Iterable >
        using iterator_type_of =
        typename std::decay< decltype( std::begin(std::declval<Iterable>()) ) >::type;

    template< typename Iterable >
        using const_iterator_type_of =
        typename std::decay< decltype( std::begin(std::declval<const Iterable>()) ) >::type;

    template< typename T >
        struct has_begin{
            template< typename U > static char test( char(*)[ sizeof(
                        std::begin( lvalue<U>() ) ) ] );
            template< typename U > static long test(...);

            enum { value = 1 == sizeof test<T>(0) };
            constexpr operator bool() { return value; }
        };

    template< typename T >
        struct has_end{
            template< typename U > static char test( char(*)[ sizeof(
                        std::end( lvalue<U>() ) ) ] );
            template< typename U > static long test(...);

            enum { value = 1 == sizeof test<T>(0) };
            constexpr operator bool() { return value; }
        };

    template< typename T,
        bool has_begin = has_begin<T>::value,
        bool has_end = has_end<T>::value >
            struct is_iterable : public std::false_type { };

    template< typename T >
        struct is_iterable<T,true,true>{
            enum {
                value =
                    is_iterator< decltype( std::begin( lvalue<T>() ) ) >::value
                    && is_iterator< decltype( std::end( lvalue<T>() ) ) >::value
            };
        };

    template< typename T >
        using is_const_iterable = is_iterable< const T >;

    template< typename T,
        bool is_iterable = type_traits::is_iterable<T>::value,
        bool is_const_iterable = type_traits::is_const_iterable<T>::value >
    struct iterable_traits{ };

    template< typename T >
    struct iterable_traits<T,true,true>{
        using iterator = type_traits::iterator_type_of<T>;
        using const_iterator = type_traits::const_iterator_type_of<T>;
    };

    template< typename T >
    struct iterable_traits<T,false,true>{
        using iterator = type_traits::const_iterator_type_of<T>;
        using const_iterator = type_traits::const_iterator_type_of<T>;
    };

    template< typename T >
    struct iterable_traits<T,true,false>{
        using iterator= type_traits::iterator_type_of<T>;
    };

} //namespace type_traits

#endif
