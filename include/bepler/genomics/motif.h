#ifndef INCLUDED_BEPLER_GENOMICS_MOTIF_H
#define INCLUDED_BEPLER_GENOMICS_MOTIF_H

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
            inline std::size_t size( ) const { return size_; }

        protected:
            std::size_t size_;
            Motif() { }
            Motif( std::size_t size ) : size_( size ) { }
            Motif( const Motif& rhs ) : size_( rhs.size_ ) { }

    }; //class Motif

} //namespace genomics

#endif
