#ifndef INCLUDED_BEPLER_ITERATOR_BASE_H
#define INCLUDED_BEPLER_ITERATOR_BASE_H

#include "bepler/type_traits/iterator_traits.h"
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
        typename Pointer = typename helpers::pointer_t<Reference>,
        typename Difference = std::ptrdiff_t
    >
    struct IteratorBase{
    
        typedef Difference difference_type;
        typedef Value value_type;
        typedef Pointer pointer;
        typedef Reference reference;
        typedef Category iterator_category;
    
        inline Derived& operator++(){
            return this->inc();
        }
    
        inline Derived operator++( int ){
            Derived cpy( derived() );
            this->inc();
            return cpy;
        }
    
        protected:
            inline Derived& derived(){
                return static_cast<Derived&>( *this );
            }
            inline const Derived& derived() const{
                return static_cast<const Derived&>( *this );
            }
            inline reference dereference() const{ return derived().dereference(); }
            inline Derived& inc() { derived().inc(); return derived(); }
            inline Derived& dec() { derived().dec(); return derived(); }
            inline Derived& advance( difference_type n ){ derived().advance( n ); return derived(); } 
            template< typename T >
            inline bool equals( const T& rhs ) const{ return derived().equals( rhs ); }
            template< typename T >
            inline difference_type compareTo( const T& rhs ) const{ return derived().compareTo( rhs ); }
    
    }; //struct IteratorBase
    
    template<
        typename Derived,
        typename Value,
        typename Reference = Value&,
        typename Category = std::input_iterator_tag,
        typename Difference = std::ptrdiff_t
    >
    struct InputIterator
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
            template< typename R = Reference >
            inline typename std::enable_if< std::is_pointer< helpers::pointer_t<R> >::value, helpers::pointer_t<R> >::type makePointer( R& ref ) const{
                return &ref;
            }

            template< typename R = Reference >
            inline typename std::enable_if< !std::is_pointer< helpers::pointer_t<R> >::value, helpers::pointer_t<R> >::type makePointer( const R& ref ) const{
                return helpers::pointer_t<R>( ref );
            }

        public:
            typedef IteratorBase<Derived,Value,Category,Reference,typename helpers::pointer_t<Reference>,Difference> base;
    
            inline typename base::reference operator*() const{
                return base::dereference();
            }
    
            inline typename base::pointer operator->() const{
                return makePointer( this->operator*() );
            }
   
            template< typename T >
            inline bool operator==( const T& b ) const{
                return base::equals( b );
            }
    
            template< typename T >
            inline bool operator!=( const T& b ) const{
                return !base::equals( b );
            }
    
    
    }; //struct InputIteratorBase
    
    template<
        typename Derived,
        typename Value,
        typename Reference = Value&,
        typename Category = std::output_iterator_tag,
        typename Difference = std::ptrdiff_t
    >
    struct OutputIterator : public IteratorBase<Derived,Value,Category,Reference,Value*,Difference>{
    
        typedef IteratorBase<Derived,Value,Category,Reference,Value*,Difference> base;
        
        typename base::reference operator*() const{
            return base::dereference();
        }
    
    }; //struct OutputIteratorBase
    
    template<
        typename Derived,
        typename Value,
        typename Reference = Value&,
        typename Category = std::forward_iterator_tag,
        typename Difference = std::ptrdiff_t
    >
    struct ForwardIterator : public InputIterator<Derived,Value,Reference,Category,Difference>{

        typedef typename InputIterator<Derived,Value,Reference,Category,Difference>::base base;
    
    }; //struct ForwardIteratorBase
    
    template<
        typename Derived,
        typename Value,
        typename Reference = Value&,
        typename Category = std::bidirectional_iterator_tag,
        typename Difference = std::ptrdiff_t
    >
    struct BidirectionalIterator : public ForwardIterator<Derived,Value,Reference,Category,Difference>{
    
        typedef typename ForwardIterator<Derived,Value,Reference,Category,Difference>::base base;

            Derived& operator--(){
                return base::dec();
            }
    
            Derived operator--( int ){
                Derived cpy( base::derived() );
                base::dec();
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
    struct RandomAccessIterator : public BidirectionalIterator<Derived,Value,Reference,Category,Difference>{

        typedef typename BidirectionalIterator<Derived,Value,Reference,Category,Difference>::base base;

            template< typename T >
            friend bool operator< (const Derived& a, const T& b ){
                return a.compareTo( b ) < 0;
            }
    
            template< typename T >
            friend bool operator<= ( const Derived& a, const T& b ){
                return a.compareTo( b ) <= 0;
            }
    
            template< typename T >
            friend bool operator> ( const Derived& a, const T& b ){
                return a.compareTo( b ) > 0;
            }
    
            template< typename T >
            friend bool operator>= ( const Derived& a, const T& b ){
                return a.compareTo( b ) >= 0;
            }
    
            typename base::difference_type operator- ( const Derived& b ) const{
                return base::compareTo( b );
            }
    
            Derived& operator+= ( typename base::difference_type n ){
                return base::advance( n );
            }
    
            Derived& operator-= ( typename base::difference_type n ){
                return base::advance( -n );
            }
    
            typename base::reference operator[] ( typename base::difference_type n ) const{
                return *( base::derived() + n );
            }
    
            friend Derived operator+ ( const Derived& a, typename base::difference_type n ){
                Derived d( a );
                d += n;
                return d;
            }
    
            friend inline Derived operator+ ( typename base::difference_type n, const Derived& a ){
                return a + n;
            }
    
            friend Derived operator- ( const Derived& a, typename base::difference_type n ){
                Derived d( a );
                d -= n;
                return d;
            }
    
    }; //struct RandomAccessIteratorBase
    
    template<
        typename Derived,
        typename Category,
        typename Value,
        typename Reference = Value&,
        typename Difference = std::ptrdiff_t
    >
    using iterator_base = typename type_traits::static_switch<
        Category,
        type_traits::static_case<
            std::input_iterator_tag,
            InputIterator< Derived, Value, Reference, Category, Difference >
        >,
        type_traits::static_case<
            std::output_iterator_tag,
            OutputIterator< Derived, Value, Reference, Category, Difference >
        >,
        type_traits::static_case<
            std::forward_iterator_tag,
            ForwardIterator< Derived, Value, Reference, Category, Difference >
        >,
        type_traits::static_case<
            std::bidirectional_iterator_tag,
            BidirectionalIterator< Derived, Value, Reference, Category, Difference >
        >,
        type_traits::static_case<
            std::random_access_iterator_tag,
            RandomAccessIterator< Derived, Value, Reference, Category, Difference >
        >
    >::type;

} //namespace itertools

#endif
