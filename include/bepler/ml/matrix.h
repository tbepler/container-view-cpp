#ifndef INCLUDED_BEPLER_ML_MATRIX_H
#define INCLUDED_BEPLER_ML_MATRIX_H

#include <utility>

namespace ml{

    template< typename T > inline auto matrix( T&& x );
    template< typename T > inline auto vector( T&& x );

    template< typename T > inline auto eye( std::size_t n );
    template< typename T > inline auto zeros( std::size_t n, std::size_t m );
    template< typename T > inline auto ones( std::size_t n, std::size_t m );

    template< typename T > inline auto zeros( std::size_t n ){
        return zeros<T>( n, n );
    }
    template< typename T > inline auto ones( std::size_t n ){
        return ones<T>( n, n );
    }

    template< typename T > inline std::size_t cols( T&& x );
    template< typename T > inline std::size_t rows( T&& x );

    template< typename T > inline auto colwise( T&& x );
    template< typename T > inline auto rowwise( T&& x );

    template< typename T > inline auto transpose( T&& x );

    template< typename T > inline auto mean( T&& x );

    template< typename T >
    inline auto center( T&& x ){
        return x - mean( colwise( x ) );
    }

} //namespace ml

#endif
