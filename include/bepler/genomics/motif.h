uifndef INCLUDED_BEPLER_GENOMICS_MOTIF_H
#define INCLUDED_BEPLER_GENOMICS_MOTIF_H

#include "bepler/functional/generator.h"
#include "bepler/functional/continuation.h"
#include <cstdlib>
#include <string>

namespace genomics{

    template< typename Impl >
    struct Motif : public Impl{

        using Impl::Impl;

        template< typename G >
        inline auto score( G&& g ) const{
            return Impl::score( std::forward<G>( g ) );
        }

        template< typename G, typename K >
        inline auto scoreAll( G&& g, K&& k ) const{
            return Impl::scoreAll( std::forward<G>( g ), std::forward<K>( k ) );
        }

        template< typename G >
        inline auto operator( G&& g ) const{
            return score( std::forward<G>( g ) );
        }

        template< typename G, typename K >
        inline auto operator( G&& g, K&& k ) const{
            return scoreAll( std::forward<G>( g ), std::forward<K>( k ) );
        }

        inline std::size_t length() const{
            return Impl::length();
        }

        template< typename T >
        inline auto operator==( const T& rhs ) const
            -> decltype( Impl::equals( rhs ) )
        {
            return Impl::equals( rhs );
        }
        template< typename T >
        friend inline bool operator==( const T& lhs, const Motif& rhs ){
            return rhs.operator==( lhs );
        }
        template< typename T >
        inline auto operator!=( const T& rhs ) const
            -> decltype( !Impl::equals( rhs ) )
        {
            return !Impl::equals( rhs );
        }
        template< typename T >
        friend inline bool operator!=( const T& lhs, const Motif& rhs ){
            return rhs.operator!=( lhs );
        }

        friend inline std::istream& operator>>( std::istream& in, Motif& m ){
            m.read( in ); return in;
        }

        friend inline std::ostream& operator<<( std::ostream& out, const Motif& m ){
            m.write( out ); return out;
        }

    };

    auto motifType( const std::string& tag ){

    }

    template< typename K, typename... Params >
    void parseMotif( const std::string& tag, std::istream& in, K&& k, Params&&... args ){
        
        switch( motifType( tag ) ){
            /*
            case Type:
                Motif<Type> m;
                in >> m;
                k( m, std::forward<Params>( args )... );
            */
        }

    }

    class Motif{
        
        public:
            virtual ~Motif() { };
            virtual double score( const char* str ) const = 0;
            inline double score( const std::string& str ) const{
                return score( str.c_str() );
            }
            virtual void scoreAll( const char* begin, const char* end, functional::acceptor_f< double >&& out ) const = 0;
            inline void scoreAll( const std::string& str, functional::acceptor_f< double >&& out ) const{
                scoreAll( str.c_str(), str.c_str() + str.size(),
                    std::forward< functional::acceptor_f<double> >( out ) );
            }
            inline std::size_t size( ) const { return size_; }

        protected:
            std::size_t size_;
            Motif() : size_( 0 ) { }
            Motif( std::size_t size ) : size_( size ) { }
            Motif( const Motif& rhs ) : size_( rhs.size_ ) { }

    }; //class Motif

} //namespace genomics

#endif
