#ifndef INCLUDED_BEPLER_ML_REGRESSION_H
#define INCLUDED_BEPLER_ML_REGRESSION_H

#include <utility>
#include "bepler/ml/matrix.h"

namespace ml{

    template< typename Vector, typename Scalar >
    class LinearModel{
        Vector w_;
        Scalar bias_;

        inline bool equals( const LinearModel& o ) const{
            return w_ == o.w_ && bias_ == o.bias_;
        }

        public:
            template< typename A, typename B >
            constexpr LinearModel( A&& w, B&& bias )
                : w_( std::forward<A>( w ) ),
                bias_( std::forward<B>( bias ) ) { }

            inline const Vector& weights() const{ return w_; }
            inline const Scalar& bias() const{ return bias_; }

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

    /*
    Ridge:

    argmin[ (Xw+w0-y)'(Xw+w0-y) ]
    w,w0

    w = (X'X + nI)\X'y
    */

    template< typename X, typename Mean >
    inline auto center( X&& x, Mean&& mean = Mean() ){
        
    }

    template< typename Vector, typename Scalar, typename Subroutines >
    struct Regression : private Subroutines{

        
        auto ridge( ) const{

        }

    };

    template< typename Vector, typename N >
    struct RidgeModel : public LinearModel<Vector,N>{
        
        template< typename X, typename Y, typename R, typename Solver >
        RidgeModel& train( X&& x, Y&& y, R&& regularizer, bool centered = false,
             Solver&& solve = Solver() ){
            
            if( !centered ){
                this->train( center( x ), center( y ), lambda, true, r );
                this->bias( mean( y ) - this->predict( mean( x, 0 ) ) );
                return *this;
            }
            auto&& a = trans(x)*x + lambda*eye( x );
            this->weights( solve( a, trans(x)*y ) );

        }

    };


} //namespace ml

#endif
