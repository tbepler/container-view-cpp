#include "bepler/genomics/feature_model.h"

#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

namespace genomics{
   
    

    size_t hash<Feature>::operator()( const Feature& f ) const{
        size_t hash = 5;
        hash = hash*31 + pos;
        hash = hash*31 + hash<string>()( seq );
        return hash;
    }

    ostream& operator<<( ostream& out, const Feature& f ){
        return out << "[" << f.pos << "]" << f.seq;
    }

    inline invalid_argument featureParseExcept( const string& s ){
        return invalid_argument( "`"+s+"' is not a feature. Syntax is [UINT]STRING" );
    }

    istream& operator>>( istream& in, Feature& f ){
        string s;
        in >> s;
        size_t pos_begin = s.find( '[' );
        if( pos_begin == string::npos ){
            throw featureParseExcept( s );
        }
        ++pos_begin;
        size_t pos_end = s.find( ']' );
        if( pos_end == string::npos || pos_end == pos_begin ){
            throw featureParseExcept( s );
        }
        f.pos = stoul( s.substr( pos_begin, pos_end - pos_begin ) );
        f.seq = s.substr( pos_end + 1 );
    }

    FeatureModel& FeatureModel::clear(){
        size_ = 0;
        scores_.clear();
        sizes_.clear();
        return *this;
    }

    inline double FeatureModel::get( const Feature& f ) const{
        if( scores_.count( f ) ){
            return scores_.at( f );
        }
        return 0;
    }

    double FeatureModel::score( const char* str ) const{
        double s = 0;
        Feature f;
        for( size_t f_len : sizes_ ){
            for( size_t pos = 0 ; pos < size_ - f_len + 1 ; ++pos ){
                f.pos = pos;
                f.seq.assign( str + pos, f_len );
                s += get( f );
            }
        }
        return s;
    }

    void FeatureModel::scoreAll( const char* begin, const char* end, functional::acceptor_f<double>& out ) const{
        for( const char* pos = begin ; pos < end - size_ + 1 ; ++pos ){
            out( score( pos ) );
        }
    }

    ostream& operator<<( ostream& out, const FeatureModel& model ){
        for( auto it = model.scores_.begin() ; it != model.scores_.end() ; ++it ){
            out << it->first << " " << it->second << endl;
        }
        return out;
    }

    istream& operator>>( istream& in, FeatureModel& model ){
        vector<Feature> features;
        vector<double> scores;
        Feature f;
        double s;
        while( !in.eof() ){
            in >> f;
            in >> s;
            features.push_back( f );
            scores.push_back( s );
        }
        model.assign( features.begin(), features.end(), scores.begin(), scores.end() );
        return in;
    }


    

} //namespace genomics
