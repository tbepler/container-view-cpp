#include "gtest/gtest.h"
#include "bepler/itertools/sliding_window.h"

//#include <iostream>

using namespace itertools;

TEST( SlidingWindowTest, ForEach ){
    static const int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    static const int sizes[] = { 1, 2, 3, 10, 11 };
    
    for( auto size : sizes ){
        int offset = 0;
        for( auto w : window( array, size ) ){
            int cur = offset++;
            for( auto i : w ){
                EXPECT_EQ( array[ cur++ ], i );
            }
            EXPECT_EQ( size, cur - offset + 1 );
        }
        EXPECT_EQ( 10 - size + 1, offset );
    }

    EXPECT_THROW( window( array, 0 ), std::out_of_range );

}
