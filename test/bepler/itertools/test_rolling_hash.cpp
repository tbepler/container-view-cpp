#include "gtest/gtest.h"
#include "bepler/itertools/rolling_hash.h"

TEST( RollingHashTest, Hashing ){
    
    itertools::RollingHash hash( 4 );
    EXPECT_EQ( 0, hash );
    hash << 3;
    EXPECT_EQ( 3, hash );
    hash >> 3;
    EXPECT_EQ( 0, hash );
    hash << 3 << 1 << 0;
    EXPECT_EQ( 3*16 + 4, hash );
    EXPECT_EQ( 4, 3 << hash );
    EXPECT_EQ( 4*4 + 2, hash << 2 );
    EXPECT_EQ( 0 + 4*2 + 2, 1 << hash << 2 );
    EXPECT_EQ( 4*4 + 2, 1 >> hash >> 2 );

}

#include <iostream>
#include "bepler/benchmark.h"

template< typename R1, typename R2 >
bool equals( const R1& range_1, const R2& range_2 ){
    auto begin1 = std::begin( range_1 );
    auto end1 = std::end( range_1 );
    auto begin2 = std::begin( range_2 );
    auto end2 = std::end( range_2 );
    for( ; begin1 != end1 && begin2 != end2 ; ++begin1, ++begin2 ){
        if( *begin1 != *begin2 ){
            return false;
        }
    }
    return begin1 == end1 && begin2 == end2;
}

const static int seq[] = { 0, 2, 3, 1, 0, 1, 2, 2, 2, 2, 3, 1, 1, 2, 3, 0, 0, 1, 0, 1, 3, 0, 0, 2, 1, 2, 0, 3, 1, 3, 3, 0, 1, 2, 0, 3, 2, 1, 3, 0, 1, 2, 3, 1, 2, 3, 1, 2, 3, 0, 0, 3, 0, 2, 0, 1, 3, 3, 0, 1, 2, 3, 2, 1, 3, 1, 0, 1, 3, 0, 2, 3, 1, 3, 1, 3, 2, 1, 1, 2, 2, 2, 3, 1, 3, 0, 0, 0, 2, 0, 1, 3, 3, 2, 1 };
const static unsigned a = 4;
const static unsigned k = 5;

struct IteratorRollingHashBenchmark{
    unsigned sum = 0;
    unsigned operator()(){
        for( auto i : itertools::rollingHash( seq, k, a ) ){
            sum += i;
            __asm__ __volatile__("");
        }
        return sum;
    }
};

struct GeneratorRollingHashBenchmark{
    unsigned sum = 0;
    unsigned operator()(){
        for( auto i : itertools::rollingHashGenerator( seq, k, a ) ){
            sum += i;
            __asm__ __volatile__("");
        }
        return sum;
    }
};

struct RollingHashBenchmark{
    unsigned sum = 0;
    unsigned operator()(){
        itertools::RollingHash hash( a );
        auto first = std::begin( seq );
        auto last = first + k;
        for( auto it = first ; it != last ; ++it ){
            hash << *it;
        }
        sum += hash.hash();
        auto end = std::end( seq );
        for( ; last != end ; ++first, ++last ){
            *first << hash << *last;
            sum += hash.hash();
            __asm__ __volatile__("");
        }
        return sum;
    }
};

struct NaiveHashBenchmark{
    unsigned sum = 0;
    unsigned operator()(){
        itertools::RollingHash hash( a );
        auto first = std::begin( seq );
        auto last = first + k;
        auto end = std::end( seq );
        for( ; last != end + 1 ; ++first, ++last ){
            hash.reset();
            for( auto it = first ; it != last ; ++it ){
                hash << *it;
                __asm__ __volatile__("");
            }
            sum += hash.hash();
        }
        return sum;
    }
};

TEST( RollingHashTest, HashingIterator ){

    const static int seq[] = { 0, 2, 3, 1, 0, 1, 2, 2, 3, 1 };
    const static unsigned a = 4;
    const static unsigned k = 3;
    const static int expected_hash[] = { 11, 45, 52, 17, 6, 26, 43, 45 };

    EXPECT_TRUE( equals( expected_hash, itertools::rollingHash( seq, k, a ) ) );
    EXPECT_EQ( IteratorRollingHashBenchmark()(), RollingHashBenchmark()() );
    EXPECT_EQ( IteratorRollingHashBenchmark()(), GeneratorRollingHashBenchmark()() );
    EXPECT_EQ( IteratorRollingHashBenchmark()(), NaiveHashBenchmark()() );
    
    double time = benchmark::timeIters( 10000, RollingHashBenchmark() );
    std::cout << "Rolling hash:" << std::endl;
    std::cout << "Cycles per iter: " << time << std::endl;
    std::cout << "Cycles per hash: " << time / 91 << std::endl;

    time = benchmark::timeIters( 10000, IteratorRollingHashBenchmark() );
    std::cout << "Iterator rolling hash:" << std::endl;
    std::cout << "Cycles per iter: " << time << std::endl;
    std::cout << "Cycles per hash: " << time / 91 << std::endl;

    time = benchmark::timeIters( 10000, GeneratorRollingHashBenchmark() );
    std::cout << "Generator rolling hash:" << std::endl;
    std::cout << "Cycles per iter: " << time << std::endl;
    std::cout << "Cycles per hash: " << time / 91 << std::endl;

    time = benchmark::timeIters( 10000, NaiveHashBenchmark() );
    std::cout << "Naive hash:" << std::endl;
    std::cout << "Cycles per iter: " << time << std::endl;
    std::cout << "Cycles per hash: " << time / 91 << std::endl;
}
