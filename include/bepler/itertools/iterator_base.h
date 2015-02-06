#ifndef INCLUDED_BEPLER_ITERATOR_BASE_H
#define INCLUDED_BEPLER_ITERATOR_BASE_H

#include "bepler/itertools/pointer_facade.h"
#include <type_traits>
#include <iterator>

namespace itertools{

    namespace helpers{

        template< typename Reference >
        using pointer_t = typename std::conditional<
            std::is_reference<Reference>::value,
            typename std::add_pointer<Reference>::type,
            PointerFacade<Reference>
        >::type;

    } //namespace itertools::helpers
    
    template<
        typename Derived,
        typename Value,
        typename Category,
        typename Reference = Value&,
        typename Pointer = Value*,
        typename Difference = std::ptrdiff_t
    >
    struct IteratorBase{
    
        typedef Difference difference_type;
        typedef Value value_type;
        typedef Pointer pointer;
        typedef Reference reference;
        typedef Category iterator_category;
    
        inline Derived& operator++(){
            derived().inc();
            return derived();
        }
    
        inline Derived operator++( int ){
            Derived cpy( derived() );
            derived().inc();
            return cpy;
        }
    
        protected:
            inline Derived& derived(){
                return static_cast<Derived&>( *this );
            }
            inline const Derived& derived() const{
                return static_cast<const Derived&>( *this );
            }
    
    }; //struct IteratorBase
    
    template<
        typename Derived,
        typename Value,
        typename Reference = Value&,
        typename Category = std::input_iterator_tag,
        typename Difference = std::ptrdiff_t
    >
    struct InputIteratorBase
        : public IteratorBase<
            Derived,
            Value,
            Category,
            Reference,
            typename helpers::pointer_t<Reference>,
            Difference
        >
    {
    
        private:
            typedef IteratorBase<Derived,Value,Category,Reference,typename helpers::pointer_t<Reference>,Difference> base;
   
            template< typename R = Reference >
            inline typename std::enable_if< std::is_pointer< helpers::pointer_t<R> >::value, helpers::pointer_t<R> >::type makePointer( R& ref ) const{
                return &ref;
            }

            template< typename R = Reference >
            inline typename std::enable_if< !std::is_pointer< helpers::pointer_t<R> >::value, helpers::pointer_t<R> >::type makePointer( const R& ref ) const{
                return helpers::pointer_t<R>( ref );
            }

        public:
    
            inline typename base::reference operator*() const{
                return base::derived().dereference();
            }
    
            inline typename base::pointer operator->() const{
                return makePointer( this->operator*() );
            }
   
            template< typename T >
            inline friend bool operator==( const Derived& a, const T& b ){
                return a.equals( b );
            }
    
            template< typename T >
            inline friend bool operator!=( const Derived& a, const T& b ){
                return !a.equals( b );
            }
    
    
    }; //struct InputIteratorBase
    
    template<
        typename Derived,
        typename Value,
        typename Reference = Value&,
        typename Category = std::output_iterator_tag,
        typename Difference = std::ptrdiff_t
    >
    struct OutputIteratorBase : public IteratorBase<Derived,Value,Category,Reference,Value*,Difference>{
    
        private:
            typedef IteratorBase<Derived,Value,Category,Reference,Value*,Difference> base;
    
        public:
            typename base::reference operator*() const{
                return base::derived().dereference();
            }
    
    }; //struct OutputIteratorBase
    
    template<
        typename Derived,
        typename Value,
        typename Reference = Value&,
        typename Category = std::forward_iterator_tag,
        typename Difference = std::ptrdiff_t
    >
    struct ForwardIteratorBase : public InputIteratorBase<Derived,Value,Reference,Category,Difference>{
    
    }; //struct ForwardIteratorBase
    
    template<
        typename Derived,
        typename Value,
        typename Reference = Value&,
        typename Category = std::bidirectional_iterator_tag,
        typename Difference = std::ptrdiff_t
    >
    struct BidirectionalIteratorBase : public ForwardIteratorBase<Derived,Value,Reference,Category,Difference>{
    
        private:
            typedef ForwardIteratorBase<Derived,Value,Reference,Category,Difference> base;
    
        public:
            Derived& operator--(){
                base::derived().dec();
                return base::derived();
            }
    
            Derived operator--( int ){
                Derived cpy( base::derived() );
                base::derived().dec();
                return cpy;
            }
    
    }; //struct BidirectionalIteratorBase
    
    template<
        typename Derived,
        typename Value,
        typename Reference = Value&,
        typename Category = std::random_access_iterator_tag,
        typename Difference = std::ptrdiff_t
    >
    struct RandomAccessIteratorBase : public BidirectionalIteratorBase<Derived,Value,Reference,Category,Difference>{
    
        private:
            typedef BidirectionalIteratorBase<Derived,Value,Reference,Category,Difference> base;
    
        public:
            friend bool operator< (const Derived& a, const Derived& b ){
                return a.compareTo( b ) < 0;
            }
    
            friend bool operator<= ( const Derived& a, const Derived& b ){
                return a.compareTo( b ) <= 0;
            }
    
            friend bool operator> ( const Derived& a, const Derived& b ){
                return a.compareTo( b ) > 0;
            }
    
            friend bool operator>= ( const Derived& a, const Derived& b ){
                return a.compareTo( b ) >= 0;
            }
    
            friend typename base::difference_type operator- ( const Derived& a, const Derived& b ){
                return a.compareTo( b );
            }
    
            Derived& operator+= ( signed n ){
                base::derived().advance( n );
                return base::derived();
            }
    
            Derived& operator-= ( signed n ){
                base::derived().advance( -n );
                return base::derived();
            }
    
            typename base::reference operator[] ( signed n ) const{
                return *( base::derived() + n );
            }
    
            friend Derived operator+ ( const Derived& a, signed n ){
                Derived d( a );
                d += n;
                return d;
            }
    
            friend inline Derived operator+ ( signed n, const Derived& a ){
                return a + n;
            }
    
            friend Derived operator- ( const Derived& a, signed n ){
                Derived d( a );
                d -= n;
                return d;
            }
    
    }; //struct RandomAccessIteratorBase

    template< typename T >
    class RawIterator : public RandomAccessIteratorBase< RawIterator<T>, T >{

        friend class RandomAccessIteratorBase< RawIterator<T>, T >;

        T val_;

        const T& dereference() const{ return val_; }
        std::ptrdiff_t compareTo( const RawIterator& rhs ) const{ return rhs.val_ - val_; }
        bool equals( const RawIterator& rhs ) const{ return val_ == rhs.val_; }
        void inc(){ ++val_; }
        void dec(){ --val_; }
        void advance( int i ){ val_ += i; }

    }; //class RawIterator
    
} //namespace itertools

#endif
