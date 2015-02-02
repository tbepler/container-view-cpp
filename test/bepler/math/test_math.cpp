#include "gtest/gtest.h"
#include "bepler/math/math.h"

TEST( MathTest, PowTest ){

    EXPECT_EQ( 4, math::pow( 2, 2 ) );
    EXPECT_EQ( 1, math::pow( 10, 0 ) );
    EXPECT_EQ( -3, math::pow( -3, 1 ) );
    EXPECT_EQ( -125, math::pow( -5, 3 ) );
    EXPECT_EQ( 1, math::pow( 1, 53 ) );

    EXPECT_THROW( math::pow( 10, -1 ), std::invalid_argument );
    EXPECT_THROW( math::pow( -100, -13 ), std::invalid_argument );
}

TEST( MathTest, SignTest ){

    EXPECT_EQ( 1, math::sign( 50 ) );
    EXPECT_EQ( 1, math::sign( 0.334 ) );
    EXPECT_EQ( 1, math::sign( 9.8 ) );

    EXPECT_EQ( 0, math::sign( 0 ) );

    EXPECT_EQ( -1, math::sign( -2 ) );
    EXPECT_EQ( -1, math::sign( -0.5 ) );
    EXPECT_EQ( -1, math::sign( -3919149.4814 ) );

}
