uifndef INCLUDED_BEPLER_GENOMICS_MOTIF_H
#define INCLUDED_BEPLER_GENOMICS_MOTIF_H

#include "bepler/functional/generator.h"
#include "bepler/functional/continuation.h"
#include <cstdlib>
#include <string>
#include <unordered_map>

namespace genomics{

    struct Motif{
        
        using acceptor = functional::acceptor_f< double >;

        virtual ~Motif() { };
        
        virtual double score( const char* str ) const = 0;
        inline double score( const std::string& str ) const{
            return score( str.c_str() );
        }
        inline double operator()( const char* str ) const{
            return score( str );
        }
        inline double operator()( const std::string& str ) const{
            return score( str );
        }

        virtual void scoreAll( const char* begin, const char* end, acceptor&& out ) const = 0;
        inline void scoreAll( const std::string& str, acceptor&& out ) const{
                scoreAll( str.c_str(), str.c_str() + str.size(),
                    std::forward< acceptor >( out ) );
        }
        inline void operator()( const char* begin, const char* end, acceptor&& out ){
            scoreAll( begin, end, out );
        }
        inline void operator()( const std::string& str, acceptor&& out ) const{
            scoreAll( str, out );
        }

        virtual std::size_t length( ) const = 0;       
        virtual bool equals( const Motif& rhs ) const = 0;

        friend inline std::ostream& operator<<( std::ostream& out, const Motif& m ){
            m.write( out ); return out;
        }

        friend inline std::istream& operator>>( std::istream& in, Motif& m ){
            m.read( in ); return in;     
        }

        protected:
            virtual void read( std::istream& in ) = 0;
            virtual void write( std::ostream& out ) const = 0;

    }; //class Motif

    inline bool operator==( const Motif& a, const Motif& b ){
        return a.equals( b );
    }

    inline bool operator!=( const Motif& a, const Motif& b ){
        return !a.equals( b );
    }
    
    template< typename Derived >
    struct MotifConcept : public Motif{

        using Motif::score;
        using Motif::scoreAll;
        using Motif::operator();

        template< typename G >
        inline auto score( G&& g ) const{
            return derived().score( std::forward<G>( g ) );
        }
        virtual double score( const char* str ) const{
            return score( functional::irange( str, str + length() ) );
        }

        template< typename G >
        inline auto operator( G&& g ) const{
            return score( std::forward<G>( g ) );
        }

        template< typename G, typename K >
        inline auto scoreAll( G&& g, K&& k ) const{
            return derived().scoreAll( std::forward<G>( g ), std::forward<K>( k ) );
        }
        virtual void scoreAll( const char* begin, const char* end, acceptor&& k ){
            return scoreAll( functional::irange( begin, end ), std::forward< acceptor >( k ) );
        }

        template< typename G, typename K >
        inline auto operator( G&& g, K&& k ) const{
            return scoreAll( std::forward<G>( g ), std::forward<K>( k ) );
        }

        protected:
            inline Derived& derived(){ return static_cast<Derived>(*this); }
            inline const Derived& derived() const{ return static_cast<const Derived>(*this); }

    }; //struct MotifConcept

} //namespace genomics

#endif
