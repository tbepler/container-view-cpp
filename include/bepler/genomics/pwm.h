#ifndef INCLUDED_BEPLER_GENOMICS_PWM_H
#define INCLUDED_BEPLER_GENOMICS_PWM_H

#include "bepler/genomics/motif.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <unordered_map>

namespace genomics{

    class PositionWeightMatrix : public Motif{

        typedef std::unordered_map<char,std::size_t> Map;

        public:
            PositionWeightMatrix() : offset_(), scores_( NULL ) { }
            PositionWeightMatrix( const PositionWeightMatrix& rhs );
            PositionWeightMatrix( const std::string& alph, const std::vector<std::vector<double> >& probs ) : PositionWeightMatrix() {
                this->probabilities( alph, probs );
            }
            virtual ~PositionWeightMatrix();
            double probability( char base, std::size_t pos ) const ;
            double loglikelihood( char base, std::size_t pos ) const;

            template< typename Input >
            double score( Input in ) const{
                double s = 0;
                for( std::size_t i = 0 ; i < size_ ; ++i, ++in ){
                    s += loglikelihood( *in, i );
                }
                return s;
            }

            virtual double score( const char* in ) const override{
                return score( in );
            }

            template< typename Input, typename Output >
            void score( Input begin, Input end, Output out ) const{
                std::deque<char> window;
                Input last = begin;
                //load first window
                for( std::size_t i = 0 ; i < size_ ; ++i, ++last ){
                    window.push_back( *last );
                }
                *out = score( window.begin() );
                ++out;
                //score the rest
                while( last != end ){
                    window.pop_front();
                    window.push_back( *last );
                    *out = score( window.begin() );
                    ++last, ++out;
                }
            }

            virtual void scoreAll( const char* begin, const char* end, functional::acceptor_f<double>&& out ) const override;

            template< typename Input >
            inline double operator()( Input in ) const{ return score( in ); }

            template< typename Input, typename Output >
            inline void operator()( Input begin, Input end, Output out ) const{
                return score( begin, end, out );
            }

            std::string alphabet( ) const;
            inline std::size_t alphabetSize( ) const{ return offset_.size(); }
            PositionWeightMatrix& clear( );
            PositionWeightMatrix& probabilities( const std::string& alph, const std::vector< std::vector<double> >& probs );
            friend std::ostream& operator<< ( std::ostream& out, const PositionWeightMatrix& pwm );
            friend std::istream& operator>> ( std::istream& in, PositionWeightMatrix& pwm );
            
        protected:
            Map offset_;
            double* scores_;
            

    }; //class PositionWeightMatrix

    //template double PositionWeightMatrix::score<const char*>( const char* in ) const;

/*
    class ScoreTable{
        
        std::vector<double> scores_;
        std::size_t cols_;
        std::size_t rows_;

        public:
            ScoreTable() { }
            template< typename Iterator >
            ScoreTable( Iterator input, std::size_t rows, std::size_t cols )
                : scores_( rows * cols ), cols_( cols ), rows_( rows )
            {
                for( std::size_t i = 0 ; i < scores_.size() ; ++i, ++input ){
                    scores_[i] = *input;
                }
            }
            
            inline double score( unsigned offset, unsigned pos ) const{
                return scores_[ pos*cols_ + offset ];
            }

            inline double operator()( unsigned offset, unsigned pos ) const{
                return score( offset, pos );
            }

            friend std::istream& operator>>( std::istream& in, ScoreTable& rhs ){
                rhs.scores_.clear();
                rhs.cols_ = 0;
                rhs.rows_ = 0;
                std::string line;
                while( std::getline( in, line ) && !line.empty() ){
                    std::stringstream ss( line );
                    if( rhs.cols_ == 0 ){
                        double s;
                        while( ss ){
                            ss >> s;
                            rhs.scores_.push_back( s );
                            ++rhs.cols_;
                        }
                    }else{
                        double s;
                        for( std::size_t i = 0 ; i < rhs.cols_ ; ++i ){
                            ss >> s;
                            rhs.scores_.push_back( s );
                        }
                    }
                    ++rhs.rows_;
                }
            }
            
    };

    class PositionWeightMatrix_new{
    
        using size_t = unsigned;
        using encoder_t = void*; //TODO
        using score_table_t = void*; //TODO
        using kmer_enum_t = void*; //TODO

        size_t k_;
        encoder_t encoder_;
        score_table_t score_table_;
        kmer_enum_t kmer_enum_;

        public:
            //constructors -- TODO

            inline double score( unsigned encoded_base, size_t pos ) const{
                //return score_table_( encoded_base, pos );
                return 0;
            }

            template< typename InputIterator >
            double score( InputIterator begin ) const{
                if( kmer_enum_ ){
                    //return kmer_enum_.score( begin );
                }
                double s = 0;
                for( size_t i = 0 ; i < k_ ; ++i, ++begin ){
                    s += score( *begin, i );
                }
                return s;
            }

            template< typename InputIterator, typename OutputIterator >
            void score( InputIterator begin, InputIterator end, OutputIterator out ) const{
                if( kmer_enum_ ){

                }
            }
            


    };
*/
    typedef PositionWeightMatrix PWM;

} //namespace genomics

#endif
