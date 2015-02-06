#ifndef INCLUDED_BEPLER_ITERTOOLS_RANGE_H
#define INCLUDED_BEPLER_ITERTOOLS_RANGE_H

#include <algorithm>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "bepler/itertools/iterator_base.h"
#include "bepler/comparable.h"
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

    template< typename T >
    class ValueIterator : public RandomAccessIteratorBase< ValueIterator< T >, const T >{
        T val_;
        public:
            ValueIterator() : val_() { }
            ValueIterator( const T& val ) : val_( val ) { }
            inline const T& dereference const{ return val_; }
            inline bool equals( const ValueIterator& o ) const{ return val_ == o.val_; }
            inline bool equals( const T& o ) const{ return val_ == o; }
            inline std::ptrdiff_t compareTo( const ValueIterator& o ) const{
                return val_ - o.val_;
            }
            inline void inc(){ ++val_; }
            inline void dec(){ --val_; }
            template< typename N >
            inline void advance( N n ){ val_ += n; }

    };

    template< typename T >
    class IRange{
        
        using value_t = std::conditional<
            type_traits::is_iterator<T>::value,
            T,
            ValueIterator<T>
        >::type;

        value_t begin_;
        value_t end_;
        public:
            IRange() : begin_(), end() { }
            IRange( const T& first, const T& last ) : begin_( first ), end_( last ) { }
            inline value_t begin() const{ return begin_; }
            inline value_t end() const{ return end_; }
    };

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
            inline const T* arrow() const{ return &val_; }
            bool equals( const RangeIterator& rhs ) const{
                if( val_ >= rhs.val_ && step_ >= 0 ) return true;
                if( val_ <= rhs.val_ && step_ <= 0 ) return true;
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
    * Class for describing an iterable range (first,last] over elements of type T
    */
    template< typename T, typename U = T >
    class Range : public Comparable< Range<T,U> >{
        
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

            Range( ) : first_( 0 ), last_( 0 ), step_( 1 ) { }
            Range( const T& first_val, const T& last_val, const step_t& step_val)
                : first_( first_val ), last_( last_val ), step_( step_val ) {
                assertConditions();
            }
            Range( const T& first_val, const T& last_val )
                : Range( first_val, last_val, first_val <= last_val ? 1 : -1 ) { }
            Range( const T& last_val ) : Range( 0, last_val ) { }

            template< typename T1, typename U1 >
            Range( const Range<T1,U1>& rhs ) : Range( rhs.first(), rhs.last(), rhs.step() ) { }

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

            inline Range reverse() const{
                return Range( last_, first_, -step_ );
            }

            template< typename T1, typename U1 >
            auto compareTo( const Range<T1,U1>& rhs ) const
                -> decltype( helpers::difference( type_traits::rvalue<T>(), type_traits::rvalue<T1>() ) )
            {
                if( first_ != rhs.first() ){
                    return helpers::difference( first_, rhs.first() );
                } 
                return helpers::difference( last_, rhs.last() );
            }

            template< typename T1, typename U1 >
            Range& operator=( const Range<T1,U1>& rhs ){
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

            Range& first( const T& first_val ){
                first_ = first_val;
                assertConditions();
                return *this;
            }

            inline const T& last() const{
                return last_;
            }

            Range& last( const T& last_val ){
                last_ = last_val;
                assertConditions();
                return *this;
            }

            inline const step_t& step() const{
                return step_;
            }

            Range& step( const step_t& step_val ){
                step_ = step_val;
                assertConditions();
                return *this;
            }

    };

    template< typename T, typename U = int >
    Range<T,U> range( const T& last ){
        return Range<T,U>( last );
    }

    template< typename T, typename U = int >
    Range<T,U> range( const T& first, const T& last ){
        return Range<T,U>( first, last );
    }

    template< typename T, typename U >
    Range<T> range( const T& first, const T& last, const U& step ){
        return Range<T,U>( first, last, step );
    }

} //namespace itertools

#endif
