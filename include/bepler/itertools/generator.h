#ifndef INCLUDED_BEPLER_ITERTOOLS_GENERATOR_H
#define INCLUDED_BEPLER_ITERTOOLS_GENERATOR_H

#include <utility>
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
        G* gen_;
        public:
            GeneratorIterator() : gen_( NULL ) { }
            GeneratorIterator( G& gen ) : gen_( gen ? &gen : NULL ) { }
            inline return_t dereference() const{ return gen_->get(); }
            void inc(){
                if( gen_ != NULL ){
                    gen_->next();
                    if( gen_->done() ){
                        gen_ = NULL;
                    }
                }
                //if( gen_ != NULL && !gen_->next() ) gen_ = NULL;
            }
            inline bool equals( const GeneratorIterator& that ) const{
                return gen_ == that.gen_;
            }
    };

    template< typename Derived, typename Value >
    struct Generator{
        
        using iterator = GeneratorIterator<Derived>;
        using const_iterator = GeneratorIterator<const Derived>;

        inline iterator begin() { return iterator( derived() ); }
        inline iterator end() { return iterator(); }
        inline const_iterator begin() const { return const_iterator( derived() ); }
        inline const_iterator end() const { return const_iterator(); }
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
