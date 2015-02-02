#ifndef INCLUDED_BEPLER_ITERTOOLS_RANGE_H
#define INCLUDED_BEPLER_ITERTOOLS_RANGE_H

#include <algorithm>
#include <stdexcept>
#include <string>
#include <cmath>

#include "bepler/itertools/iterator_base.h"
#include "bepler/comparable.h"
#include "bepler/type_traits/type_traits_extended.h"
#include "bepler/math/math.h"

namespace itertools{

    namespace helpers{
        
        template< typename T >
        inline T numSteps( const T& diff, const T& step ){
            return std::ceil( diff / (double) step );
        }

        template< typename T, typename StepType >
        inline StepType numSteps( const T& first, const T& last, const StepType& step ){
            return numSteps( last - first, step );
        }

    } //namespace itertools::helpers

    template< typename T, typename StepType = type_traits::diff_type<T> >
    class RangeIterator
        : public RandomAccessIteratorBase< RangeIterator< T, StepType >, const T >{

        T val_;
        StepType step_;

        public:
            RangeIterator( const T& val, const StepType& step )
                : val_( val ), step_( step ) { } 
            inline const T& dereference() const{ return val_; }
            inline const T* get() const{ return &val_; }
            bool equals( const RangeIterator& rhs ) const{
                if( val_ >= rhs.val_ && step_ >= 0 ) return true;
                if( val_ <= rhs.val_ && step_ <= 0 ) return true;
                return false;
            }
            std::ptrdiff_t compareTo( const RangeIterator& rhs ) const{
                return helpers::numSteps( val_ - rhs.val_, step_ );
            }
            inline void inc(){ val_ += step_; }
            inline void dec(){ val_ -= step_; }
            inline void advance( int n ){ val_ += step_ * n; }

    }; //class RangeIterator

    /*
    * Class for describing an iterable range (first,last] over elements of type T
    */
    template< typename T >
    class Range : public Comparable< Range<T> >{
        
        typedef type_traits::diff_type<T> step_type;
        typedef T value_type;

        T first_;
        T last_;
        step_type step_;

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
            Range( const T& first_val, const T& last_val, const step_type& step_val)
                : first_( first_val ), last_( last_val ), step_( step_val ) {
                assertConditions();
            }
            Range( const T& first_val, const T& last_val )
                : Range( first_val, last_val, first_val <= last_val ? 1 : -1 ) { }
            Range( const T& last_val ) : Range( 0, last_val ) { }

            template< typename T1 >
            Range( const Range<T1>& rhs ) : Range( rhs.first(), rhs.last(), rhs.step() ) { }

            inline std::size_t size() const {
                return helpers::numSteps( first_, last_, step_ );
            }

            T operator[] ( std::size_t i ) const{
                if( i < size() ){
                    return first_ + i * step_;
                }
                throw std::out_of_range( "Index out of bounds: " + std::to_string( i ) ); 
            }

            inline RangeIterator<T,step_type> begin() const{
                return RangeIterator<T,step_type>( first_, step_ );
            }

            inline RangeIterator<T,step_type> end() const{
                return RangeIterator<T,step_type>( last_, step_ );
            }

            inline Range reverse() const{
                //TODO not well defined when type is unsigned
                return Range( last_, first_, -step_ );
            }

            int compareTo( const Range& rhs ) const{
                if( first_ == rhs.first_ ){
                    return last_ - rhs.last_;
                }
                return first_ - rhs.first_;
            }

            template< typename T1 >
            Range& operator=( const Range<T1>& rhs ){
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

            inline const step_type& step() const{
                return step_;
            }

            Range& step( const step_type& step_val ){
                step_ = step_val;
                assertConditions();
                return *this;
            }

    };

    template< typename T >
    Range<T> range( const T& last ){
        return Range<T>( last );
    }

    template< typename T >
    Range<T> range( const T& first, const T& last ){
        return Range<T>( first, last );
    }

    template< typename T >
    Range<T> range( const T& first, const T& last, const type_traits::diff_type<T>& step ){
        return Range<T>( first, last, step );
    }

} //namespace itertools

#endif
