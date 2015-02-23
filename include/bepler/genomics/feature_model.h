#ifndef INCLUDED_BEPLER_GENOMICS_FEATURE_MODEL_H
#define INCLUDED_BEPLER_GENOMICS_FEATURE_MODEL_H

#include "bepler/genomics/motif.h"
#include "bepler/containers/hashed_deque.h"
#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <functional>
#include <iostream>

namespace genomics{

    struct Feature{
        containers::HashedDeque<char> seq;
        std::size_t pos;

        Feature() : seq(), pos( 0 ) { }
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
        using super_t = MotifConcept< FeatureModel >;

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

            using super_t::score;
            using super_t::scoreAll;

            template< typename G >
            double scoreGenerator( G&& g ) const{
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
                            ++f.pos;
                        }
                        if( f.size() == f_size ){
                            s += this->get( f );
                        }
                    }
                } );
                return s;
            }

            template< typename G, typename K >
            void scoreAllGenerator( G&& g, K&& k ) const{
                using namespace functional;
                auto s = [this]( auto&& gen ){
                    return this->score( std::forward<decltype(gen)>( gen ) );
                };
                map( s, window<char>( length(),
                    std::forward<G>( g ) ), std::forward<K>( k ) );
            }

            FeatureModel& clear();
            virtual double score( const char* str ) const override;
            virtual void scoreAll( const char* begin, const char* end, acceptor&& out ) const override;

            virtual std::size_t length() const override{ return len_; }
            virtual bool equals( const Motif& rhs ) const override;
    
    }; //class FeatureModel

} //namespace genomics

#endif
