#ifndef INCLUDED_BEPLER_GENOMICS_FEATURE_MODEL_H
#define INCLUDED_BEPLER_GENOMICS_FEATURE_MODEL_H

#include <string>
#include <functional>
#include <iostream>

namespace genomics{

    struct Feature{
        std::string seq;
        std::size_t pos;

        Feature() { }
        Feature( const std::string& s, std::size_t p )
            : seq( s ), pos( p ) { }
        Feature( std::string&& s, std::size_t p )
            : seq( std::forward<std::string>( s ) ), pos( p ) { }
    };

    bool operator==( const Feature& a, const Feature& b ){
        return a.pos == b.pos && a.seq == b.seq;
    }

    bool operator!=( const Feature& a, const Feature& b ){
        return a.pos != b.pos || a.seq != b.seq;
    }

    bool operator<( const Feature& a, const Feature& b ){
        return a.pos < b.pos || ( a.pos == b.pos && a.seq < b.seq );
    }
    
    bool operator<=( const Feature& a, const Feature& b ){
        return a.pos < b.pos || ( a.pos == b.pos && a.seq <= b.seq );
    }

    bool operator>( const Feature& a, const Feature& b ){
        return a.pos > b.pos || ( a.pos == b.pos && a.seq > b.seq );
    }
    
    bool operator>=( const Feature& a, const Feature& b ){
        return a.pos > b.pos || ( a.pos == b.pos && a.seq >= b.seq );
    }

    template<>
    struct std::hash<Feature>{
        std::size_t operator()( const Feature& f ) const{
            std::size_t hash = 5;
            hash = hash*31 + pos;
            hash = hash*31 + std::hash<std::string>()( seq );
            return hash;
        }
    };

    std::ostream& operator<<( std::ostream& out, const Feature& f ){
        return out << "[" << f.pos << "]" << f.seq;
    }

    std::istream& operator>>( std::istream& in, Feature& f ){
        
    }

    class FeatureModel{
        
        public:
            
            template< typename G, typename K >
            void score( G&& g, K&& k ) const{
                
            }

    }; //class FeatureModel

} //namespace genomics

#endif
