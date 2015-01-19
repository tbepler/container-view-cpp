#include "gtest/gtest.h"
#include "bepler/math/math.h"

TEST( PowTest, HandlesPositiveExp ){

    EXPECT_EQ( 4, math::pow( 2, 2 ) );
    EXPECT_EQ( 1, math::pow( 10, 0 ) );
    EXPECT_EQ( -3, math::pow( -3, 1 ) );
    EXPECT_EQ( -125, math::pow( -5, 3 ) );
    EXPECT_EQ( 1, math::pow( 1, 53 ) );

}

TEST( PowTest, HandlesNegativeExp ){

    EXPECT_THROW( math::pow( 10, -1 ), std::invalid_argument );
    EXPECT_THROW( math::pow( -100, -13 ), std::invalid_argument );

}
