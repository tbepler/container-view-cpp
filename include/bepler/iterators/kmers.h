#ifndef INCLUDED_BEPLER_ITERATORS_KMERS_H
#define INCLUDED_BEPLER_ITERATORS_KMERS_H

#include <vector>
#include <iterator>
#include <type_traits>
#include <cstddef>
#include "bepler/containers/container.h"
#include "bepler/math/math.h"
#include "bepler/exceptions/no_such_element.h"

namespace iterators{

    template< typename E, typename Container > class Kmers;

    template< typename E, typename Container >
    class KmerIterator : public std::iterator< std::random_access_iterator_tag, Container >{

        public:
            KmerIterator( const Kmers<E,Container>* kmers = NULL, std::size_t i = 0 ) : kmers_( kmers ), i_( i ) { }
            KmerIterator( const KmerIterator<E,Container>& cpy ) : kmers_( cpy.kmers_ ), i_( cpy.i_ ) { }
            KmerIterator<E,Container>& operator++ ( ){
                ++i_;
                return *this;
            }
            KmerIterator<E,Container> operator++ ( int ){
                KmerIterator<E,Container> tmp( *this );
                operator++();
                return tmp;
            }
            KmerIterator<E,Container>& operator-- ( ){
                --i_;
                return *this;
            }
            KmerIterator<E,Container> operator-- ( int ){
                KmerIterator<E,Container> tmp( *this );
                operator--();
                return tmp;
            }
            template< typename I >
            typename std::enable_if<std::is_integral<I>::value,KmerIterator<E,Container>&>::type operator+= ( I n ){
                i_ += n;
                return *this;
            }
            template< typename I >
            typename std::enable_if<std::is_integral<I>::value,KmerIterator<E,Container>&>::type operator-= ( I n ){
                i_ -= n;
                return *this;
            }
            template< typename I >
            friend typename std::enable_if<std::is_integral<I>::value,KmerIterator<E,Container>>::type operator+( const KmerIterator<E,Container>& lhs, I rhs ){
                KmerIterator<E,Container> it( lhs );
                it += rhs;
                return it;
            }
            template< typename I >
            friend typename std::enable_if<std::is_integral<I>::value,KmerIterator<E,Container>>::type operator+( I lhs, const KmerIterator<E,Container>& rhs ){
                return rhs + lhs;
            }
            template< typename I >
            friend typename std::enable_if<std::is_integral<I>::value,KmerIterator<E,Container>>::type operator-( const KmerIterator<E,Container>& lhs, I rhs ){
                KmerIterator<E,Container> it( lhs );
                it -= rhs;
                return it;
            }
            friend KmerIterator<E,Container> operator-( const KmerIterator<E,Container>& lhs, const KmerIterator<E,Container>& rhs ){
                KmerIterator<E,Container> it( lhs );
                it -= rhs.i_;
                return it;
            }
            bool operator==( const KmerIterator<E,Container>& rhs ) const{
                return (*kmers_) == (*(rhs.kmers_)) && i_ == rhs.i_;
            }
            bool operator!=( const KmerIterator<E,Container>& rhs ) const{
                return !operator==( rhs );
            }
            bool operator<( const KmerIterator<E,Container>& rhs ) const{
                return i_ < rhs.i_;
            }
            bool operator>( const KmerIterator<E,Container>& rhs ) const{
                return i_ > rhs.i_;
            }
            bool operator<=( const KmerIterator<E,Container>& rhs ) const{
                return i_ <= rhs.i_;
            }
            bool operator>=( const KmerIterator<E,Container>& rhs ) const{
                return i_ >= rhs.i_;
            }
            Container operator*() const{
                return (*kmers_)[ i_ ];
            }
            Container operator[] ( std::size_t i ) const{
                return (*kmers_)[ i_ + i ];
            }

        private:
            Kmers<E,Container>* kmers_;
            std::size_t i_;

    }; //class KmerIterator<E,Container>

    template< typename E, typename Container = std::vector<E> >
    class Kmers : public containers::ContainerTypes< Container,
        KmerIterator< E, Container >, const KmerIterator< E, Container > >{

        public:
            typedef Container container_type;

            Kmers( ) : k_( 0 ) { };
    
            template< typename Iter >
            Kmers( Iter begin, Iter end, std::size_t k ) : k_( k ){
                alphabet( begin, end );
            }

            template< typename Alph >
            Kmers( Alph a, std::size_t k ) : Kmers( a.begin(), a.end(), k ){ }

            std::size_t size( ) const{
                return math::pow( alphabet_.size(), k_ );
            }

            const std::vector<E>& alphabet( ) const{
                return alphabet_;
            }

            std::size_t k( ) const{
                return k_;
            }

            void alphabet( const std::vector<E>& alph ){
                alphabet_ = alph;
            }

            template< typename Iter >
            void alphabet( Iter begin, Iter end ){
                alphabet_.clear();
                for( Iter i = begin; i != end ; ++i ){
                    alphabet_.push_back( *i );
                }
            }

            void k( std::size_t k ){
                k_ = k;
            }

            template< typename Iter >
            void get( std::size_t index, Iter begin ) const{
                Iter it = begin;
                for( std::size_t i = 0 ; i < k_ ; ++i ){
                    *it = elementAt( index, i );
                    ++it;
                }
            }

            Container get( std::size_t index ) const{
                Container c;
                c.resize( k_ );
                get( index, c.begin() );
                return c;
            }

            Container operator[] ( std::size_t i ) const{
                return get( i );
            }

            KmerIterator<E,Container> begin() const{
                return KmerIterator<E,Container>( this );
            }

            KmerIterator<E,Container> end() const{
                return KmerIterator<E,Container>( this, size() );
            }

            template< typename Iter >
            std::size_t indexOf( Iter begin ) const{
                Iter it = begin;
                std::size_t kmer_index = 0, alph_index = 0;
                for( std::size_t i = 0 ; i < k_ ; ++i ){
                    alph_index = alphabetIndexOf( *it );
                    kmer_index += math::pow( alphabet_.size(), k_ - i - 1 ) * alph_index;
                    ++it;
                }
                return alph_index;
            }

            template<>
            std::size_t indexOf<Container>( Container c ) const{
                return indexOf( c.begin() );
            }

            bool operator==( const Kmers& rhs ) const{
                return k_ == rhs.k_ && alphabet_ == rhs.alphabet_;
            }

            bool operator!=( const Kmers& rhs ) const{
                return !operator==( rhs );
            }

        protected:
            inline std::size_t alphabetIndexOf( std::size_t kmer_index, std::size_t pos ) const {
                return ( kmer_index / math::pow( alphabet_.size(), k_ - pos - 1 ) ) % alphabet_.size() ;
            }

            std::size_t alphabetIndexOf( const E& element ) const{
                for( std::size_t i = 0 ; i < alphabet_.size() ; ++i ){
                    if( alphabet_[i] == element ) return i;
                }
                throw exceptions::NoSuchElementError( element );
            }

            inline const E& elementAt( std::size_t index, std::size_t pos ) const{
                return alphabet_[ alphabetIndexOf( index, pos ) ];
            }


        private:
            std::vector<E> alphabet_;
            std::size_t k_;

    }; //class Kmers

} //namespace iterators

#endif
