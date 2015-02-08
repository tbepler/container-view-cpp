#ifndef INCLUDED_BEPLER_ITERTOOLS_GENERATOR_H
#define INCLUDED_BEPLER_ITERTOOLS_GENERATOR_H

#include <utility>
#include <memory>
#include <type_traits>
#include <stdexcept>
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

    template< typename G >
    struct generator_traits{
        using value_type = typename std::decay<decltype( std::declval<G>().get() )>::type;
        using reference = decltype( std::declval<G>().get() );
    };

    template< typename G >
    class GeneratorIterator : public InputIteratorBase< GeneratorIterator<G>, typename generator_traits<G>::value_type, typename generator_traits<G>::reference >{
        using return_t = typename generator_traits<G>::reference;
        std::unique_ptr<G> gen_;
        public:
            GeneratorIterator() : gen_( (G*) NULL ) { }
            GeneratorIterator( const G& gen ) : gen_( gen ? new G( gen ) : (G*) NULL ) { }
            GeneratorIterator( const GeneratorIterator& o ) : gen_( o.gen_ ? new G( *o.gen_ ) : (G*) NULL ) { }
            GeneratorIterator& operator=( const GeneratorIterator& o ){
                gen_ = o.gen_ ? new G( *o.gen_ ) : (G*) NULL;
                return *this;
            }
            inline return_t dereference() const{ return gen_->get(); }
            void inc(){
                if( gen_ && !gen_->next() ){
                    gen_ = NULL;
                }
            }
            inline bool equals( const GeneratorIterator& that ) const{
                return gen_ == that.gen_;
            }
    };

    template< typename Derived, typename Value, typename Reference = Value >
    struct Generator : public InputIteratorBase<
        Generator< Derived, Value >,
        Value,
        Reference
    >{

        inline Derived& begin() { return derived(); }
        inline Derived begin() const{ return derived(); }
        inline Derived& end() { return derived(); }
        inline Derived end() const{ return derived(); }

        inline Reference dereference() const{ return derived().get(); }
        inline bool equals( const Derived& d ) const{ return derived().done() || d.done(); }
        inline void inc(){ derived().next(); }

        explicit operator bool() const{ return !derived().done(); }
        Value operator()(){
            if( derived().done() ){
                throw std::out_of_range( "Out of elements" );
            }
            Value v = derived().get();
            derived().next();
            return v;
        }

        private:
            inline Derived& derived(){ return *static_cast<Derived*>(this); }
            inline const Derived& derived() const{ return *static_cast<const Derived*>(this); }

    };

    template< typename Derived, typename Value, typename Reference = Value >
    struct BidirectionalGenerator : public BidirectionalIteratorBase<
        BidirectionalGenerator< Derived, Value >,
        Value,
        Reference
    >{

        inline Derived& begin() { return derived(); }
        inline Derived begin() const{ return derived(); }
        inline Derived& end() { return derived(); }
        inline Derived end() const{ return derived(); }

        inline Reference dereference() const{ return derived().get(); }
        inline bool equals( const Derived& d ) const{ return derived().done() || d.done(); }
        inline void inc(){ derived().next(); }
        inline void dec(){ derived().prev(); }

        explicit operator bool() const{ return !derived().done(); }
        Value operator()(){
            if( derived().done() ){
                throw std::out_of_range( "Out of elements" );
            }
            Value v = derived().get();
            derived().next();
            return v;
        }

        private:
            inline Derived& derived(){ return *static_cast<Derived*>(this); }
            inline const Derived& derived() const{ return *static_cast<const Derived*>(this); }

    };

} //namespace itertools

#endif
