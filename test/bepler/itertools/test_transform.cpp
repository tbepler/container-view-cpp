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

TEST( TestTransform, TransformIterators ){
    static const int nums[] = { 1, 5, -12, 35, 13, 13, 0 };
    Adder<int> add5( 5 );
    unsigned i = 0;
    for( auto val : itertools::transform( nums, add5 ) ){
        EXPECT_EQ( nums[i++] + 5, val );
    }
    EXPECT_EQ( i, 7 );
    i = 0;
    for( auto val : itertools::transform( nums, &addFive ) ){
        EXPECT_EQ( nums[i++] + 5, val );
    }
    EXPECT_EQ( i, 7 );
}

TEST( TestTransform, TransformRange ){

}
