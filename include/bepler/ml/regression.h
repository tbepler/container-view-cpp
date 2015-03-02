#ifndef INCLUDED_BEPLER_ML_REGRESSION_H
#define INCLUDED_BEPLER_ML_REGRESSION_H

#include <utility>
#include "bepler/ml/matrix.h"

namespace ml{

    template< typename Vector, typename N >
    class LinearModel{
        Vector w_;
        N bias_;

        inline bool equals( const LinearModel& o ) const{
            return w_ == o.w_ && bias_ == o.bias_;
        }

        public:
            template< typename A, typename B >
            constexpr LinearModel( A&& w, B&& bias )
                : w_( std::forward<A>( w ) ),
                bias_( std::forward<B>( bias ) ) { }

            inline const Vector& weights() const{ return w_; }
            inline const N& bias() const{ return bias_; }

            template< typename V >
            inline void weights( V&& w ){ w_ = std::forward<V>( w ); }
            template< typename B >
            inline void bias( B&& bias ){ bias_ = std::forward<B>( b ); }

            template< typename Matrix >
            inline auto predict( Matrix&& x ) const{
                return x * w_ + bias_;
            }

            inline bool operator==( const LinearModel& o ) const{
                return this->equals( o );
            }
            inline bool operator!=( const LinearModel& o ) const{
                return !this->equals( o );
            }

    }; //class LinearModel

    template< typename W, typename N >
    constexpr linear_model( W&& w, N&& bias ){
        return LinearModel<decltype(vector( w )),N>( std::forward<W>( w ),
            std::forward<N>( bias ) );
    }

    template< typename X, typename Y, typename N, typename Solver >
    auto ridge( X&& x, Y&& y, N&& lambda, bool centered = false,
        Solver&& solve = Solver() ){
        
        if( !centered ){
            auto model = ridge( center( x ), center( y ), lambda, true,
                std::forward<Solver>( solve ) );
            model.bias( mean( y ) - mean( colwise( x ) ) * model.weights() );
            return model;
        }

        using matrix = decltype( matrix( x ) );
        auto&& a = transpose(x)*x + lambda*eye<matrix>( cols( x ) );
        auto&& w = solve( a, transpose(x)*y );
        return linear_model( w, 0 );

    }

} //namespace ml

#endif
