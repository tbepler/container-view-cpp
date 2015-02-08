#ifndef INCLUDED_BEPLER_ITERTOOLS_RANGE_H
#define INCLUDED_BEPLER_ITERTOOLS_RANGE_H

#include <algorithm>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "bepler/itertools/iterator_base.h"
#include "bepler/itertools/generator.h"
#include "bepler/comparable.h"
#include "bepler/type_traits/iterator_traits.h"
#include "bepler/type_traits/type_traits_extended.h"
#include "bepler/math/math.h"

namespace itertools{

    namespace helpers{
        
        template< typename T >
        using step_t = typename type_traits::safe_make_signed<T>::type;

        template< typename T, typename U = T >
        using diff_t = type_traits::safe_signed_diff_t<T,U>;

        template< typename T, typename U >
        inline typename std::enable_if<
            std::is_signed< type_traits::diff_type<T,U> >::value,
            diff_t<T,U> >::type
                difference( const T& lhs, const U& rhs ){
            return lhs - rhs;
        }

        template< typename T, typename U = T >
        inline typename std::enable_if<
            !std::is_signed< type_traits::diff_type<T,U> >::value,
            diff_t<T,U> >::type
                difference( const T& lhs, const U& rhs ){
            return lhs < rhs ? -( (diff_t<T,U>)( rhs - lhs ) ) : lhs - rhs;
        }

        template< typename T, typename U >
        inline std::ptrdiff_t numSteps( const T& diff, const U& step ){
            return math::roundAwayFromZero( diff / (double) step );
        }

        template< typename T, typename U >
        inline std::ptrdiff_t numSteps( const T& first, const T& last, const U& step ){
            return numSteps( difference( last, first ), step );
        }

    } //namespace itertools::helpers

    template< typename R >
    struct range_traits{
        using value_type = typename std::iterator_traits< decltype( type_traits::rvalue<R>().begin() )>::value_type;
        using reference = typename std::iterator_traits< decltype( type_traits::rvalue<R>().begin() ) >::reference;
    };

    template< typename T >
    class ValueIterator : public RandomAccessIteratorBase< ValueIterator< T >, const T >{
        T val_;
        public:
            ValueIterator() : val_() { }
            ValueIterator( const T& val ) : val_( val ) { }
            inline const T& dereference() const{ return val_; }
            template< typename U >
            inline bool equals( const ValueIterator<U>& o ) const{ return val_ == o.val_; }
            inline bool equals( const T& o ) const{ return val_ == o; }
            template< typename U >
            inline std::ptrdiff_t compareTo( const ValueIterator<U>& o ) const{
                return val_ - o.val_;
            }
            inline void inc(){ ++val_; }
            inline void dec(){ --val_; }
            template< typename N >
            inline void advance( N n ){ val_ += n; }

    };

    template< typename T >
    class IRange : public Comparable< IRange<T> >{
        
        using value_t = typename std::conditional<
            type_traits::is_iterator<T>::value,
            T,
            ValueIterator<T>
        >::type;

        value_t begin_;
        value_t end_;
        public:
            IRange() : begin_(), end_() { }
            IRange( const T& last ) : begin_(), end_( last ) { }
            IRange( const T& first, const T& last ) : begin_( first ), end_( last ) { }
            inline value_t begin() const{ return begin_; }
            inline value_t end() const{ return end_; }
            std::size_t size() const{
                auto dist = std::distance( begin_, end_ );
                return dist < 0 ? -dist : dist;
            }
            inline typename std::iterator_traits<value_t>::reference
            operator[]( typename std::iterator_traits<value_t>::difference_type n ) const{
                return *std::next( begin_, n );
            }
            template< typename U >
            typename std::iterator_traits<value_t>::difference_type compareTo( const IRange<U>& o ) const{
                if( begin_ != o.begin_ ){
                    return begin_ - o.begin_;
                }
                return end_ - o.end_;
            }
            IRange& operator++(){
                ++begin_, ++end_;
                return *this;
            }
            IRange operator++(int){
                IRange cpy( *this );
                ++(*this);
                return cpy;
            }
            IRange& operator--(){
                --begin_, --end_;
                return *this;
            }
            IRange operator--(int){
                IRange cpy( *this );
                --(*this);
                return cpy;
            }
            template< typename N >
            IRange& operator+=( const N& n ){
                begin_ += n;
                end_ += n;
                return *this;
            }
            template< typename N >
            IRange& operator-=( const N& n ){
                begin_ -= n;
                end_ -= n;
                return *this;
            }
    };

