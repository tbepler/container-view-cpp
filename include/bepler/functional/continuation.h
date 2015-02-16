#ifndef INCLUDED_BEPLER_FUNCTIONAL_CONTINUATION_H
#define INCLUDED_BEPLER_FUNCTIONAL_CONTINUATION_H

#include <type_traits>
#include <iterator>
#include <utility>
#include <deque>

namespace functional{

    struct identity_cps{
        template< typename T >
        inline T operator()( T x ) const{ return x; }
        template< typename T, typename K >
        inline auto operator()( T x, K&& k ){ return k( x ); }
    };

    const static identity_cps identity;

    template< unsigned n >
    struct rotate_args{
        template< typename F >
        inline static auto eval( F&& f ){ return f(); }
        
        template< typename F, typename T >
        inline static auto eval( F&& f, T&& arg ){
            return f( std::forward<T>( arg ) );
        }

        template< typename F, typename T, typename... Ts >
        inline static auto eval( F&& f, T&& x, Ts&&... xs ){
            return rotate_args< n - 1 >::eval(
                std::forward<F>( f ),
                std::forward<Ts>( xs )...,
                std::forward<T>( x )
            );
        }
    };

    template<>
    struct rotate_args< 0 >{
        template< typename F, typename... Ts >
        inline static auto eval( F&& f, Ts&&... args ){
            return f( std::forward<Ts>( args )... );
        }
    };

    struct eval_cps{
        template< typename K, typename F, typename... Params >
        inline auto operator()( K&& k, F&& f, Params&&... args ) const{
            return k( f( std::forward<Params>( args )... ) );
        }
    };

    template< typename F >
    struct make_cps{
        make_cps( F&& f ) : f_( std::forward<F>( f ) ) { }
        template< typename T, typename... Args >
        inline auto operator()( T&& first, Args&&... xs ) const{
            return rotate_args< sizeof...( xs ) + 1 >::eval( 
                eval_cps(),
                std::forward<F>( f_ ),
                std::forward<T>( first ),
                std::forward<Args>( xs )...
            );
        }
        private:
            F&& f_;
    };

    template< typename F >
    inline auto as_cps( F&& f ){
        return make_cps<F>( std::forward<F>( f ) );
    }

    template< typename T >
    struct aggregator{
        static inline auto gather( const T& x )
            -> decltype( x.gather() )
        {
            return x.gather();
        }
    };

    template< typename F, typename... Ks >
    struct pipe_cps{
        pipe_cps( F&& f, Ks&&... ks ): f_( std::forward<F>( f ) ), k_( std::forward<Ks>( ks )... ) { }
        template< typename... Params >
        inline auto operator()( Params&&... args )const{
            return f_( std::forward<Params>( args )..., k_ );
        }
        template< typename T = pipe_cps< Ks... > >
        inline auto gather() const
            -> decltype( aggregator<T>::gather( std::declval<T>() ) )
        {
            return aggregator<T>::gather( k_ );
        }
        private:
            F f_;
            pipe_cps< Ks... > k_;
    };

    template< typename F, typename K >
    struct pipe_cps<F,K>{
        pipe_cps( F&& f, K&& k ): f_( std::forward<F>( f ) ), k_( std::forward<K>( k ) ) { }
        template< typename... Params >
        inline auto operator()( Params&&... args )const{
            return f_( std::forward<Params>( args )..., k_ );
        }
        template< typename T = K >
        inline auto gather() const
            -> decltype( aggregator<T>::gather( std::declval<T>() ) )
        {
            return aggregator<T>::gather( k_ );
        }
        private:
            F f_;
            K k_;
    };

    template< typename F >
    struct pipe_cps<F>{
        pipe_cps( F&& f ): f_( std::forward<F>( f ) ) { }
        template< typename... Params >
        inline auto operator()( Params&&... args )const{
            return f_( std::forward<Params>( args )... );
        }
        template< typename T = F >
        inline auto gather() const
            -> decltype( aggregator<T>::gather( this->f_ ) )
        {
            return aggregator<T>::gather( f_ );
        }
        private:
            F f_;
    };

