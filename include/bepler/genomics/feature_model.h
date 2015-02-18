#ifndef INCLUDED_BEPLER_GENOMICS_FEATURE_MODEL_H
#define INCLUDED_BEPLER_GENOMICS_FEATURE_MODEL_H

#include "bepler/genomics/motif.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
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

        inline std::size_t size() const{ return seq.size(); }
    };

    inline bool operator==( const Feature& a, const Feature& b ){
        return a.pos == b.pos && a.seq == b.seq;
    }

    inline bool operator!=( const Feature& a, const Feature& b ){
        return a.pos != b.pos || a.seq != b.seq;
    }

    inline bool operator<( const Feature& a, const Feature& b ){
        return a.pos < b.pos || ( a.pos == b.pos && a.seq < b.seq );
    }
    
    inline bool operator<=( const Feature& a, const Feature& b ){
        return a.pos < b.pos || ( a.pos == b.pos && a.seq <= b.seq );
    }

    inline bool operator>( const Feature& a, const Feature& b ){
        return a.pos > b.pos || ( a.pos == b.pos && a.seq > b.seq );
    }
    
    inline bool operator>=( const Feature& a, const Feature& b ){
        return a.pos > b.pos || ( a.pos == b.pos && a.seq >= b.seq );
    }

    template<>
    struct std::hash<Feature>{
        std::size_t operator()( const Feature& f ) const;
    };

    std::ostream& operator<<( std::ostream& out, const Feature& f );
    std::istream& operator>>( std::istream& in, Feature& f );

    class FeatureModel : public Motif{
        
        using FeatureMap = std::unordered_map< Feature, double >;

        FeatureMap scores_;
        std::unordered_set< std::size_t > sizes_;

        double get( const Feature& f ) const;

        public:
            FeatureModel() { }

            FeatureModel( const FeatureModel& rhs ) : Motif( rhs ), scores_( rhs.scores_ ), sizes_( rhs.sizes_ ) { }
            
            template< typename IFeatures, typename IScores >
            FeatureModel( IFeatures f_begin, IFeatures f_end, IScores s_begin, IScores s_end ) {
                assign( f_begin, f_end, s_begin, s_end );
            }

            template< typename IFeatures, typename IScores >
            FeatureModel& assign( IFeatures f_begin, IFeatures f_end, IScores s_begin, IScores s_end ){
                clear();
                for( IFeatures f_it = f_begin, IScores s_it = s_begin ; f_it != f_end && s_it != s_end ; ++f_it, ++s_it ){
                    scores_[ *f_it ] = *s_it;
                    sizes_.insert( f_it->size() );
                    if( f_it->pos + f_it->size() > size_ ){
                        size_ = f_it->pos + f_it->size();
                    }
                }
                return *this;
            }

            FeatureModel& clear();
            virtual double score( const char* str ) const;
            virtual void scoreAll( const char* begin, const char* end, functional::acceptor_f<double>& out ) const;
            friend std::ostream& operator<<( std::ostream& out, const FeatureModel& model );
            friend std::istream& operator>>( std::istream& in, FeatureModel& model );

    }; //class FeatureModel

} //namespace genomics

#endif