    template< typename T, typename N >
    IRange<T> operator+( const IRange<T>& a, const N& b ){
        IRange<T> cpy( a );
        cpy += b;
        return cpy;
    }

    template< typename N, typename T >
    inline IRange<T> operator+( const N& b, const IRange<T>& a ){
        return a + b;
    }

    template< typename T, typename N >
    IRange<T> operator-( const IRange<T>& a, const N& b ){
        IRange<T> cpy( a );
        cpy -= b;
        return cpy;
    }

/*
    template< typename T, bool is_iter = type_traits::is_iterator<T>::value >
    class StepIterator;
    
    template< typename T >
    class StepIterator<T,false>: public RandomAccessIteratorBase< StepIterator<T>, const T >{
        using step_t = helpers::diff_t<T>; 
        T val_;
        step_t step_;
        public:
            StepIterator() : val_(), step_( 1 ) { }
            StepIterator( const T& val ) : val_( val ), step_( 1 ) { }
            StepIterator( const T& val, const step_t& step )
                : val_( val ), step_( step ) { }
            inline const T& dereference() const{ return val_; }
            bool equals( const StepIterator& o ){
                step_t s = math::max( step_, o.step_ );
                T lower = math::min( val_ - s, val_ + s );
                T upper = math::max( val_ - s, val_ + s );
                return math::between( val_, lower, upper );
            }
            

    };
*/

    template< typename T, typename U = T >
    class RangeIterator
        : public RandomAccessIteratorBase< RangeIterator< T, U >, const T >{

        typedef U step_t;

        T val_;
        step_t step_;

        public:
            RangeIterator( ) : val_(), step_() { }
            RangeIterator( const T& val, const step_t& step )
                : val_( val ), step_( step ) { } 
            inline const T& dereference() const{ return val_; }
            bool equals( const RangeIterator& rhs ) const{
                if( step_ >= 0 && val_ >= rhs.val_ ) return true;
                if( step_ <= 0 && val_ <= rhs.val_ ) return true;
                return false;
            }
            inline std::ptrdiff_t compareTo( const RangeIterator& rhs ) const{
                return -helpers::numSteps( val_, rhs.val_, step_ );
            }
            inline void inc(){ val_ += step_; }
            inline void dec(){ val_ -= step_; }
            inline void advance( int n ){ val_ += step_ * n; }

    }; //class RangeIterator
/*
    template< typename T >
    class XRange : public Generator< XRange<T>, T >{
        using step_t = typename helpers::diff_t<T>;
        mutable T cur_;
        T end_;
        step_t step_;
        public:
            XRange() : cur_(), end_(), step_() { }
            XRange( const T& end ) : cur_(), end_( end ), step_( end_ < cur_ ? -1 : 1 ) { }
            XRange( const T& begin, const T& end ) : cur_( begin ), end_( end ), step_( end_ < cur_ ? -1 : 1 ) { }
            XRange( const T& begin, const T& end, const step_t& step ) : cur_( begin ), end_( end ), step_( step ) { }
            inline bool done() const{ return cur_ == end_ || ( step_ < 0 ? cur_ < end_ : cur_ > end_ ); }
            inline bool next() const { cur_ += step_; return !done(); }
            inline T get() const{ return cur_; }
            inline T operator[]( std::size_t i ) const{ return cur_ + i * step_; }
            inline std::size_t size() const{ return helpers::numSteps( cur_, end_, step_ ); }
    };
*/
    /*
    * Class for describing an iterable range (first,last] over elements of type T
    */

    
    template< typename T, typename U = T >
    class XRange : public Comparable< XRange<T,U> >{
        
