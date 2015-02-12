#ifndef INCLUDED_BEPLER_ITERTOOLS_ROLLING_HASH_H
#define INCLUDED_BEPLER_ITERTOOLS_ROLLING_HASH_H

#include <iterator>
#include "bepler/math/math.h"
#include "bepler/itertools/range.h"
#include "bepler/itertools/generator.h"

namespace itertools{

    template< typename InputIterator >
    std::size_t makeHash( InputIterator begin, std::size_t k, std::size_t a ){
        std::size_t hash = 0;
        while( k > 0 ){
            hash += (*begin) * math::pow( a, --k );
            ++begin;
        }
        return hash;
    }

    template< typename RandomAccessIterator >
    std::size_t makeHash( RandomAccessIterator begin, RandomAccessIterator end, std::size_t a ){
        return makeHash( begin, std::distance( begin, end ), a );
    }

    template< typename Container >
    std::size_t makeHash( const Container& c, std::size_t a ){
        return makeHash( std::begin( c ), c.size(), a );
    }

    template< typename N >
    std::size_t nextHash( std::size_t hash, N first, N next, std::size_t k, std::size_t a ){
        hash -= first * math::pow( a, k - 1 );
        hash *= a;
        hash += next;
        return hash;
    }

    template< typename N >
    std::size_t prevHash( std::size_t hash, N prev, N last, std::size_t k, std::size_t a ){
        hash -= last;
        hash /= a;
        hash += prev * math::pow( a, k - 1 );
        return hash;
    }

    class RollingHash{
        std::size_t hash_;
        std::size_t a_;
        std::size_t k_;
        public:        
            RollingHash( std::size_t a ) : hash_( 0 ), a_( a ), k_( 0 ) { }
            explicit operator std::size_t() const{
                return hash_;
            }
            inline RollingHash& reset(){ hash_ = 0, k_ = 0; return *this; }
            inline std::size_t hash() const{ return hash_; }
            inline std::size_t width() const{ return k_; }
            friend inline bool operator==( const RollingHash& a, const std::size_t& b ){ return a.hash_ == b; }
            friend inline bool operator==( const std::size_t& b, const RollingHash& a ){ return a.hash_ == b; }
            friend inline bool operator!=( const RollingHash& a, const std::size_t& b ){ return a.hash_ != b; }
            friend inline bool operator!=( const std::size_t& b, const RollingHash& a ){ return a.hash_ != b; }
            //shift n into the hash from right
            friend RollingHash& operator<<( RollingHash& hash, unsigned n ){
                hash.hash_ *= hash.a_;
                hash.hash_ += n;
                ++hash.k_;
                return hash;
            }
            //shift n out of the hash from left
            friend RollingHash& operator<<( unsigned n, RollingHash& hash ){
                hash.hash_ -= n * math::pow( hash.a_, --hash.k_ );
                return hash;
            }
            //shift n into the hash from left
            friend RollingHash& operator>>( unsigned n, RollingHash& hash ){
                hash.hash_ += n * math::pow( hash.a_, hash.k_++ );
                return hash;
            }
            //shift n out of the hash from right
            friend RollingHash& operator>>( RollingHash& hash, unsigned n ){
                hash.hash_ -= n;
                hash.hash_ /= hash.a_;
                --hash.k_;
                return hash;
            }
    };


    template< typename BiIterator >
    class RollingHashIterator : public BidirectionalIterator<
        RollingHashIterator< BiIterator >,
        std::size_t,
        std::size_t
    >{
        using iterator = BiIterator;
        mutable iterator first_;
        mutable iterator end_;
        mutable RollingHash hash_;
        mutable int offset_;

        inline void next() const{
            *first_ << hash_ << *end_;
            ++first_, ++end_;
        }

        inline void prev() const{
            *(--first_) >> hash_ >> *(--end_);
        }

        public:
            RollingHashIterator( iterator first, std::size_t k, std::size_t a )
                : first_( first ), end_( first ), hash_( a ), offset_( 0 ){
                for( std::size_t i = 0 ; i < k ; ++i ){
                    hash_ << *end_;
                    ++end_;
                }
            }
            RollingHashIterator( iterator first, iterator end, std::size_t a )
                : first_( first ), end_( end ), hash_( a ), offset_( 0 ){
                for( iterator it = first ; it != end ; ++it ){
                    hash_ << *it;
                }
            }
            RollingHashIterator( std::size_t k, iterator end, std::size_t a ) : first_( end ), end_( end ), hash_( a ), offset_( 0 ) {
                for( std::size_t i = 0 ; i < k ; ++i ){
                    *(--first_) >> hash_; 
                }
            }
            inline std::size_t dereference() const{
                while( offset_ > 0 ){
                    next(), --offset_;
                }
                while( offset_ < 0 ){
                    prev(), ++offset_;
                }
                return hash_.hash();
            }
            inline void inc(){
                ++offset_;
            }
            inline void dec(){
                --offset_;
            }
            inline bool equals( const RollingHashIterator& that ) const{
                return std::distance( end_, that.end_ ) == offset_ - that.offset_;
            }
    };

    template< typename InputIterator >
    class RollingHashGenerator : public Generator<
        RollingHashGenerator<InputIterator>,
        std::size_t,
        std::size_t
    >{
        using iterator = InputIterator;
        iterator first_;
        iterator last_;
        iterator end_;
        RollingHash hash_;
        public:
            RollingHashGenerator( iterator begin, iterator end, std::size_t k, std::size_t a )
            : first_( begin ), last_( begin ), end_( end ), hash_( a ){
                for( std::size_t i = 0 ; i < k ; ++i ){
                    hash_ << *last_;
                    if( i != k - 1 ){
                        ++last_;
                    }
                }
            }
            inline bool done() const{ return last_ == end_; }
            inline std::size_t get() const{ return hash_.hash(); }
            inline bool next(){
                if( ++last_ == end_ ) return false;
                *first_ << hash_ << *last_;
                ++first_;
                return true;
            }
    };

    template< typename Container >
    auto rollingHashGenerator( const Container& c, std::size_t k, std::size_t a ) -> RollingHashGenerator< decltype( std::begin( c ) ) >{
        return RollingHashGenerator< decltype( std::begin( c ) ) >( std::begin( c ), std::end( c ), k, a );
    }

    template< typename BiIterator >
    auto rollingHash( BiIterator begin, BiIterator end, std::size_t k, std::size_t a ) -> decltype( irange( std::declval<RollingHashIterator<BiIterator> >(), std::declval<RollingHashIterator<BiIterator> >() ) ){
        using iterator = BiIterator;
        RollingHashIterator<iterator> first( begin, k, a );
        RollingHashIterator<iterator> last( k, end, a );
        return irange( first, ++last );
    }

    template< typename Container >
    auto rollingHash( const Container& c, std::size_t k, std::size_t a ) -> decltype( rollingHash( std::begin( c ), std::end( c ), k, a ) ){
        return rollingHash( std::begin( c ), std::end( c ), k , a );
    }


} //namespace itertools

#endif