    template< typename F, typename... Ks >
    struct aggregator< pipe_cps<F,Ks...> >{
        static inline auto gather( const pipe_cps<F,Ks...>& p )
            -> decltype( p.gather() )
        {
            return p.gather();
        }
    };

    template< typename F >
    auto pipe( F&& f ){ return f; }

    template< typename F, typename K >
    auto pipe( F&& f, K&& k ){
        //return pipe_cps< F, K >( std::forward<F>( f ), std::forward<K>( k ) );
        return [=]( auto&&... args ){
            return f( std::forward< decltype( args )>( args )..., k );
        };
    }

    template< typename F, typename K, typename... Ks >
    auto pipe( F&& f, K&& k, Ks&&... ks ){
        //return pipe_cps< F, K, Ks... >(
        //    std::forward<F>( f ),
        //    std::forward<K>( k ),
        //    std::forward<Ks>( ks )... 
        //);
        auto pipe_k = pipe( std::forward<K>( k ), std::forward<Ks>( ks )... );
        return [=]( auto&&... args){
            return f( std::forward< decltype( args ) >( args )..., pipe_k );
        };
    }
 
    struct foldl_f{
        
        template< typename F, typename T, typename G >
        auto operator()( F&& f, T cur, G&& g ){
            g( [&]( auto x ){
                cur = f( cur, x );
            } );
            return cur;
        }
    
        template< typename F, typename T, typename G, typename K >
        auto operator()( F&& f, T cur, G&& g, K&& k ){
            g( [&]( auto x ){
                cur = f( cur, x );
            } );
            return k( cur );
        }
    
    };

    template< typename F, typename T, typename G >
    auto foldl( F&& f, T cur, G&& g ){
        g( [&]( auto x ){
            cur = f( cur, x );
        } );
        return cur;
    }

    template< typename F, typename T, typename G, typename K >
    auto foldl( F&& f, T cur, G&& g, K&& k ){
        g( [&]( auto x ){
            cur = f( cur, x );
        } );
        return k( cur );
    }

    template< typename F, typename... Args >
    auto foldl( F&& f, Args&&... xs ){
        return [=]( auto&&... args ){
            return foldl( f, xs..., std::forward<decltype(args)>( args )... );
        };
    }

    template< typename F, typename G, typename K >
    void map( F&& f, G&& g, K&& k ){
        g( [&]( auto x ){
            k( f( x ) );
        } );
    }

    template< typename F, typename G >
    auto map( F&& f, G&& g ){
        return [=]( auto&& k ){
            g( [&]( auto x ){
                k( f( x ) );
            } );
        };
    }

    template< typename F, typename... Args >
    auto curry( F&& f, Args&&... xs ){
        return [=]( auto&&... args ){
            return f( xs..., std::forward< decltype( args ) >( args )... );
        };
    }

    template< typename K >
    class Range{
        K&& k_;
        public:
            Range( K&& k ) : k_( std::forward<K>( k ) ) { }
            template< typename T >
            void operator()( T begin, T end ){
                if( begin < end ){
                    for( auto i = begin ; i < end ; ++i ){
                        k_( i );
                    }
                }else{
                    for( auto i = begin ; i > end ; --i ){
                        k_( i );
                    }
                }
            }
            template< typename T, typename D >
            void operator()( T begin, T end, D step ){
                if( begin < end ){
                    for( auto i = begin ; i < end ; i += step ){
                        k_( i );
                    }
                }else{
                    for( auto i = begin ; i > end ; i += step ){
                        k_( i );
                    }
                }
            }
    };