        typedef typename type_traits::safe_make_signed<U>::type step_t;
        typedef T value_type;

        T first_;
        T last_;
        step_t step_;

        void assertConditions(){
            if( step_ == 0 ){
                throw std::logic_error( "Step size must not be 0" );
            }
            if( last_ < first_ && step_ >= 0 ){
                throw std::logic_error( "Step size must be negative if last < first" );
            }
            if( first_ < last_ && step_ <= 0 ){
                throw std::logic_error( "Step size must be positive if last > first" );
            }
        }

        public:

            XRange( ) : first_( 0 ), last_( 0 ), step_( 1 ) { }
            XRange( const T& first_val, const T& last_val, const step_t& step_val)
                : first_( first_val ), last_( last_val ), step_( step_val ) {
                assertConditions();
            }
            XRange( const T& first_val, const T& last_val )
                : XRange( first_val, last_val, first_val <= last_val ? 1 : -1 ) { }
            XRange( const T& last_val ) : XRange( 0, last_val ) { }

            template< typename T1, typename U1 >
            XRange( const XRange<T1,U1>& rhs ) : XRange( rhs.first(), rhs.last(), rhs.step() ) { }

            inline std::size_t size() const {
                return helpers::numSteps( first_, last_, step_ );
            }

            T operator[] ( std::size_t i ) const{
                if( i < size() ){
                    return first_ + i * step_;
                }
                throw std::out_of_range( "Index out of bounds: " + std::to_string( i ) ); 
            }

            inline RangeIterator<T,step_t> begin() const{
                return RangeIterator<T,step_t>( first_, step_ );
            }

            inline RangeIterator<T,step_t> end() const{
                return RangeIterator<T,step_t>( last_, step_ );
            }

            inline XRange reverse() const{
                return XRange( last_, first_, -step_ );
            }

            template< typename T1, typename U1 >
            auto compareTo( const XRange<T1,U1>& rhs ) const
                -> decltype( helpers::difference( type_traits::rvalue<T>(), type_traits::rvalue<T1>() ) )
            {
                if( first_ != rhs.first() ){
                    return helpers::difference( first_, rhs.first() );
                } 
                return helpers::difference( last_, rhs.last() );
            }

            template< typename T1, typename U1 >
            XRange& operator=( const XRange<T1,U1>& rhs ){
                first_ = rhs.first();
                last_ = rhs.last();
                step_ = rhs.step();
                assertConditions();
                return *this;
            }

            //getters and setters
            inline const T& first() const{
                return first_;
            }

            XRange& first( const T& first_val ){
                first_ = first_val;
                assertConditions();
                return *this;
            }

            inline const T& last() const{
                return last_;
            }

            XRange& last( const T& last_val ){
                last_ = last_val;
                assertConditions();
                return *this;
            }

            inline const step_t& step() const{
                return step_;
            }

            XRange& step( const step_t& step_val ){
                step_ = step_val;
                assertConditions();
                return *this;
            }

    };

    template< typename T >
    IRange<T> irange( const T& last ){
        return IRange<T>( last );
    }

    template< typename T >
    IRange<T> irange( const T& first, const T& last ){
        return IRange<T>( first, last );
    }
    
    template< typename T >
    IRange<T> range( const T& last ){
        return IRange<T>( last );
    }

    template< typename T >
    IRange<T> range( const T& first, const T& last ){
        return IRange<T>( first, last );
    }

    template< typename T, typename U >
    XRange<T> range( const T& first, const T& last, const U& step ){
        return XRange<T>( first, last, step );
    }

    template< typename T >
    XRange<T> xrange( const T& last ){
        return XRange<T>( last );
    }

    template< typename T >
    XRange<T> xrange( const T& first, const T& last ){
        return XRange<T>( first, last );
    }

    template< typename T, typename U >
    XRange<T> xrange( const T& first, const T& last, const U& step ){
        return XRange<T>( first, last, step );
    }

} //namespace itertools

#endif
