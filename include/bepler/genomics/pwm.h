#ifndef INCLUDED_BEPLER_GENOMICS_PWM_H
#define INCLUDED_BEPLER_GENOMICS_PWM_H

#include "bepler/genomics/motif.h"
#include <iostream>
#include <vector>
#include <unordered_map>

namespace genomics{

    class PositionWeightMatrix : public Motif{

        typedef std::unordered_map<char,std::size_t> Map;

        public:
            PositionWeightMatrix( const PositionWeightMatrix& rhs );
            virtual ~PositionWeightMatrix();
            virtual double score( const char* str ) const;
            double probability( char base, std::size_t pos ) const ;
            double loglikelihood( char base, std::size_t pos ) const;
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

    typedef PositionWeightMatrix PWM;

} //namespace genomics

#endif
