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

    template< typename T >
    struct defines_has_next{
        template< typename U > static char test( char(*)[ sizeof(
            std::declval<T>().hasNext() ) ] );
        template< typename U > static long test(...);
        enum { value = 1 == sizeof test<T>(0) };
    };

    template< typename Generator, typename = std::is_function >
    class GeneratorIterator : public InputIteratorBase< GeneratorIterator<Generator>, decltype( std::declval<Generator>().next() ) >{
        
        typedef decltype( std::declval<Generator>().next() ) value_t;

        Generator* g_;
        value_t val_;

        public:
            GeneratorIterator( Generator& g ) : g_( &g ), val_( g() ) { }

            std::enable_if< defines_has_next<Generator>::value, bool >::type
            inline equals( const GeneratorIterator& b ) const{
                return ( b.g_ == NULL && !g_->hasNext() ) || ( g_ == b.g_ && val_ == b.val_ );
            }

            std::enable_if< !defines_has_next<Generator>::value && std::is_convertible<Generator,bool>::value, bool >::type
            inline equals( const GeneratorIterator& b ) const{
                return ( b.g_ == NULL && !(*g_) ) || ( g_ == b.g_ && val_ == b.val_ );
            }

            std::enable_if< !defines_has_next<Generator>::value && !std::is_convertible<Generator,bool>::value, bool >::type
            inline equals( const GeneratorIterator& b ) const{
                return g_ == b.g_ && val_ == b.val_;
            }

            inline const value_t& dereference() const{ return val_; }
            inline const value_t* arrow() const{ return &val_; }
            inline void inc(){ val_ = (*g_)(); }
        

    };

    template< typename Derived, typename Value >
    struct Generator : public InputIteratorBase< Derived,
        Value, PointerFacade<Value> >{
        
        inline Derived begin() const{ return derived(); }
        inline Derived& begin() { return derived(); }
        inline void* end() const{ return NULL; }
        inline bool equals( const Derived& b ) const{ return !derived().hasNext() && !b.hasNext(); }
        inline bool equals( void* b ) const{ return b == NULL && !derived().hasNext(); }
        inline Value dereference() const{ return derived().get(); }
        inline PointerFacade<Value> arrow() const{ return PointerFacade<Value>( derived().get() ); }
        inline void inc(){ derived().next(); }

        private:
            inline Derived& derived(){ return *static_cast<Derived*>(this); }
            inline const Derived& derived() const{ return *static_cast<const Derived*>(this); }

    };

} //namespace itertools

#endif
