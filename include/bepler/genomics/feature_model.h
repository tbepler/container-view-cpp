#ifndef INCLUDED_BEPLER_GENOMICS_FEATURE_MODEL_H
#define INCLUDED_BEPLER_GENOMICS_FEATURE_MODEL_H

#include "bepler/genomics/motif.h"
#include <string>
#include <deque>
#include <unordered_map>
#include <set>
#include <functional>
#include <iostream>

namespace genomics{

    struct Feature{
        std::deque<char> seq;
        std::size_t pos;

        Feature() { }
        Feature( const std::string& s, std::size_t p )
            : seq( s.begin(), s.end() ), pos( p ) { }

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

    class FeatureModel : public MotifConcept< FeatureModel >{
        
        using FeatureMap = std::unordered_map< Feature, double >;

        const static std::size_t sindex = 0;

        FeatureMap scores_;
        std::set< std::size_t > sizes_;
        std::size_t len_;

        double get( const Feature& f ) const;
        void put( const Feature& f, double score );

        protected:
            virtual void read( std::istream& in ) override;
            virtual void write( std::ostream& out ) const override;

        public:
            FeatureModel() : scores_(), sizes_(), len_( 0 ) { }

            FeatureModel( const FeatureModel& rhs ) : scores_( rhs.scores_ ), sizes_( rhs.sizes_ ), len_( rhs.len_ ) { }
            
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

            template< typename G >
            double score( G&& g ) const{
                double s = 0;
                std::vector<Feature> features( sizes_.size() );
                g( [&]( char c ){
                    auto size_it = this->sizes_.begin();
                    auto feat_it = features.begin();
                    for( ; size_it != this->sizes_.end() && feat_it != features.end() ; ++size_it, ++feat_it ){
                        Feature& f = *feat_it;
                        std::size_t f_size = *size_it;
                        f.seq.push_back( c );
                        while( f.size() > f_size ){
                            f.seq.pop_front();
                        }
                        if( f.size() == f_size ){
                            s += this->get( f );
                        }
                    }
                } );
                return s;
            }

            template< typename G, typename K >
            void scoreAll( G&& g, K&& k ){
                using namespace functional;
                auto s = [this]( auto&& gen ){
                    return this->score( std::forward<decltype(gen)>( gen ) );
                }
                map( s, window<char>( length(),
                    std::forward<G>( g ) ), std::forward<K>( k ) );
            }

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
