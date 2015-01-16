#ifndef INCLUDE_PERMUTATION_H_
#define INCLUDE_PERMUTATION_H_

#include <cstddef>

/**
* This class provides convenient mapping between indices and permutations for some
* specified alphabet size and permutation length
*
* @author Tristan Bepler
**/

class Permutation{

    public:
        Permutation( ) : m_alphabet( 0 ), m_len( 0 ) { }
        Permutation( std::size_t alphabet, std::size_t len )
            : m_alphabet( alphabet ), m_len( len ) { }

        ~Permutation( ) { }

        std::size_t length( ) const { return m_len; }
        void length( std::size_t len ) { m_len = len; }

        std::size_t alphabet( ) const { return m_alphabet; }
        void alphabet( std::size_t alph ) { m_alphabet = alph; }

        std::size_t size( ) const { return pow( m_alphabet, m_len ); }
 
        template< typename Container >
        void permutation( std::size_t index, Container& c ) const{
            for( std::size_t i = 0 ; i < m_len ; ++i ){
                c[ m_len - i - 1 ] = ( index / pow( m_alphabet, i ) ) % m_alphabet;
            }
        }

        template< typename Container >
        Container permutation( std::size_t index ) const{
            Container c( m_len );
            permutation( index, c );
            return c;
        }

        template< typename Container >
        std::size_t indexOf( const Container& permutation ) const{
            std::size_t idx = 0, entry = 0;
            for( std::size_t i = 0 ; i < m_len ; ++i ){
                entry = permutation[ m_len - i - 1 ];
                idx += pow( m_alphabet, i ) * entry;
            }
            return idx;
        }

    private:
        std::size_t m_len;
        std::size_t m_alphabet;

        static std::size_t pow( std::size_t base, std::size_t exp ){
            std::size_t result = 1;
            while( exp ){
                if( exp & 1 ){
                    result *= base;
                }
                exp >>= 1;
                base *= base;
            }
            return result;
        }
};


#endif
