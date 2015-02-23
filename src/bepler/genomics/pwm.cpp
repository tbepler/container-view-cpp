#include "bepler/genomics/pwm.h"

#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <cstring>

using namespace std;

namespace genomics{

    PositionWeightMatrix::PositionWeightMatrix( const PositionWeightMatrix& rhs ) : size_( rhs.size_ ){
        
        size_t elems = rhs.size_ * rhs.offset_.size() ;
        double* scores_copy = new double[ elems ];
        memcpy( scores_copy, rhs.scores_, elems * sizeof( double ) );

        if( scores_ ){
            delete scores_;
        }
        scores_ = scores_copy;
        offset_ = rhs.offset_;

    }

    PositionWeightMatrix::~PositionWeightMatrix( ){
        if( scores_ ){
            delete scores_;
            scores_ = NULL;
        }
    }

/*
    double PositionWeightMatrix::score( const char* str ) const{
        double s = 0;
        for( size_t i = 0 ; i < size_ ; ++i ){
            s += loglikelihood( str[i], i );
        }
        return s;
    }
*/

    void PositionWeightMatrix::scoreAll( const char* begin, const char* end, acceptor&& out ) const{
        for( const char* pos = begin ; pos < end - size_ + 1 ; ++pos ){
            out( score( pos ) );
        }
    }

    std::size_t PositionWeightMatrix::length( ) const{
        return size_;
    }

    bool PositionWeightMatrix::equals( const Motif& m ) const{
        if( const PositionWeightMatrix* rhs
            = dynamic_cast< const PositionWeightMatrix* >( &m ) ){
            if( length() == rhs->length() && alphabet() == rhs->alphabet() ){
                for( std::size_t i = 0 ; i < length() ; ++i ){
                    for( char c : alphabet() ){
                        if( loglikelihood( c, i ) != rhs->loglikelihood( c, i ) ){
                            return false;
                        }
                    }
                }
                return true;
            }
        }
        return false;
    }

    double PositionWeightMatrix::probability( char base, size_t pos ) const{
        return exp( loglikelihood( base, pos ) );
    }

    double PositionWeightMatrix::loglikelihood( char base, size_t pos ) const{
        size_t char_offset = offset_.at( base );
        return scores_[ pos * alphabetSize() + char_offset ];
    }

    string PositionWeightMatrix::alphabet( ) const{
        string alph;
        for( auto it = offset_.begin() ; it != offset_.end() ; ++it ){
            alph += it->first;
        }
        sort( alph.begin(), alph.end() );
        return alph;
    }

    PositionWeightMatrix& PositionWeightMatrix::clear( ){
        size_ = 0;
        offset_.clear();
        if( scores_ ){
            delete scores_;
            scores_ = NULL;
        }
        return *this;
    }

    PositionWeightMatrix& PositionWeightMatrix::probabilities( const string& alph, const vector< vector<double> >& probs ){

        //validate the input
        size_t alph_size = alph.size();
        size_t len = probs.size();
        for( size_t i = 0 ; i < len ; ++i ){
            if( probs[i].size() != alph_size ){
                stringstream err;
                err << "Rows must be same size as alphabet but aren't. Alphabet: " << alph_size << ", row[" << i << "]: " << probs[i].size() ;
                throw invalid_argument( err.str() );
            }
        }

        //log transform probs and write to scores array
        size_t elems = len * alph_size ;
        double* scores = new double[ elems ];
        for( size_t i = 0 ; i < len ; ++i ){
            for( size_t j = 0 ; j < alph_size ; ++j ){
                scores[ i * alph_size + j ] = log( probs[i][j] );
            }
        }

        //assign member variables
        if( scores_ ){
            delete scores_;
        }
        scores_ = scores;
        offset_.clear();
        for( size_t i = 0 ; i < alph.size() ; ++i ){
            offset_[ alph[i] ] = i;
        }
        size_ = probs.size();

        return (*this);

    }

    void PositionWeightMatrix::write ( ostream& out ) const{
        //write PWM out in column form
        string alph = alphabet();
        //first write alphabet
        for( size_t i = 0 ; i < alph.size() ; ++i ){
            if( i != 0 ) out << " ";
            out << alph[i];
        }
        //now write positions in alphabet order
        for( size_t i = 0 ; i < length() ; ++i ){
            out << endl;
            for( size_t j = 0 ; j < alph.size() ; ++j ){
                if( j != 0 ) out << " ";
                out << probability( alph[j], i );
            }
        }
    }

    void PositionWeightMatrix::read ( istream& in ){
        //read PWM from column form probabilities
        string line;
        //read to first non empty line
        while( getline( in, line ) && line.empty() );

        if( !line.empty() ){
            //first line is alphabet
            string alph;
            string token;
            stringstream ss( line );
            while( !ss.eof() ){
                ss >> token;
                alph += token[0];
            }
            //now read probabilities
            vector< vector<double> > probs;
            while( getline( in, line ) && !line.empty() ){
                ss.str( line );
                ss.clear();
                vector<double> row;
                double d;
                for( size_t i = 0 ; i < alph.size() ; ++i ){
                    ss >> d;
                    row.push_back( d );
                }
                probs.push_back( row );
            }
            this->probabilities( alph, probs );
        }else{
            this->clear();
        }

    }


} //namespace genomics
