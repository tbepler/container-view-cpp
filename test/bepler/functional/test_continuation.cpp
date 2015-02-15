#include "gtest/gtest.h"
#include "bepler/functional/continuation.h"

#include <functional>

using namespace functional;

int add5( int i ){ return i + 5; }

struct MultiOpFunctor{

    template< typename T >
    T operator()( T x ) const{ return x; }

    auto operator()() const{ return 0; }

    template< typename T, typename... Args >
    auto operator()( T x, Args... xs ) const{ return x + (*this)( xs... ); }

};

TEST( ContinuationTest, AsCPS ){

    int v;
    auto cps_plus = as_cps( std::plus<int>() );
    cps_plus( 1, 2, [&]( auto i ){
        v = i;
    } );
    EXPECT_EQ( 3, v );

    cps_plus( -3, -8, [&]( auto i ){
        v = i;
    } );
    EXPECT_EQ( -11, v );

    auto cps_add5 = as_cps( add5 );
    cps_add5( -13, [&]( auto i ){
        v = i;
    } );
    EXPECT_EQ( -8, v );

    auto cps_sum = as_cps( MultiOpFunctor() );
    cps_sum( 1, 1, 5, 0, -3, [&]( auto i ){
        v = i;
    } );
    EXPECT_EQ( 4, v );
    EXPECT_EQ( 4, cps_sum( 1, 1, 5, 0, -3, identity ) );
    EXPECT_EQ( 0, cps_sum( identity ) );

}

template< typename T, T init = T() >
struct sum_aggregator{
    sum_aggregator() : val_( init ) { }
    inline auto operator()( const T& x ) const{ return val_ += x; }
    inline auto gather() const{ return val_; }
    private:
        mutable T val_;
};

TEST( ContinuationTest, Pipe ){

    int i;
    auto f = pipe( as_cps( add5 ), [&](auto v){
        i = v;
    } );
    f( 0 );
    EXPECT_EQ( 5, i );

    f( -13 );
    EXPECT_EQ( -8, i );
    
    auto add10 = pipe( as_cps( add5 ), as_cps( add5 ), identity );
    EXPECT_EQ( 13, add10( 3 ) );

    auto add5_and_sum = pipe( range_c, as_cps( add5 ), sum_aggregator<int>() );
    EXPECT_EQ( 18, add5_and_sum( 0, 3 ) );
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
    
    int begin = 0;
    int last = 1;
    range( window<int>(2)( [&]( const auto& it ){
        int cur = begin;
        for( const auto& i : it ){
            EXPECT_EQ( cur++, i );
        }
        EXPECT_EQ( 2, cur - begin );
        ++begin, ++last;
    } ) )( 0, 20 );
    EXPECT_EQ( 20, last );

    begin = 0;
    last = 3;
    range( window<int>(4)( [&]( const auto& it ){
        int cur = begin;
        for( const auto& i : it ){
            EXPECT_EQ( cur++, i );
        }
        EXPECT_EQ( 4, cur - begin );
        ++begin, ++last;
    } ) )( 0, 20 );
    EXPECT_EQ( 20, last );
    
    begin = 0;
    last = 0;
    range( window<int>(0)( [&]( const auto& it ){
        int cur = begin;
        for( const auto& i : it ){
            EXPECT_EQ( cur++, i );
        }
        EXPECT_EQ( 0, cur - begin );
        ++begin, ++last;
    } ) )( 0, 20 );
    EXPECT_EQ( 20, last );

    begin = 0;
    last = 14;
    range( window<int>(15)( [&]( const auto& it ){
        int cur = begin;
        for( const auto& i : it ){
            EXPECT_EQ( cur++, i );
        }
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
            EXPECT_EQ( str[ cur++ ], i );
        }
        EXPECT_EQ( 8, cur - begin );
        ++begin, ++last;
    } ) )( str );
    EXPECT_EQ( str.size(), last );


}
