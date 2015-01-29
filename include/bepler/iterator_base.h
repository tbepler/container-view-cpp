#ifndef INCLUDED_BEPLER_ITERATOR_BASE_H
#define INCLUDED_BEPLER_ITERATOR_BASE_H

#include <type_traits>
#include <iterator>

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

    Derived& operator++(){
        derived().inc();
        return derived();
    }

    Derived operator++( int ){
        Derived cpy( derived() );
        derived().inc();
        return cpy;
    }

    protected:
        Derived& derived(){
            return static_cast<Derived&>( *this );
        }
        const Derived& derived() const{
            return static_cast<const Derived&>( *this );
        }

}; //struct IteratorBase

template<
    typename Derived,
    typename Value,
    typename Reference = Value&,
    typename Pointer = Value*,
    typename Category = std::input_iterator_tag,
    typename Difference = std::ptrdiff_t
>
struct InputIteratorBase : public IteratorBase<Derived,Value,Category,Reference,Pointer,Difference>{

    private:
        typedef IteratorBase<Derived,Value,Category,Reference,Pointer,Difference> base;

    public:
        typename base::reference operator*() const{
            return base::derived().dereference();
        }

        typename base::pointer operator->() const{
            return base::derived().get();
        }

        friend bool operator==( const Derived& a, const Derived& b ){
            return a.compareTo( b ) == 0;
        }

        friend bool operator!=( const Derived& a, const Derived& b ){
            return a.compareTo( b ) != 0;
        }


}; //struct InputIteratorBase

template<
    typename Derived,
    typename Value,
    typename Reference = Value&,
    typename Pointer = Value*,
    typename Category = std::output_iterator_tag,
    typename Difference = std::ptrdiff_t
>
struct OutputIteratorBase : public IteratorBase<Derived,Value,Category,Reference,Pointer,Difference>{

    private:
        typedef IteratorBase<Derived,Value,Category,Reference,Pointer,Difference> base;

    public:
        typename base::reference operator*() const{
            return base::derived().dereference();
        }

}; //struct OutputIteratorBase

template<
    typename Derived,
    typename Value,
    typename Reference = Value&,
    typename Pointer = Value*,
    typename Category = std::forward_iterator_tag,
    typename Difference = std::ptrdiff_t
>
struct ForwardIteratorBase : public InputIteratorBase<Derived,Value,Reference,Pointer,Category,Difference>{

}; //struct ForwardIteratorBase

template<
    typename Derived,
    typename Value,
    typename Reference = Value&,
    typename Pointer = Value*,
    typename Category = std::bidirectional_iterator_tag,
    typename Difference = std::ptrdiff_t
>
struct BidirectionalIteratorBase : public ForwardIteratorBase<Derived,Value,Reference,Pointer,Category,Difference>{

    private:
        typedef ForwardIteratorBase<Derived,Value,Reference,Pointer,Category,Difference> base;

    public:
        Derived& operator--(){
            base::derived().dec();
            return *base::derived();
        }

        Derived operator--( int ){
            Derived cpy( *base::derived() );
            base::derived().dec();
            return cpy;
        }

}; //struct BidirectionalIteratorBase

template<
    typename Derived,
    typename Value,
    typename Reference = Value&,
    typename Pointer = Value*,
    typename Category = std::random_access_iterator_tag,
    typename Difference = std::ptrdiff_t
>
struct RandomAccessIteratorBase : public BidirectionalIteratorBase<Derived,Value,Reference,Pointer,Category,Difference>{

    private:
        typedef BidirectionalIteratorBase<Derived,Value,Reference,Pointer,Category,Difference> base;

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

#endif
