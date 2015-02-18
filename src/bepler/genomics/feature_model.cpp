#include "bepler/genomics/feature_model.h"

#include <ios>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

namespace genomics{
   
    


    ostream& operator<<( ostream& out, const Feature& f ){
        return out << "[" << f.pos << "]" << f.seq;
    }

    inline invalid_argument featureParseExcept( const string& s ){
        return invalid_argument( "`"+s+"' is not a feature. Syntax is [UINT]STRING" );
    }

    istream& operator>>( istream& in, Feature& f ){
        string s;
        in >> s;
        if( s.empty() ){
            in.setstate( ios::failbit );
            return in;
        }
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
        return in;
    }

    FeatureModel& FeatureModel::clear(){
        size_ = 0;
        scores_.clear();
        sizes_.clear();
        return *this;
    }

    double FeatureModel::get( const Feature& f ) const{
        if( scores_.count( f ) ){
            return scores_.at( f );
        }
        return 0;
    }

    void FeatureModel::put( const Feature& f, double score ){
        scores_[ f ] = score;
        sizes_.insert( f.size() );
        if( f.pos + f.size() > size_ ){
            size_ = f.pos + f.size();
        }
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

    void FeatureModel::scoreAll( const char* begin, const char* end, functional::acceptor_f<double>&& out ) const{
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
        Feature f;
        double s;
        model.clear();
        while( in >> f && in >> s ){
            model.put( f, s );
        }
        return in;
    }

    bool operator==( const FeatureModel& a , const FeatureModel& b ){
        return a.scores_ == b.scores_;
    }

    bool operator!=( const FeatureModel& a, const FeatureModel& b ){
        return a.scores_ != b.scores_;
    }

    

} //namespace genomics

namespace std{
    size_t hash<genomics::Feature>::operator()( const genomics::Feature& f ) const{
        size_t h = 5;
        h = h*31 + f.pos;
        h = h*31 + hash<string>()( f.seq );
        return h;
    }
} //namespace std
