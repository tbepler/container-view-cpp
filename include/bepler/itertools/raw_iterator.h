#ifndef INCLUDED_BEPLER_ITERTOOLS_RAW_ITERATOR_H
#define INCLUDED_BEPELR_ITERTOOLS_RAW_ITERATOR_H

#include "bepler/itertools/iterator_base.h"

namespace itertools{

    template< typename T >
    class RawIterator : public RandomAccessIterator< RawIterator<T>, T, T >{

        friend class RandomAccessIterator< RawIterator<T>, T >::base;

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
