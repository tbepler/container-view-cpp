#ifndef INCLUDED_BEPLER_GENOMICS_DNA_ENCODER_H
#define INCLUDED_BEPLER_GENOMICS_DNA_ENCODER_H

#include <stdexcept>

namespace genomics{

    namespace DNA{

        const static char[] BASES = "ACGT";
        enum DNA { A, C, G, T };

        DNA encode( char base ){
            switch( base ){
                case 'A': return A;
                case 'C': return C;
                case 'G': return G;
                case 'T': return T;
                case 'a': return A;
                case 'c': return C;
                case 'g': return G;
                case 't': return T;
            }
            throw std::out_of_range( "No such DNA base: " + base );
        }

        inline char decode( unsigned encoded_base ){
            return BASES[ encoded_base ];
        }

        template< typename InputIterator, typename OutputIterator >
        void encode( InputIterator begin, InputIterator end, OutputIterator out ){
            for( auto it = begin ; it != end ; ++it, ++out ){
                *out = encode( *it );
            }
        }
        
        template< typename InputIterator, typename OutputIterator >
        void decode( InputIterator begin, InputIterator end, OutputIterator out ){
            for( auto it = begin ; it != end ; ++it, ++out ){
                *out = decode( *it );
            }
        }

    } //namespace DNA

    struct DNAEncoder{

        inline DNA::DNA encode( char base ){
            return DNA::encode( base );
        }

        template< typename Integer >
        char decode( Integer base ){
            return DNA::BASES[base];
        }

    };

} //namespace genomics

#endif
