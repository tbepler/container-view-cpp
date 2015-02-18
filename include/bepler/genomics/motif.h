#ifndef INCLUDED_BEPLER_GENOMICS_MOTIF_H
#define INCLUDED_BEPLER_GENOMICS_MOTIF_H

#include "bepler/functional/generator.h"
#include <cstdlib>
#include <string>

namespace genomics{

    class Motif{
        
        public:
            virtual ~Motif() { };
            virtual double score( const char* str ) const = 0;
            inline double score( const std::string& str ) const{
                return score( str.c_str() );
            }
            virtual void scoreAll( const char* begin, const char* end, functional::acceptor_f< double >& out ) const = 0;
            inline void scoreAll( const std::string& str, functional::acceptor_f< double >& out ) const{
                scoreAll( str.c_str(), str.c_str() + str.size(), out );
            }
            inline std::size_t size( ) const { return size_; }

        protected:
            std::size_t size_;
            Motif() { }
            Motif( std::size_t size ) : size_( size ) { }
            Motif( const Motif& rhs ) : size_( rhs.size_ ) { }

    }; //class Motif

} //namespace genomics

#endif
