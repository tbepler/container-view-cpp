#ifndef INCLUDED_BEPLER_COMPARABLE_H
#define INCLUDED_BEPLER_COMPARABLE_H

template< typename T >
struct Comparable{

    friend bool operator==( const T& a, const T& b ){
        return a.compareTo( b ) == 0;
    }

    friend bool operator!=( const T& a, const T& b ){
        return a.compareTo( b ) != 0;
    }

    friend bool operator<( const T& a, const T& b ){
        return a.compareTo( b ) < 0;
    }

    friend bool operator<=( const T& a, const T& b ){
        return a.compareTo( b ) <= 0;
    }

    friend bool operator>( const T& a, const T& b ){
        return a.compareTo( b ) > 0;
    }

    friend bool operator>=( const T& a, const T& b ){
        return a.compareTo( b ) >= 0;
    }

}; //struct Comparable

template< typename Function, typename T, typename U = T >
struct Comparator{
    
    friend bool operator==( const T& a, const U& b ){
        return Function( a, b ) == 0;
    }

    friend bool operator!=( const T& a, const U& b ){
        return Function( a, b ) != 0;
    }

    friend bool operator<( const T& a, const U& b ){
        return Function( a, b ) < 0;
    }

    friend bool operator<=( const T& a, const U& b ){
        return Function( a, b ) <= 0;
    }

    friend bool operator>( const T& a, const U& b ){
        return Function( a, b ) > 0;
    }

    friend bool operator>=( const T& a, const U& b ){
        return Function( a, b ) >= 0;
    }

}; //struct Comparator

#endif
