#ifndef INCLUDED_BEPLER_ITERTOOLS_PAIR_H
#define INCLUDED_BEPLER_ITERTOOLS_PAIR_H

#include <iterator>
#include <utility>

namespace itertools{

    template< typename I >
    constexpr auto pair( I begin, I end ){
        return std::pair<I,I>( begin, end );
    }

} //namespace itertools

namespace std{

    template< typename I >
    constexpr I begin< std::pair<I,I> >( const std::pair<I,I>& r ){
        return r.first;
    }

    template< typename I >
    constexpr I end< std::pair<I,I> >( const std::pair<I,I>& r ){
        return r.second;
    }


} //namespace std

#endif
