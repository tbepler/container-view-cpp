#ifndef INCLUDED_BEPLER_EQUATABLE_H
#define INCLUDED_BEPLER_EQUATABLE_H

template< typename T >
struct Equatable{

    friend bool operator==( const T& a, const T& b ){
        return a.equals( b );
    }

    friend bool operator!=( const T& a, const T& b ){
        return !a.equals( b );
    }

}; //struct Equatable

template< typename Function, typename T, typename U = T >
struct Equator{

    friend bool operator==( const T& a, const U& b ){
        return Function( a, b );
    }

    friend bool operator!=( const T& a, const U& b ){
        return !Function( a, b );
    }

}; //struct Equator

#endif
