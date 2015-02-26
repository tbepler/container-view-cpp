#ifndef INCLUDED_BEPLER_FUNCTIONAL_LIST_H
#define INCLUDED_BEPLER_FUNCTIONAL_LIST_H

#include "bepler/functional/bind.h"


namespace functional{
    
    struct empty{};

    template< typename List, typename K >
    constexpr auto pop( List&& l, K&& k ){
        return l.pop( std::forward<K>( k ) );
    }

    template< typename K >
    constexpr auto pop( empty l, K&& k ){
        //error! cannot pop empty list
    }

    template< typename T, typename U >
    struct List{
        T x;
        U xs;

        template< typename A, typename B >
        constexpr List( A&& a, B&& b ) : x( std::forward<A>( a ) ),
            xs( std::forward<B>( b ) ) { }

        template< typename K >
        constexpr auto pop( K&& k ){
            return k( x, xs );
        }
    };

    template< typename T, typename L >
    constexpr auto prepend( T&& x, L&& xs ){
        return List< decltype(x), decltype( xs ) >( std::forward<T>( x ),
            std::forward<L>( xs ) );
    }

    constexpr auto list( ){
        return empty();
    }

    template< typename T, typename... Ts >
    constexpr auto list( T&& x, Ts&&... xs ){
        return prepend( std::forward<T>( x ),
            list( std::forward<Ts>( xs )... ) );
    }

    struct Map{
    
        template< typename F, typename List >
        constexpr auto operator()( F&& f, List&& l ){
            return pop( std::forward<List>( l ),
                bind( Map(), std::forward<F>( f ) ) );
        }
    
        template< typename F >
        constexpr auto operator()( F&& f, empty l ){
            return l;
        }
    
        template< typename F, typename T, typename Ts >
        constexpr auto operator()( F&& f, T&& x, Ts&& xs ){
            return prepend( f( std::forward<T>( x ) ),
                map( std::forward<F>( f ), std::forward<Ts>( xs ) ) );
        }

    };

    static const Map map_list;
    
    template< typename List, typename K >
    constexpr auto head( std::size_t n, List&& l, K&& k );

    template< typename List >
    struct Head{

        std::size_t n_;
        List xs_;

        template< typename T >
        constexpr Head( std::size_t n, T&& xs ) : n_( n ),
            xs_( std::forward<T>( xs ) ) { }

        template< typename K >
        constexpr auto pop( K&& k ){
            return pop( xs_, bind( *this, std::forward<K>( k ) ) );
        }

        template< typename K, typename T >
        constexpr auto operator()( K&& k, T&& x, empty xs ){
            return k( std::forward<T>( x ), xs );
        }
        
        template< typename K, typename T, typename L >
        constexpr auto operator()( K&& k, T&& x, L&& xs ){
            return head( n_ - 1, std::forward<L>( xs ),
                    bind( std::forward<K>( k ), std::forward<T>( x ) ) );
        }

    };

    template< typename List >
    struct Head<List&&> : public Head<List>{
        using super_t = Head<List>;
        using super_t::super_t;
    };

    template< typename List, typename K >
    constexpr auto head( std::size_t n, List&& l, K&& k ){
        return n > 0 ? k( Head<decltype(l)>( n, std::forward<List>( l ) ) )
            : k( empty() );
    }

    template< typename K >
    inline auto list( K&& k ){
        
    }

} //namespace functional


#endif
