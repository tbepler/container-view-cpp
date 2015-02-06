#ifndef INCLUDED_BEPLER_ITERTOOLS_GENERATOR_H
#define INCLUDED_BEPLER_ITERTOOLS_GENERATOR_H

#include <utility>
#include <type_traits>
#include "bepler/itertools/iterator_base.h"
#include "bepler/itertools/pointer_facade.h"

namespace itertools{

    /*
    template< typename T >
    interface Generator{
        bool hasNext() const;
        T next();
    };
    */

    namespace helpers{

        template< typename T >
        struct defines_has_next{
            template< typename U > static char test( char(*)[ sizeof(
                std::declval<T>().done() ) ] );
            template< typename U > static long test(...);
            enum { value = 1 == sizeof test<T>(0) };
        };

    } //namespace itertools::helpers

    template< typename Generator >
    class GeneratorIterator : public InputIteratorBase< GeneratorIterator<Generator>, decltype( std::declval<Generator>().get() ), decltype( std::declval<Generator>().get() ) >{
        
        typedef decltype( std::declval<Generator>().get() ) value_t;

        Generator g_;

        public:
            GeneratorIterator() : g_( ) { }
            GeneratorIterator( const Generator& g ) : g_( g ) { }
            //GeneratorIterator( Generator& g, const value_t& val ) : g_( &g ), advance_( true ), val_( val ) { }

            template< typename U = Generator >
            typename std::enable_if< helpers::defines_has_next<U>::value, bool >::type
            inline equals( const GeneratorIterator& b ) const{
                return ( b.g_.done() && g_.done() )/* || ( g_ == b.g_ )*/;
            }

            template< typename U = Generator >
            typename std::enable_if< !helpers::defines_has_next<U>::value && std::is_convertible<U,bool>::value, bool >::type
            inline equals( const GeneratorIterator& b ) const{
                return ( b.g_ == NULL && !(*g_) ) || ( g_ == b.g_ );
            }

            template< typename U = Generator >
            typename std::enable_if< !helpers::defines_has_next<U>::value && !std::is_convertible<U,bool>::value, bool >::type
            inline equals( const GeneratorIterator& b ) const{
                return g_ == b.g_;
            }

            inline value_t dereference() const{
                return g_.get();
            }
            inline value_t* arrow() const{ 
                return &g_.get();
            }
            inline void inc(){
                g_.next();
            }
        

    };

    template< typename Derived, typename Value, typename Reference = Value >
    struct Generator : public InputIteratorBase< Derived,
        Value, Reference > {

        inline Derived& begin() { return derived(); }
        inline const Derived& begin() const { return derived(); }
        inline const Derived& end() const{ return derived(); }
        inline bool equals( const Derived& b ) const{ return derived().done() || b.done(); }
        inline Reference dereference() const{ return derived().get(); }
        inline void inc(){ derived().next(); }

        private:
            inline Derived& derived(){ return *static_cast<Derived*>(this); }
            inline const Derived& derived() const{ return *static_cast<const Derived*>(this); }

    };

} //namespace itertools

#endif
