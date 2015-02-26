#include "gtest/gtest.h"
#include "bepler/functional/generator.h"

#include <iostream>

struct show{

    template< typename T >
    inline T operator()( T&& x ) const{
        std::cout << x << std::endl;
        return x;
    }

};

TEST( FunctionalListTest, ConstructList ){
    using namespace functional;
    auto l = list( 1, 2, 3, 4, 5 );
    map_list( show(), l );

}
