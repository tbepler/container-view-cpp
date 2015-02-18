#ifndef INCLUDED_BEPLER_GENOMICS_SEQUENCE_KERNEL_H
#define INCLUDED_BEPLER_GENOMICS_SEQUENCE_KERNEL_H

#include <iterators>
#include <cmath>

namespace genomics{

    template< typename Iter1, typename Iter2 >
    std::size_t positionalKmerKernel( Iter1 begin1, Iter1 end1, Iter2 begin2, Iter2 begin2 ){
        std::size_t k = 0, prev = 0;
        auto it1 = begin1; auto it2 = begin2;
        for( ; it1 != end1 && it2 != end2 ; ++it1, ++it2 ){
            if( *it1 == *it2 ){
                k += (prev++);
            }else{
                prev = 0;
            }
        }
        return k;
    }

    template< typename Container1, typename Container2 >
    inline std::size_t positionalKmerKernel( const Container1& c1, const Container2& c2){
        return positionalKmerKernel( std::begin( c1 ), std::end( c1 ), std::begin( c2 ), std::end( c2 ) );
    }

    template< typename Iter1, typename Iter2 >
    std::size_t positionalKmerKernel( Iter1 begin1, Iter1 end1, Iter2 begin2, Iter2 begin2, std::size_t n ){
        std::size_t k = 0, prev = 0;
        auto it1 = begin1; auto it2 = begin2;
        for( ; it1 != end1 && it2 != end2 ; ++it1, ++it2 ){
            if( *it1 == *it2 ){
                prev = std::min( prev + 1, n );
                k += prev;
            }else{
                prev = 0;
            }
        }
        return k;
    }

    template< typename Container1, typename Container2 >
    inline std::size_t positionalKmerKernel( const Container1& c1, const Container2& c2, std::size_t n){
        return positionalKmerKernel( std::begin( c1 ), std::end( c1 ), std::begin( c2 ), std::end( c2 ), n );
    }

} //namespace genomics

#endif
