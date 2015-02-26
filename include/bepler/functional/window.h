#ifndef INCLUDED_BEPLER_FUNCTIONAL_WINDOW_H
#define INCLUDED_BEPLER_FUNCTIONAL_WINDOW_H

#include "bepler/functional/bind.h"
#include <vector>

namespace functional{

    template< typename T >
    class Window{
        std::vector<T> buffer_;
        std::size_t i_;

        public:

            constexpr Window( std::size_t n ) : buffer_( n ), i_( 0 ) { }

            template< typename U, typename K >
            inline void operator()( U&& x, K&& k ){
                buffer_[ i_ % buffer_.size() ] = x;
                ++i_;
                if( i_ >= buffer_.size() ){
                    k( *this );
                }
            }

            template< typename K >
            inline void operator()( K&& k ){
                for( std::size_t pos = i_ - buffer_.size() ; pos < i_ ; ++pos ){
                    k( buffer_[ pos % buffer_.size() ] );
                }
            }


    };

    template< typename T >
    struct RollingWindow{

        template< typename G, typename K >
        inline void operator()( std::size_t n, G&& g, K&& k ) const{
            //make Window class to accept elements of g
            Window<T> win( n );
            //call the generator 
            g( [&]( auto&& x ){
                //put x in the window, which will call the acceptor as needed
                win( std::forward<decltype(x)>( x ), std::forward<K>( k ) );
            } );
        }

    };

    //static const RollingWindow rolling_window;

    template< typename T, typename G, typename K >
    inline void rolling_window( std::size_t n, G&& g, K&& k ){
        return RollingWindow<T>()( n, std::forward<G>( g ),
            std::forward<K>( k ) );
    }

    template< typename T, typename... Ts >
    constexpr auto rolling_window( std::size_t n, Ts&&... xs ){
        return bind( RollingWindow<T>(), n, std::forward<Ts>( xs )... );
    }

} //namespace functional

#endif
