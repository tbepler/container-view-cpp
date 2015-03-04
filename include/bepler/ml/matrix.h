#ifndef INCLUDED_BEPLER_ML_MATRIX_H
#define INCLUDED_BEPLER_ML_MATRIX_H

#include <armadillo>
#include <utility>

namespace ml{

    template< typename T >
    using matrix = arma::Mat<T>;

    using matrix_d = matrix<double>;
    using matrix_f = matrix<float>;
    using matrix_l = matrix<long>;
    using matrix_i = matrix<int>;
    using matrix_s = matrix<short>;

    enum Dim{ COL, ROW };

    template< typename M >
    inline auto size( M&& x, Dim d ){
        switch( d ){
            case COL: return x.n_cols;
            case ROW: return x.n_rows;
        }
    }

    template< typename M >
    inline auto size( M&& x ){
        return std::pair< decltype( x.n_rows ),
            decltype( x.n_cols ) >( x.n_rows, x.n_cols );
    }

    template< typename M >
    inline auto numel( M&& x ){ return x.n_elem; }

    template< typename M >
    inline auto rows( M&& x ){ return size( std::forward<M>( x ), ROW ); }

    template< typename M >
    inline auto cols( M&& x ){ return size( std::forward<M>( x ), COL ); }

    template< typename M >
    inline auto repmat( M&& x, std::size_t row_dim, std::size_t col_dim ){
        return arma::repmat( std::forward<M>( x ), row_dim, col_dim );
    }

    template< typename M >
    inline auto eye( std::size_t n, std::size_t m ){
        return arma::eye<M>( n, m );
    }
    
    template< typename M >
    inline auto eye( std::size_t n ){
        return arma::eye<M>( n, n );
    }

    template< typename M >
    inline auto zeros( std::size_t n, std::size_t m ){
        return arma::zeros<M>( n, m );
    }
    
    template< typename M >
    inline auto zeros( std::size_t n ){
        return arma::zeros<M>( n, n );
    }

    template< typename M >
    inline auto ones( std::size_t n, std::size_t m ){
        return arma::ones<M>( n, m );
    }
    
    template< typename M >
    inline auto ones( std::size_t n ){
        return arma::ones<M>( n, n );
    }

    template< typename M >
    inline auto sum( M&& x, Dim d ){
        switch( d ){
            case COL: return arma::sum( std::forward<M>( x ), 0 );
            case ROW: return arma::sum( std::forward<M>( x ), 1 );
        }
    }

    template< typename M >
    inline auto sum( M&& x ){
        return arma::accu( std::forward<M>( x ) );
    }

    template< typename M >
    inline auto mean( M&& x, Dim d ){
        switch( d ){
            case COL: return arma::mean( std::forward<M>( x ), 0 );
            case ROW: return arma::mean( std::forward<M>( x ), 1 );
        }
    }

    template< typename M >
    inline auto mean( M&& x ){
        return arma::mean( arma::mean( x ) );
    }

    template< typename M >
    inline auto center( M&& x ){
        auto colmeans = mean( std::forward<M>( x ), COL );
        auto b = ml::repmat( colmeans, rows( std::forward<M>( x ) ), 1 );
        return x - b;
    }


} //namespace ml

#endif
