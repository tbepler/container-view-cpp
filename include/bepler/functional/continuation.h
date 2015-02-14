#ifndef INCLUDED_BEPLER_FUNCTIONAL_CONTINUATION_H
#define INCLUDED_BEPLER_FUNCTIONAL_CONTINUATION_H

#include <type_traits>
#include <iterator>
#include <utility>
#include <deque>

namespace functional{

    template< typename F >
    struct cont_adaptor{
        static constexpr bool is_instance = false;
    };

    template< typename R, typename... As >
    struct cont_adaptor< R( As... ) >{
        template< typename F >
        static auto adapt( F&& f ){
            return [=]( As... args, auto g ){
                return g( f( std::forward<As>( args )... ) );
            };
        }
    };

    template< typename F >
    auto pipe( F&& f ){ return f; }

    template< typename F, typename K >
    auto pipe( F&& f, K&& k ){
        return f( k );
    }

    template< typename F, typename K, typename... Ks >
    auto pipe( F&& f, K&& k, Ks... ks ){
        return f( pipe( k, ks... ) );
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

} //namespace functional

#endif
