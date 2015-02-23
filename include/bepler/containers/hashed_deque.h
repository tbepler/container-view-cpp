#ifndef INCLUDED_BEPLER_CONTAINERS_HASHED_DEQUE_H
#define INCLUDED_BEPLER_CONTAINERS_HASHED_DEQUE_H

#include "bepler/math/math.h"
#include <deque>
#include <functional>

namespace containers{

    template< typename T, typename Allocator = std::allocator<T> >
    class HashedDeque : public std::deque< T, Allocator >{

        friend class std::hash< HashedDeque >;
        using super_t = typename std::deque< T, Allocator >;

        const static std::size_t prime = 31;

        std::size_t hash_;

        inline void hash_last(){
            hash_ *= prime;
            hash_ += std::hash<T>()( this->back() );
        }

        inline void hash_front(){
            hash_ += std::hash<T>()( this->front() ) * math::pow( prime, this->size() - 1 );
        }

        inline void unhash_last(){
            if( this->size() > 0 ){
                hash_ -= std::hash<T>()( this->back() );
                hash_ /= prime;
            }
        }

        inline void unhash_front(){
            if( this->size() > 0 ){
                hash_ -= std::hash<T>()( this->front() ) * math::pow( prime, this->size() - 1 );
            }
        }

        public:
            HashedDeque() : super_t(), hash_( 0 ) { }
            
            template< typename Iter >
            HashedDeque( Iter begin, Iter end ) : HashedDeque() {
                for( auto it = begin ; it != end ; ++it ){
                    push_back( *it );
                }
            }

            template< typename Container >
            HashedDeque( const Container& c ) : HashedDeque( std::begin( c ), std::end( c ) ) { }

            HashedDeque( const HashedDeque& other ) : super_t( other ), hash_( other.hash_ ) { }

            HashedDeque( HashedDeque&& other ) : super_t( other ), hash_( other.hash_ ) { }

            template< typename Iter >
            void assign( Iter begin, Iter end ){
                clear();
                for( auto it = begin ; it != end ; ++it ){
                    push_back( *it );
                }
            }
            
            auto clear(){
                hash_ = 0;
                return super_t::clear();
            }

            void push_back( const T& value ){
                super_t::push_back( value );
                hash_last();
            }

            void push_back( T&& value ){
                super_t::push_back( std::forward<T>( value ) );
                hash_last();
            }

            void push_front( const T& value ){
                super_t::push_front( value );
                hash_front();
            }

            void push_front( T&& value ){
                super_t::push_front( std::forward<T>( value ) );
                hash_front();
            }

            void pop_back(){
                unhash_last();
                super_t::pop_back();
            }

            void pop_front(){
                unhash_front();
                super_t::pop_front();
            }

    };

} //namespace containers

namespace std{

    template< typename T, typename A >
    struct hash< containers::HashedDeque< T, A > >{
        inline std::size_t operator()( const containers::HashedDeque< T, A >& c ){
            return c.hash_;
        }
    };

} //namespace std

#endif