    struct range_cps{
        template< typename T, typename K>
        void operator()( T begin, T end, K&& k ) const{
            if( begin < end ){
                for( auto i = begin ; i < end ; ++i ){
                    k( i );
                }
            }else{
                for( auto i = begin ; i > end ; --i ){
                    k( i );
                }
            }
        }
        template< typename T, typename D, typename K >
        void operator()( T begin, T end, D step, K&& k ) const{
            if( begin < end ){
                for( auto i = begin ; i < end ; i += step ){
                    k( i );
                }
            }else{
                for( auto i = begin ; i > end ; i += step ){
                    k( i );
                }
            }
        }
        template< typename T, typename... Args >
        auto operator()( T&& x, Args&&... xs ) const{
            return curry( range_cps(), std::forward<T>( x ), std::forward<Args>( xs )... );
        }
    };
    
    struct irange_cps{
        template< typename T, typename K >
        void operator()( T begin, T end, K&& k ) const{
            for( auto i = begin ; i != end ; ++i ){
                k( *i );
            }
        }
        template< typename R, typename K >
        void operator()( R&& r, K&& k ) const{
            (*this)( std::begin( r ), std::end( r ), std::forward<K>( k ) );
        }
        template< typename T, typename... Args >
        auto operator()( T&& x, Args&&... xs ) const{
            return curry( irange_cps(), std::forward<T>( x ), std::forward<Args>( xs )... );
        }
    };

    const static range_cps range;
    const static irange_cps irange;

/*
    template< typename K >
    class IterRange{
        K&& k_;
        public:
            IterRange( K&& k ) : k_( std::forward<K>( k ) ) { }
            template< typename T >
            void operator()( T begin, T end ){
                for( auto i = begin ; i != end ; ++i ){
                    k_( *i );
                }
            }
            template< typename R >
            void operator()( R&& r ){
                this->operator()( std::begin( r ), std::end( r ) );
            }
    };

    template< typename K >
    inline auto range( K&& k ){
       return Range<K>( std::forward<K>( k ) );
    }
    
    template< typename K >
    inline auto irange( K&& k ){
        return IterRange<K>( std::forward<K>( k ) );
    }
*/

    template< typename T, typename G, typename K >
    void window( std::size_t n, G&& g, K&& k ){
        std::deque<T> win;
        g( [&]( const T& x ){
            win.push_back( x );
            while( win.size() > n ){
                win.pop_front();
            }
            if( win.size() == n ){
                k( irange( win ) );
            }
        } );
    }

    template< typename T, typename G >
    auto window( std::size_t n, G&& g ){
        return [=]( auto&& k ){
            window<T>( n, g, std::forward<decltype(k)>( k ) );
        };
    }

    template< typename T >
    auto window( std::size_t n ){
        return [=]( auto&& g, auto&& k ){
            window<T>( n,
                std::forward<decltype(g)>( g ), std::forward<decltype(k)>( k ) );
        };
    }

/*
    struct window_f{
        
        template< typename T, typename G, typename K >
        void operator()( std::size_t n, G&& g, K&& k ) const{
            std::deque<T> win;
            g( [&]( const T& x ){
                win.push_back( x );
                while( win.size() > n ){
                    win.pop_front();
                }
                if( win.size() == n ){
                    k( irange( win ) );
                }
            } );
        }

        template< typename T, typename G >
        auto operator()( std::size_t n, G&& g ) const{
            return [=]( auto&& k ){
                window_f().operator()<T,G,decltype(k)>( n, g, std::forward<decltype(k)>( k ) );
            };
        }

        template< typename T >
        auto operator()( std::size_t n ) const{
            return [=]( auto&& g, auto&& k ){
                window_f().operator()<T,decltype(g),decltype(k)>( n,
                    std::forward<decltype(g)>( g ), std::forward<decltype(k)>( k ) );
            };
        }

    };

    static const window_f window;
*/

/*
    template< typename T >
    auto window( std::size_t n ){

        return [=]( auto&& k ){ 
            std::deque<T> w;
            return [=]( const T& x ) mutable {
                w.push_back( x );
                while( w.size() > n ){
                    w.pop_front();
                }
                if( w.size() == n ){
                    k( w );
                }
            };
        };

    }
    */

} //namespace functional

#endif
