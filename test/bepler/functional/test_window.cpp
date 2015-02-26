#include "gtest/gtest.h"
#include "bepler/functional/window.h"
#include "bepler/functional/generator.h"

#include <iostream>

TEST( FunctionalWindowTest, RollingWindow ){
    using namespace functional;

    int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int start = 0;
    rolling_window<int>( 3, irange2( array ), [&]( auto&& w ){
        int s = start;
        w( [&]( int x ){
            std::cout << x << " ";
            EXPECT_EQ( s, x );
            ++s;
        } );
        std::cout << std::endl;
        ++start;
    } );
    EXPECT_EQ( 8, start );

}
