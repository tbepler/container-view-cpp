#ifndef INCLUDED_BEPLER_MATH_MATH_H
#define INCLUDED_BEPLER_MATH_MATH_H

#include <cmath>
#include "bepler/strings/strformat.h"
#include <type_traits>
#include <stdexcept>

namespace math{

    template<typename I0 = long, typename I1 = I0>
    typename std::enable_if<
        std::is_integral<I0>::value && std::is_integral<I1>::value,
        I0
        >::type
        pow( I0 base, I1 exp ){

            if( exp < 0 ){
                throw std::invalid_argument( strings::sprintf( "Exponent must be >= 0, but was %ld", exp ) );
            }

            I0 result = 1;
            while( exp ){
                if( exp & 1 ){
                    result *= base;
                }
                exp >>= 1;
                base *= base;
            }
            return result;

        }

    template< typename T > inline T max( T val ){ return val; }
    template< typename T > inline T max( T a, T b ){ return a > b ? a : b; }
    template< typename T, typename ... Ts >
    inline T max( T first, T second, Ts... rest ){
        return max( first, max( second, rest... ) );
    }

    template< typename T > inline T min( T val ){ return val; }
    template< typename T > inline T min( T a, T b ){ return a < b ? a : b; }
    template< typename T, typename ... Ts >
    inline T min( T first, T second, Ts... rest ){
        return min( first, min( second, rest... ) );
    }

    template< typename T > inline bool between( T val, T lower, T upper ){
        return val > lower && val < upper;
    }

    template< typename T > inline T clamp( T val, T lower, T upper ){
        return min( upper, max( lower, val ) );
    }

    template< typename T > inline int sign( T val ){
        return ( T(0) < val ) - ( val < T(0) );
    }

    template< typename T > inline T positive( T val ){
        return val < 0 ? -val : val;
    }

    template< typename T > inline T abs( T val ){
        return positive( val );
    }

    template< typename T > inline T negative( T val ){
        return val > 0 ? -val : val;
    }

    template< typename T > inline T truncate( T val ){
        return val < 0 ? std::ceil( val ) : std::floor( val ); 
    }

    template< typename T > inline T roundTowardsZero( T val ){
        return truncate( val );
    }

    template< typename T > inline T roundAwayFromZero( T val ){
        return val < 0 ? std::floor( val ) : std::ceil( val );
    }

} //namespace math

#endif
