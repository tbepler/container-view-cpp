#include "gtest/gtest.h"
#include "bepler/functional/continuation.h"

using namespace functional;

int add5( int i ){ return i + 5; }

TEST( ContinuationTest, Pipe ){

    auto add5_cont = []( int i, auto f ){ return f( i + 5 ); };

    //auto f = pipe( add5_cont, &add5 );
    //EXPECT_EQ( 10, f( 0 ) );

    int i;
    pipe( [](auto&& k){ return k( 5 ); }, [&](auto v){
        i = v;
    } );
    EXPECT_EQ( 5, i );
    //for_( 2 );
    //EXPECT_EQ( 7, i );
    //for_( -6 );
    //EXPECT_EQ( -1, i );

    //auto lambda_f = pipe( [](int n){ return n; }, &add5 );
    //EXPECT_EQ( 10, lambda_f( 5 ) );

}

TEST( ContinuationTest, Range ){

    int begin = -10;
    int end = 10;
    
    int val = begin;
    range( [&]( auto i ){
        int v = val++;
        EXPECT_EQ( v, i );
    } )( begin, end );
    EXPECT_EQ( end, val );

    double t = 20;
    range( [&]( auto i ){
        double v = t--;
        EXPECT_EQ( v, i );
    } )( 20.0, 3.0 );
    EXPECT_EQ( 3.0, t );

    int step = -2;
    val = end;
    range( [&]( auto i ){
        int v = val;
        val += step;
        EXPECT_EQ( v, i );
    } )( end, begin, step );
    EXPECT_EQ( begin, val );

    t = 10.0;
    range( [&]( auto i ){
        double v = t;
        t -= 1.5;
        EXPECT_EQ( v, i );
    } )( 10.0, 0.0, -1.5 );
    EXPECT_EQ( -0.5, t );

}

TEST( ContinuationTest, Window ){

    

}
