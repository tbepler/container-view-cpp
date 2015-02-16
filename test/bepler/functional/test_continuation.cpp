#include "gtest/gtest.h"
#include "bepler/functional/continuation.h"

#include <functional>
#include <vector>

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

TEST( ContinuationTest, Range ){

    int begin = -10;
    int end = 10;
    
    int val = begin;
    range( begin, end, [&]( auto i ){
        int v = val++;
        EXPECT_EQ( v, i );
    } );
    EXPECT_EQ( end, val );

    double t = 20;
    range( 20.0, 2.0, [&]( auto i ){
        double v = t--;
        EXPECT_EQ( v, i );
    } );
    EXPECT_EQ( 2.0, t );

    int step = -2;
    val = end;
    range( end, begin, step, [&]( auto i ){
        int v = val;
        val += step;
        EXPECT_EQ( v, i );
    } );
    EXPECT_EQ( begin, val );

    auto test_r = range( end, begin );
    val = end;
    test_r( [&]( auto i ){
        int v = val--;
        EXPECT_EQ( v, i );
    } );
    EXPECT_EQ( begin, val );

    t = 10.0;
    range( 10.0, 0.0, -1.5, [&]( auto i ){
        double v = t;
        t -= 1.5;
        EXPECT_EQ( v, i );
    } );
    EXPECT_EQ( -0.5, t );

}

TEST( ContinuationTest, Window ){
    
    int begin = 0;
    int last = 1;
    window<int>( 2, range( 0, 20 ), [&]( auto&& w ){
        int cur = begin;
        w( [&]( const auto& i ){
            int val = cur++;
            EXPECT_EQ( val, i );
        } );
        EXPECT_EQ( 2, cur - begin );
        ++begin, ++last;
    } );
    EXPECT_EQ( 20, last );

    begin = 0;
    last = 3;
    window<int>( 4, range( 0, 20 ), [&]( auto&& w ){
        int cur = begin;
        w( [&]( const auto& i ){
            int val = cur++;
            EXPECT_EQ( val, i );
        } );
        EXPECT_EQ( 4, cur - begin );
        ++begin, ++last;
    } );
    EXPECT_EQ( 20, last );
    
    begin = 0;
    last = 0;
    window<int>( 0, range( 0, 20 ), [&]( auto&& w ){
        int cur = begin;
        w( [&]( const auto& i ){
            int val = cur++;
            EXPECT_EQ( val, i );
        } );
        EXPECT_EQ( 0, cur - begin );
        ++begin, ++last;
    } );
    EXPECT_EQ( 20, last );

    begin = 0;
    last = 14;
    window<int>( 15, range( 0, 20 ), [&]( auto&& w ){
        int cur = begin;
        w( [&]( const auto& i ){
            int val = cur++;
            EXPECT_EQ( val, i );
        } );
        EXPECT_EQ( 15, cur - begin );
        ++begin, ++last;
    } );
    EXPECT_EQ( 20, last );

    std::string str = "ACGCTCAGGCAGCTGACTAGCATCGACTGATCGCTGTCGATGCGATATATATACGGCCGGCAG";
    begin = 0;
    last = 7;
    window<char>( 8, irange( str ), [&]( auto&& w ){
        int cur = begin;
        w( [&]( const auto& i ){
            int val = cur++;
            std::string& s = str;
            EXPECT_EQ( s[ val ], i );
        } );
        EXPECT_EQ( 8, cur - begin );
        ++begin, ++last;
    } );
    EXPECT_EQ( str.size(), last );


}

TEST( ContinuationTest, Fold ){

    int val = foldl( std::plus<int>(), 0, curry( range, 0, 21 ) );
    EXPECT_EQ( 210, val );

    auto push_back = []( auto& c, const auto& x ){
        c.push_back( x ); return c;
    };
    std::vector<double> exp{ -13.3, -5.444, 5.43, 0.242, 14, 2.24 };
    auto res = foldl( push_back, std::vector<double>(), curry( irange, exp ) );
    EXPECT_EQ( exp, res );

    double sum = foldl( std::plus<double>(), 0.0, curry( irange, exp ) );
    EXPECT_EQ( -13.3  - 5.444 + 5.43 + 0.242 + 14 + 2.24, sum );
   
    auto sum_and_push = []( auto& c, auto&& g ){
        c.push_back( foldl( std::plus<int>(), 0, g ) );
        return c;
    };
    auto win_sum = foldl( sum_and_push, std::vector<int>(), window<int>( 3, range( 0, 10 ) ) );
    std::vector<int> win_exp{ 3, 6, 9, 12, 15, 18, 21, 24 };
    EXPECT_EQ( win_exp, win_sum );

}

TEST( ContinuationTest, Map ){

    auto map5 = map( []( auto x ){ return x + 5; }, curry( range, 0, 21 ) );
    EXPECT_EQ( 315, foldl( std::plus<int>(), 0, map5 ) );

    int expect = 3;
    auto sum_int = foldl( std::plus<int>(), 0 );
    map( sum_int, window<int>( 3, range( 0, 10 ) ), [&]( auto i ){
        int val = expect;
        EXPECT_EQ( val, i );
        expect += 3;
    } );
    EXPECT_EQ( 27, expect );

}

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

    //auto add5_and_sum = pipe( range_c, as_cps( add5 ), sum_aggregator<int>() );
    //EXPECT_EQ( 18, add5_and_sum( 0, 3 ) );
}
