#include "bepler/genomics/feature_model.h"

#include <ios>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

namespace genomics{
   
    
    inline ostream& operator<<( ostream& out, const deque<char>& seq ){
        for( char c : seq ){
            out << c;
        }
        return out;
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
        string sub = s.substr( pos_end + 1 );
        f.seq.assign( sub.begin(), sub.end() );
        return in;
    }

    FeatureModel& FeatureModel::clear(){
        len_ = 0;
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
        if( f.pos + f.size() > len_ ){
            len_ = f.pos + f.size();
        }
    }

    double FeatureModel::score( const char* str ) const{
        double s = 0;
        Feature f;
        for( size_t f_len : sizes_ ){
            f.pos = 0;
            f.seq.clear();
            for( size_t i = 0 ; i < len_ ; ++i ){
                f.seq.push_back( str[i] );
                while( f.seq.size() > f_len ){
                    f.seq.pop_front();
                    ++f.pos;
                }
                if( f.seq.size() == f_len ){
                    s += get( f );
                }
            }
        }
        return s;
    }

    void FeatureModel::scoreAll( const char* begin, const char* end, acceptor&& out ) const{
        for( const char* pos = begin ; pos < end - len_ + 1 ; ++pos ){
            out( score( pos ) );
        }
    }

    bool FeatureModel::equals( const Motif& m ) const{
        if( const FeatureModel* rhs = dynamic_cast< const FeatureModel* >( &m ) ){
            return scores_ == rhs->scores_;
        }
        return false;
    }

    void FeatureModel::write( ostream& out ) const{
        for( auto it = scores_.begin() ; it != scores_.end() ; ++it ){
            out << it->first << " " << it->second << endl;
        }
    }

    void FeatureModel::read( istream& in ){
        Feature f;
        double s;
        clear();
        while( in >> f && in >> s ){
            put( f, s );
        }
    }
    

} //namespace genomics

namespace std{
    size_t hash<genomics::Feature>::operator()( const genomics::Feature& f ) const{
        return 151*f.pos + hash< containers::HashedDeque<char> >()( f.seq );
    }
} //namespace std
