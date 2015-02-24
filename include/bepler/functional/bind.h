#ifndef INCLUDED_BEPLER_FUNCTIONAL_BIND_H
#define INCLUDED_BEPLER_FUNCTIONAL_BIND_H

#include <utility>

namespace functional{

    template< typename F, typename T >
    struct Bind{

        template< typename A, typename B >
        constexpr Bind( A&& a, B&& b ) : f_( std::forward<A>( a ) ),
            x_( std::forward<B>( b ) ) { }
        
        template< typename... Params >
        constexpr auto operator()( Params&&... args ){
            return f_( x_, std::forward<Params>( args )... );
        }
/*
        template< typename... Params >
        inline auto operator()( Params&&... args ) const{
            return f_( x_, std::forward<Params>( args )... );
        }
*/
        private:
            F f_;
            T x_;
    };

    //when F or T are rvalue reference - use value member
    //instead of rvalue reference member
    template< typename F, typename T >
    struct Bind< F&&, T > : public Bind< F, T > {
        using super_t = Bind<F,T>;
        using super_t::super_t;
    };
    
    template< typename F, typename T >
    struct Bind< F, T&& > : public Bind< F, T > {
        using super_t = Bind<F,T>;
        using super_t::super_t;
    };

    template< typename F, typename T >
    struct Bind< F&&, T&& > : public Bind< F, T > {
        using super_t = Bind<F,T>;
        using super_t::super_t;
    };

    template< typename F, typename T >
    constexpr auto bind( F&& f, T&& x ){
        return Bind< decltype(f), decltype(x) >(
            std::forward<F>( f ), std::forward<T>( x ) );
    }

    template< typename F, typename T, typename... Ts >
    constexpr auto bind( F&& f, T&& x, Ts&&... xs ){
        return bind( bind( std::forward<F>( f ), std::forward<T>( x ) ),
            std::forward<Ts>( xs )... );
    }

} //namespace functional

#endif
