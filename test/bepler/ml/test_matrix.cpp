#include "gtest/gtest.h"
#include "bepler/ml/matrix.h"

using namespace ml;

TEST( MatrixTest, Center ){

    matrix_d A( 5, 5 );
    A.fill( 10 );
    EXPECT_EQ( 250, sum( A ) );
    EXPECT_EQ( 0, sum( center( A ) ) );
    

}
