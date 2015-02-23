#ifndef INCLUDED_BEPLER_GENOMICS_MOTIF_H
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
        inline double score( std::string&& str ) const{
            return score( str.c_str() );
        }
        inline double operator()( const char* str ) const{
            return score( str );
        }
        inline double operator()( const std::string& str ) const{
            return score( str );
        }
        inline double operator()( std::string&& str ) const{
            return score( std::forward<std::string>( str ) );
        }

        virtual void scoreAll( const char* begin, const char* end, acceptor&& out ) const = 0;
        inline void scoreAll( const std::string& str, acceptor&& out ) const{
            scoreAll( str.c_str(), str.c_str() + str.size(),
                std::forward< acceptor >( out ) );
        }
        inline void scoreAll( std::string&& str, acceptor&& out ) const{
            scoreAll( str.c_str(), str.c_str() + str.size(),
                std::forward< acceptor >( out ) );
        }
        inline void operator()( const char* begin, const char* end, acceptor&& out ){
            scoreAll( begin, end, std::forward<acceptor>( out ) );
        }
        inline void operator()( const std::string& str, acceptor&& out ) const{
            scoreAll( str, std::forward<acceptor>( out ) );
        }
        inline void operator()( std::string&& str, acceptor&& out ) const{
            scoreAll( std::forward<std::string>( str ), std::forward<acceptor>( out ) );
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

        template< typename G, typename T >
        using enable = typename std::enable_if<
            !std::is_same< typename std::decay<G>::type, std::string >::value
            && !std::is_same< typename std::decay<G>::type, char* >::value,
            T
        >::type;

        using Motif::score;
        using Motif::scoreAll;
        using Motif::operator();

        template< typename G >
        inline enable<G,double> score( G&& g ) const{
            return derived().scoreGenerator( std::forward<G>( g ) );
        }
        virtual double score( const char* str ) const override{
            using namespace functional;
            return score( curry( irange, str, str + length() ) );
        }

        template< typename G >
        inline auto operator()( G&& g ) const{
            return score( std::forward<G>( g ) );
        }

        template< typename G, typename K >
        inline enable<G,void> scoreAll( G&& g, K&& k ) const{
            derived().scoreAllGenerator( std::forward<G>( g ), std::forward<K>( k ) );
        }
        virtual void scoreAll( const char* begin, const char* end, acceptor&& k ) const override{
            using namespace functional;
            scoreAll(
                curry( irange, begin, end ),
                std::forward< acceptor >( k )
            );
        }

        template< typename G, typename K >
        inline auto operator()( G&& g, K&& k ) const{
            return scoreAll( std::forward<G>( g ), std::forward<K>( k ) );
        }

        protected:
            inline Derived& derived(){ return static_cast<Derived>(*this); }
            inline const Derived& derived() const{ return *static_cast<const Derived*>(this); }

    }; //struct MotifConcept

} //namespace genomics

#endif
