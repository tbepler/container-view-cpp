#ifndef INCLUDED_BEPLER_MATH_MATH_H
#define INCLUDED_BEPLER_MATH_MATH_H

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

} //namespace math

#endif
