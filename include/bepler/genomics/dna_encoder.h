#ifndef INCLUDED_BEPLER_GENOMICS_DNA_ENCODER_H
#define INCLUDED_BEPLER_GENOMICS_DNA_ENCODER_H

#include <iterator>
#include <iostream>
#include <stdexcept>

namespace genomics{

    namespace DNA{

        const static char BASES[] = "ACGT";
        enum BASE { A, C, G, T };

        bool isDNA( char base ){
            switch( base ){
                case 'A': return true;
                case 'C': return true;
                case 'G': return true;
                case 'T': return true;
                case 'a': return true;
                case 'c': return true;
                case 'g': return true;
                case 't': return true;
            }
            return false;
        }

        template< typename InputIterator >
        bool isDNA( InputIterator begin, InputIterator end ){
            for( auto it = begin ; it != end ; ++it ){
                if( !isDNA( *it ) ) return false;
            }
            return true;
        }

        template< typename Range >
        inline bool isDNA( Range& r ){
            return isDNA( std::begin( r ), std::end( r ) );
        }

        DNA encode( char base ){
            switch( base ){
                //case A: return A;
                //case C: return C;
                //case G: return G;
                //case T: return T;
                //convert from char to DNA enum
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

        std::istream& operator>>( std::istream& in, DNA& base ){
            char c;
            in >> c;
            base = encode( c );
            return in;
        }

        std::ostream& operator<<( std::ostream& out, DNA base ){
            return out << decode( base );
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
