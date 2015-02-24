#include "gtest/gtest.h"
#include "bepler/functional/fold.h"

#include "bepler/functional/generator.h"
#include "bepler/benchmark.h"
#include <iostream>

TEST( FunctionalFoldTest, Fold ){
    using namespace functional;

    int array[] = { 1, 1, 5, -6, 5, 8, 1, 200, -53, 139, -42 };
    auto sum = fold( std::plus<int>(), 0 );

    EXPECT_EQ( 259, sum( irange2( array ) ) );
    EXPECT_EQ( 259, fold( std::plus<int>(), 0, irange2( array ) ) );

}


const static int array[] = { 1, 1, 5, -6, 5, 8, 1, 200, -53, 139, -42,
129, 10341, -1414, 0, 12, -41, 1131, -496, 494, 3953, 958674939, 2424, 352593, -2482141, 1482, 0, -248, 8181, -2, -24, -2442, 149284};

struct SumLoop{
    int total = 0;
    int operator()(){
        for( int i : array ){
            __asm__ __volatile__("");
            total += i;
        }
        return total;
    }
};

struct add{

    template< typename T, typename U >
    constexpr auto operator()( T a, U b ){ return a + b; }

};

struct SumFold{
    int total = 0;
    int operator()(){
        using namespace functional;
        auto sum = fold( add(), 0 );
        total += sum( [&]( auto&& k ){
            for( int i : array ){
                __asm__ __volatile__("");
                k( i );
            }
        } );
        return total;
    }
};

TEST( FunctionalFoldTest, Benchmark ){

    double time = benchmark::timeIters( 10000, SumLoop() );
    std::cout << "sum loop:" << std::endl;
    std::cout << "cycles per iter: " << time << std::endl;

    time = benchmark::timeIters( 10000, SumFold() );
    std::cout << "sum fold:" << std::endl;
    std::cout << "cycles per iter: " << time << std::endl;

}
