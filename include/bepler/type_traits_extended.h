#ifndef INCLUDED_BEPLER_TYPE_TRAITS_EXTENDED_H
#define INCLUDED_BEPLER_TYPE_TRAITS_EXTENDED_H

template< typename T > T& lvalue_of_type();
template< typename T > T rvalue_of_type();

template< typename T >
struct is_incrementable{
    template< typename U > static char test( char(*)[ sizeof(
        lvalue_of_type<U>()++, ++lvalue_of_type<U>() ) ] );
    //template< typename U, typename Post = decltype( lvalue_of_type<U>()++ ), typename Pre = decltype( ++lvalue_of_type<U>() ) > static char test( int );
    template< typename U > static long test(...);

    enum { value = 1 == sizeof test<T>(0) };

};


#endif
