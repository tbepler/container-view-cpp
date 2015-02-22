#ifndef INCLUDED_BEPLER_FUNCTIONAL_GENERATOR_H
#define INCLUDED_BEPLER_FUNCTIONAL_GENERATOR_H

#include <functional>

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




} //namespace functional

#endif
