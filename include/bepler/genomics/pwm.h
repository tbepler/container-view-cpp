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

    typedef PositionWeightMatrix PWM;

} //namespace genomics

#endif
