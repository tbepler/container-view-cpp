#include "gtest/gtest.h"
#include "bepler/itertools/transform.h"

template< typename T >
class Adder{
    T val_;
    public:
        Adder() { }
        Adder( const T& val ) : val_( val ) { }
        T operator()( const T& t ) const{ return val_ + t; }
};

template< typename T >
T addFive( const T& val ){ return val + 5; }

TEST( TransformTest, TransformIterators ){
    static const int nums[] = { 1, 5, -12, 35, 13, 13, 0 };
    Adder<int> add5( 5 );
    unsigned i = 0;
    for( auto val : itertools::transform( std::begin( nums ), std::end( nums ), add5 ) ){
        EXPECT_EQ( nums[i++] + 5, val );
    }
    EXPECT_EQ( i, 7 );
    
    i = 0;
    for( auto val : itertools::transform( std::begin( nums ), std::end( nums ), &addFive<int> ) ){
        EXPECT_EQ( nums[i++] + 5, val );
    }
    EXPECT_EQ( i, 7 );
    
    i = 0;
    for( auto val : itertools::map( add5, std::begin( nums ), std::end( nums ) ) ){
        EXPECT_EQ( nums[i++] + 5, val );
    }
    EXPECT_EQ( i, 7 );
    
    i = 0;
    for( auto val : itertools::map( &addFive<int>, std::begin( nums ), std::end( nums ) ) ){
        EXPECT_EQ( nums[i++] + 5, val );
    }
    EXPECT_EQ( i, 7 );
}

TEST( TransformTest, TransformRange ){
    static const int nums[] = { 1, 5, -12, 35, 13, 13, 0 };
    Adder<int> add5( 5 );
    unsigned i = 0;
    for( auto val : itertools::transform( nums, add5 ) ){
        EXPECT_EQ( nums[i++] + 5, val );
    }
    EXPECT_EQ( i, 7 );
    
    i = 0;
    for( auto val : itertools::transform( nums, &addFive<int> ) ){
        EXPECT_EQ( nums[i++] + 5, val );
    }
    EXPECT_EQ( i, 7 );
    
    i = 0;
    for( auto val : itertools::map( add5, nums ) ){
        EXPECT_EQ( nums[i++] + 5, val );
    }
    EXPECT_EQ( i, 7 );
    
    i = 0;
    for( auto val : itertools::map( &addFive<int>, nums ) ){
        EXPECT_EQ( nums[i++] + 5, val );
    }
    EXPECT_EQ( i, 7 );
}
