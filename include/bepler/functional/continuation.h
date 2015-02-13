#ifndef INCLUDED_BEPLER_FUNCTIONAL_CONTINUATION_H
#define INCLUDED_BEPLER_FUNCTIONAL_CONTINUATION_H

#include <type_traits>
#include <utility>

namespace functional{

    template< typename F >
    struct continuation{
        
        static constexpr bool is_instance = false;
    };

    template< typename R, typename... As >
    struct continuation< R(*)( As... ) >{
        template< typename F, typename G >
        static auto pipe( F&& f, G&& g ){
            return [=](As... args ){
                return g( f( std::forward<As>( args )... ) );
            };
        }
        static constexpr bool is_instance = true;
    };
    
    template< typename R, typename... As >
    struct continuation< R( As... ) >{
        template< typename F, typename G >
        static auto pipe( F&& f, G&& g ){
            return [=](As... args ){
                return g( f( std::forward<As>( args )... ) );
            };
        }
        static constexpr bool is_instance = true;
    };

    template< typename F, typename G >
    auto pipe ( F&& f, G&& g ) -> decltype( continuation<F>::pipe( f, g ) ){
        return continuation<F>::pipe( f, g );
    }

} //namespace functional

#endif
