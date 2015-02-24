#ifndef INCLUDED_BEPLER_FUNCTIONAL_FOLD_H
#define INCLUDED_BEPLER_FUNCTIONAL_FOLD_H

#include "bepler/functional/bind.h"

namespace functional{

    struct Fold{
        
        template< typename F, typename T, typename G >
        inline auto operator()( F&& f, T x, G&& g ) const{
            g( [&]( auto&& v ){
                x = f( x, std::forward< decltype(v) >( v ) );
            } );
            return x;
        }

        template< typename... Params >
        constexpr auto operator()( Params&&... args ){
            return bind( Fold(), std::forward<Params>( args )... );
        }

    };

    static const Fold fold;

} //namespace functional

#endif
