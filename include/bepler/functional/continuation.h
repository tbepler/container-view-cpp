#ifndef INCLUDED_BEPLER_FUNCTIONAL_CONTINUATION_H
#define INCLUDED_BEPLER_FUNCTIONAL_CONTINUATION_H

#include <type_traits>
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
    inline auto range( K&& k ){
        return Range<K>( std::forward<K>( k ) );
    }

    auto window( std::size_t n ){

        return [=]( auto&& k ){ 
            //std::deque<auto> w;
            return [=]( const auto& x ) mutable {
                //w.push_back( x );
                //if( w.size() > n ){
                    //w.pop_front();
                    //k( const w );
                //}
            };
        };

    }

} //namespace functional

#endif
