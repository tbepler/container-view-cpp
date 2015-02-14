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

#include <iostream>

TEST( ContinuationTest, Window ){
    
    int begin = 0;
    int last = 1;
    range( window<int>(2)( [&]( const auto& it ){
        int cur = begin;
        bool first = true;
        for( const auto& i : it ){
            if( ! first ) std::cout << ", ";
            first = false;
            std::cout << i;
            EXPECT_EQ( cur++, i );
        }
        std::cout << std::endl;
        EXPECT_EQ( 2, cur - begin );
        ++begin, ++last;
    } ) )( 0, 20 );
    EXPECT_EQ( 20, last );

    begin = 0;
    last = 3;
    range( window<int>(4)( [&]( const auto& it ){
        int cur = begin;
        bool first = true;
        for( const auto& i : it ){
            if( ! first ) std::cout << ", ";
            first = false;
            std::cout << i;
            EXPECT_EQ( cur++, i );
        }
        std::cout << std::endl;
        EXPECT_EQ( 4, cur - begin );
        ++begin, ++last;
    } ) )( 0, 20 );
    EXPECT_EQ( 20, last );
    
    begin = 0;
    last = 0;
    range( window<int>(0)( [&]( const auto& it ){
        int cur = begin;
        bool first = true;
        for( const auto& i : it ){
            if( ! first ) std::cout << ", ";
            first = false;
            std::cout << i;
            EXPECT_EQ( cur++, i );
        }
        std::cout << std::endl;
        EXPECT_EQ( 0, cur - begin );
        ++begin, ++last;
    } ) )( 0, 20 );
    EXPECT_EQ( 20, last );

    begin = 0;
    last = 14;
    range( window<int>(15)( [&]( const auto& it ){
        int cur = begin;
        bool first = true;
        for( const auto& i : it ){
            if( ! first ) std::cout << ", ";
            first = false;
            std::cout << i;
            EXPECT_EQ( cur++, i );
        }
        std::cout << std::endl;
        EXPECT_EQ( 15, cur - begin );
        ++begin, ++last;
    } ) )( 0, 20 );
    EXPECT_EQ( 20, last );

    std::string str = "ACGCTCAGGCAGCTGACTAGCATCGACTGATCGCTGTCGATGCGATATATATACGGCCGGCAG";
    begin = 0;
    last = 7;
    irange( window<char>(8)( [&]( const auto& it ){
        int cur = begin;
        for( const auto& i : it ){
            std::cout << i;
            EXPECT_EQ( str[ cur++ ], i );
        }
        std::cout << std::endl;
        EXPECT_EQ( 8, cur - begin );
        ++begin, ++last;
    } ) )( str );
    EXPECT_EQ( str.size(), last );


}
