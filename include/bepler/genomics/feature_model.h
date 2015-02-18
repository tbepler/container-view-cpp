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

    std::ostream& operator<<( std::ostream& out, const Feature& f );
    std::istream& operator>>( std::istream& in, Feature& f );

} //namespace genomics

namespace std{
    template<>
    struct hash<genomics::Feature>{
        size_t operator()( const genomics::Feature& f ) const;
    };
} //namespace std

namespace genomics{

    class FeatureModel : public Motif{
        
        using FeatureMap = std::unordered_map< Feature, double >;

        FeatureMap scores_;
        std::unordered_set< std::size_t > sizes_;

        double get( const Feature& f ) const;
        void put( const Feature& f, double score );

        public:
            FeatureModel() : Motif() { }

            FeatureModel( const FeatureModel& rhs ) : Motif( rhs ), scores_( rhs.scores_ ), sizes_( rhs.sizes_ ) { }
            
            template< typename IFeatures, typename IScores >
            FeatureModel( IFeatures f_begin, IFeatures f_end, IScores s_begin, IScores s_end ) : FeatureModel() {
                assign( f_begin, f_end, s_begin, s_end );
            }

            template< typename IFeatures, typename IScores >
            FeatureModel& assign( IFeatures f_begin, IFeatures f_end, IScores s_begin, IScores s_end ){
                clear();
                auto f_it = f_begin; auto s_it = s_begin;
                for( ; f_it != f_end && s_it != s_end ; ++f_it, ++s_it ){
                    put( *f_it, *s_it );
                }
                return *this;
            }

            using Motif::score;
            using Motif::scoreAll;

            FeatureModel& clear();
            virtual double score( const char* str ) const;
            virtual void scoreAll( const char* begin, const char* end, functional::acceptor_f<double>&& out ) const;
            friend std::ostream& operator<<( std::ostream& out, const FeatureModel& model );
            friend std::istream& operator>>( std::istream& in, FeatureModel& model );
            friend bool operator==( const FeatureModel& a, const FeatureModel& b );
            friend bool operator!=( const FeatureModel& a, const FeatureModel& b );

    }; //class FeatureModel

} //namespace genomics

#endif
