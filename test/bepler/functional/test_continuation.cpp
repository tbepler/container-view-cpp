#include "gtest/gtest.h"
#include "bepler/functional/continuation.h"

using namespace functional;

int add5( int i ){ return i + 5; }

TEST( ContinuationTest, Pipe ){

    auto f = pipe( &add5, &add5 );
    EXPECT_EQ( 10, f( 0 ) );

    int i;
    auto for_ = pipe( &add5, [&](auto v){ i = v; } );
    for_( 1 );
    EXPECT_EQ( 6, i );
    for_( 2 );
    EXPECT_EQ( 7, i );
    for_( -6 );
    EXPECT_EQ( -1, i );

}
