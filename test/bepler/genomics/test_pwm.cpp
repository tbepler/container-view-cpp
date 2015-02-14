#include "gtest/gtest.h"
#include "bepler/genomics/pwm.h"

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cmath>

const static std::string alph = "ACGT";
const static std::vector< std::vector< double > > probs = {
{ 0.001, 0.001, 0.001, 0.997 },
{ 0.25, 0.25, 0.25, 0.25 },
{ 0.7, 0.1, 0.1, 0.1 },
{ 0.33, 0.33, 0.33, 0.01 },
{ 0.25, 0.5, 0.24, 0.01 }
};

const static std::string text = 
"A C G T\n0.001 0.001 0.001 0.997\n0.25 0.25 0.25 0.25\n0.7 0.1 0.1 0.1\n0.33 0.33 0.33 0.01\n0.25 0.5 0.24 0.01";

TEST( PositionWeightMatrixTest, Initialize ){

    genomics::PWM pwm;
    pwm.probabilities( alph, probs );
    for( unsigned i = 0 ; i < 4 ; ++i ){
        for( unsigned j = 0 ; j < 5 ; ++j ){
            EXPECT_EQ( std::log( probs[j][i] ), pwm.loglikelihood( alph[i], j ) );
        }
    }
    pwm.clear();
    EXPECT_EQ( 0, pwm.size() );
    std::stringstream ss( text );
    ss >> pwm;
    EXPECT_EQ( alph, pwm.alphabet() );
    for( unsigned i = 0 ; i < 4 ; ++i ){
        for( unsigned j = 0 ; j < 5 ; ++j ){
            EXPECT_EQ( std::log( probs[j][i] ), pwm.loglikelihood( alph[i], j ) );
        }
    }

}

TEST( PositionWeightMatrixTest, Score ){
    using std::log;
    std::string str = "CCAGT";
    double exp_score = log( 0.001 ) + log( 0.25 ) + log( 0.7 ) + log( 0.33 ) + log( 0.01 );
    
    genomics::PWM pwm( alph, probs );
    EXPECT_EQ( exp_score, pwm.score( str.begin() ) );

    unsigned idxs[] = { 0, 1, 1, 0, 2, 0, 3, 3, 0, 1, 3, 3, 3, 3, 1, 3 };
    str = "ACCAGATTACTTTTCT";
    std::vector<double> exp_scores = {
        log( probs[0][0] ) + log( probs[1][1] ) + log( probs[2][1] ) + log( probs[3][0] ) + log( probs[4][2] ),
        log( probs[0][1] ) + log( probs[1][1] ) + log( probs[2][0] ) + log( probs[3][2] ) + log( probs[4][0] ),
        log( probs[0][1] ) + log( probs[1][0] ) + log( probs[2][2] ) + log( probs[3][0] ) + log( probs[4][3] ),
        log( probs[0][0] ) + log( probs[1][2] ) + log( probs[2][0] ) + log( probs[3][3] ) + log( probs[4][3] ),
        log( probs[0][2] ) + log( probs[1][0] ) + log( probs[2][3] ) + log( probs[3][3] ) + log( probs[4][0] ),
        log( probs[0][0] ) + log( probs[1][3] ) + log( probs[2][3] ) + log( probs[3][0] ) + log( probs[4][1] ),
        log( probs[0][3] ) + log( probs[1][3] ) + log( probs[2][0] ) + log( probs[3][1] ) + log( probs[4][3] ),
        log( probs[0][3] ) + log( probs[1][0] ) + log( probs[2][1] ) + log( probs[3][3] ) + log( probs[4][3] ),
        log( probs[0][0] ) + log( probs[1][1] ) + log( probs[2][3] ) + log( probs[3][3] ) + log( probs[4][3] ),
        log( probs[0][1] ) + log( probs[1][3] ) + log( probs[2][3] ) + log( probs[3][3] ) + log( probs[4][3] ),
        log( probs[0][3] ) + log( probs[1][3] ) + log( probs[2][3] ) + log( probs[3][3] ) + log( probs[4][1] ),
        log( probs[0][3] ) + log( probs[1][3] ) + log( probs[2][3] ) + log( probs[3][1] ) + log( probs[4][3] )
    };

    std::vector<double> scores;
    pwm( std::begin( str ), std::end( str ), std::back_insert_iterator<std::vector<double> >( scores ) );

    EXPECT_EQ( exp_scores, scores );


}
