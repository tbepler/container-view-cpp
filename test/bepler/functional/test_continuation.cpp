#include "gtest/gtest.h"
#include "bepler/functional/continuation.h"

using namespace functional;

int add5( int i ){ return i + 5; }

TEST( ContinuationTest, Bind ){

    auto f = bind( &add5, &add5 );
    EXPECT_EQ( 10, f( 0 ) );

}
