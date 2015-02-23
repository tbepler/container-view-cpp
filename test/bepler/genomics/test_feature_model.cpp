#include "gtest/gtest.h"
#include "bepler/genomics/feature_model.h"

#include "bepler/benchmark.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;
using namespace genomics;

static const string model_string =
"[0]A 2"
"[0]C 2"
"[1]A 1"
"[1]C 1"
"[1]G 1"
"[1]T 1"
"[2]C 3"
"[2]T 1"
"[0]AT 1"
"[0]AG 2"
"[0]CG 1"
"[0]CC 2"
"[1]TT 2"
"[1]TC 1"
"[0]ATC 3"
"[0]AGC 1";

static const string test = "ACGATGC";
static const vector<double> expected = { 3, 4, 2, 4, 4 };

static const vector<Feature> features =
{
    Feature( "A", 0 ),
    Feature( "C", 0 ),
    Feature( "A", 1 ),
    Feature( "C", 1 ),
    Feature( "G", 1 ),
    Feature( "T", 1 ),
    Feature( "C", 2 ),
    Feature( "T", 2 ),
    Feature( "AT", 0 ),
    Feature( "AG", 0 ),
    Feature( "CG", 0 ),
    Feature( "CC", 0 ),
    Feature( "TT", 1 ),
    Feature( "TC", 1 ),
    Feature( "ATC", 0 ),
    Feature( "AGC", 0 )
};

static const vector<double> scores =
{
    2,
    2, 
    1,
    1,
    1,
    1,
    3,
    1,
    1,
    2,
    1,
    2,
    2,
    1,
    3,
    1
};

TEST( FeatureModelTest, Initialization ){

    FeatureModel empty;
    EXPECT_EQ( 0, empty.length() );

    FeatureModel constructed( features.begin(), features.end(), scores.begin(), scores.end() );
    EXPECT_EQ( 3, constructed.length() );
    EXPECT_NE( empty, constructed );

    FeatureModel from_stream;
    stringstream ss( model_string );
    ss >> from_stream;

    EXPECT_EQ( 3, from_stream.length() );
    EXPECT_EQ( constructed, from_stream );
    EXPECT_NE( empty, from_stream );

    FeatureModel assigned;
    assigned.assign( features.begin(), features.end(), scores.begin(), scores.end() );
    EXPECT_EQ( constructed, assigned );
    EXPECT_EQ( from_stream, assigned );
    EXPECT_NE( empty, assigned );

    assigned.clear();
    EXPECT_EQ( empty, assigned );
    EXPECT_NE( from_stream, assigned );
    EXPECT_NE( constructed, assigned );


    stringstream ss2;
    ss2 << from_stream;
    FeatureModel back_in;
    ss2 >> back_in;
    EXPECT_EQ( from_stream, back_in );
    EXPECT_NE( empty, back_in );
  

}


TEST( FeatureModelTest, Score ){

    FeatureModel model( features.begin(), features.end(), scores.begin(), scores.end() );
   
    EXPECT_EQ( expected[0], model.score( test ) );
    for( unsigned i = 0 ; i < expected.size() ; ++i ){
        EXPECT_EQ( expected[i], model.score( test.substr( i ) ) );
        EXPECT_EQ( expected[i], model( test.substr( i ) ) );
        EXPECT_EQ( expected[i], model.score(
            functional::irange( test.substr( i ) ) ) );
        EXPECT_EQ( expected[i], model(
            functional::irange( test.substr( i ) ) ) );
    }

    vector<double> scores;
    model.scoreAll( test, [&]( auto s ){
        scores.push_back( s );
    } );
    EXPECT_EQ( expected, scores );

    scores.clear();
    model( test, [&]( auto s ){
        scores.push_back( s );
    } );
    EXPECT_EQ( expected, scores );
    
    scores.clear();
    model.scoreAll( functional::irange( test ), [&]( auto s ){
        scores.push_back( s );
    } );
    EXPECT_EQ( expected, scores );

    scores.clear();
    model( functional::irange( test ), [&]( auto s ){
        scores.push_back( s );
    } );
    EXPECT_EQ( expected, scores );
    

}

struct ScoreString{

    double sum = 0;
    const FeatureModel& model;
    ScoreString( const FeatureModel& m ) : model( m ) { }
    double operator()(){
        for( double i = 0 ; i < expected.size() ; ++i ){
            sum += model( test.substr( i ) );
        }
        return sum;
    }

};

struct ScoreGenerator{

    double sum = 0;
    const FeatureModel& model;
    ScoreGenerator( const FeatureModel& m ) : model( m ) { }
    double operator()(){
        for( double i = 0 ; i < expected.size() ; ++i ){
            sum += model( functional::irange( test.substr( i ) ) );
        }
        return sum;
    }

};

struct ScoreAllString{

    double sum = 0;
    const FeatureModel& model;
    ScoreAllString( const FeatureModel& m ) : model( m ) { }
    double operator()(){
        //model( test, [this]( auto s ){
        //    this->sum += s;
        //} );
        sum += functional::foldl( std::plus<double>(), 0.0, 
            std::bind( model, test ) );
        return sum;
    }

};

struct ScoreAllGenerator{

    double sum = 0;
    const FeatureModel& model;
    ScoreAllGenerator( const FeatureModel& m ) : model( m ) { }
    double operator()(){
        //model( functional::irange( test ), [this]( auto s ){
        //    this->sum += s;
        //} );
        sum += functional::foldl( std::plus<double>(), 0.0, 
            std::bind( model, functional::irange( test ) ) );
        return sum;
    }

};

TEST( FeatureModelTest, Benchmark ){

    FeatureModel model( features.begin(), features.end(), scores.begin(), scores.end() );

    double time = benchmark::timeIters( 10000, ScoreString( model ) );
    std::cout << "Score string:" << std::endl;
    std::cout << "Cycles per iter: " << time << std::endl;
    std::cout << "Cycles per score: " << time / expected.size() << std::endl;
    
    time = benchmark::timeIters( 10000, ScoreGenerator( model ) );
    std::cout << "Score generator:" << std::endl;
    std::cout << "Cycles per iter: " << time << std::endl;
    std::cout << "Cycles per score: " << time / expected.size() << std::endl;

    time = benchmark::timeIters( 10000, ScoreAllString( model ) );
    std::cout << "Score all string:" << std::endl;
    std::cout << "Cycles per iter: " << time << std::endl;
    std::cout << "Cycles per score: " << time / expected.size() << std::endl;
    
    time = benchmark::timeIters( 10000, ScoreAllGenerator( model ) );
    std::cout << "Score all generator:" << std::endl;
    std::cout << "Cycles per iter: " << time << std::endl;
    std::cout << "Cycles per score: " << time / expected.size() << std::endl;

}



