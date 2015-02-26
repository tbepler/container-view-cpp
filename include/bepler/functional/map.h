#ifndef INCLUDED_BEPLER_FUNCTIONAL_MAP_H
#define INCLUDED_BEPLER_FUNCTIONAL_MAP_H

namespace functional{

    template< typename F, typename G, typename K >
    inline void map( F&& f, G&& g, K&& k ){
        g( [&]( auto&&... x ){
            k( f( std::forward<decltype(x)>( x )... ) );
        } );
    }

    template< typename F, typename G, typename K >
    inline void map_cps( F&& f, G&& g, K&& k ){
        g( [&]( auto&&... x ){
            f( std::forward<decltype(x)>( x )..., std::forward<K>( k ) );
        } );
    }

    template< typename Functions, typename G, typename K >
    inline void map_all( Functions&& fs, G&& g, K&& k ){
        g( [&]( auto&&... x ){
            for( auto&& f : fs ){
                k( f( std::forward<decltype(x)>( x )... ) );
            }
        } );
    }

    template< typename Functions, typename G, typename K >
    inline void map_all_cps( Functions& fs, G&& g, K&& k ){
        g( [&]( auto&&... x ){
            for( auto&& f : fs ){
                f( std::forward<decltype(x)>( x )..., std::forward<K>( k ) );
            }
        } );
    }

} //namespace functional

#endif
