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

    class PositionWeightMatrix : public MotifConcept< PositionWeightMatrix >{

        typedef std::unordered_map<char,std::size_t> Map;

        public:
            using Motif::score;
            using Motif::scoreAll;

            PositionWeightMatrix() : offset_(), scores_( NULL ), size_( 0 ) { }
            PositionWeightMatrix( const PositionWeightMatrix& rhs );
            PositionWeightMatrix( const std::string& alph, const std::vector<std::vector<double> >& probs ) : PositionWeightMatrix() {
                this->probabilities( alph, probs );
            }
            virtual ~PositionWeightMatrix();
            double probability( char base, std::size_t pos ) const ;
            double loglikelihood( char base, std::size_t pos ) const;

            template< typename G >
            inline double score( G&& g ) const{
                double s = 0;
                std::size_t i = 0;
                g( [&]( auto c ){
                    s += loglikelihood( c, i++ );
                } );
                return s;
            }

            template< typename G, typename K >
            inline void scoreAll( G&& g, K&& k ) const{
                using namespace functional;
                map( *this ,
                    window<char>( length(), std::forward<G>( g ) ),
                    std::forward<K>( k )
                );
            }

            virtual void scoreAll( const char* begin, const char* end, acceptor&& out ) const override;
            virtual std::size_t length() const override;
            virtual bool equals( const Motif& m ) const override;

            std::string alphabet( ) const;
            inline std::size_t alphabetSize( ) const{ return offset_.size(); }
            PositionWeightMatrix& clear( );
            PositionWeightMatrix& probabilities( const std::string& alph, const std::vector< std::vector<double> >& probs );
            
        protected:
            Map offset_;
            double* scores_;
            std::size_t size_;

            virtual void read( std::istream& in ) override;
            virtual void write( std::ostream& out ) const override;
            

    }; //class PositionWeightMatrix

    typedef PositionWeightMatrix PWM;

} //namespace genomics

#endif
