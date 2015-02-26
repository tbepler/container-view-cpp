#ifndef INCLUDED_BEPLER_FUNCTIONAL_GENERATOR_H
#define INCLUDED_BEPLER_FUNCTIONAL_GENERATOR_H

#include <functional>
#include "bepler/functional/bind.h"

namespace functional{

    template< typename T >
    using acceptor_f = std::function< void( T ) >;

    template< typename T >
    using generator_f = std::function< void( acceptor_f<T> ) >;

    template< typename G, typename K >
    void enumerate( std::size_t start, G&& g, K&& k ){
        std::size_t i = start;
        g( [&]( auto v ){
            k( i++, v );
        } );
    }

    template< typename G, typename K >
    inline void enumerate( G&& g, K&& k ){
        enumerate( 0, std::forward<G>( g ), std::forward<K>( k ) );
    }

    template< typename G, typename K >
    inline void for_each( G&& g, K&& k ){
        g( std::forward<K>( k ) );
    }

    struct IRange{

        template< typename Iter, typename K >
        inline void operator()( Iter begin, Iter end, K&& k ) const{
            for( auto it = begin ; it != end ; ++it ){
                k( *it );
            }
        }

        template< typename Range, typename K >
        inline void operator()( Range&& r, K&& k ) const{
            for( auto&& val : r ){
                k( std::forward< decltype(val) >( val ) );
            }
        }

        template< typename... Params >
        constexpr auto operator()( Params&&... args ){
            return bind( IRange(), std::forward<Params>( args )... );
        }

    };

    static const IRange irange2;


} //namespace functional

#endif
