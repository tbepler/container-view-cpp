#include "gtest/gtest.h"
#include "bepler/itertools/range.h"

#include <type_traits>

template< typename T >
class RangeTest : public ::testing::Test{ };

typedef ::testing::Types< char, int, unsigned, float, double > MyTypes;
TYPED_TEST_CASE( RangeTest, MyTypes );

TYPED_TEST( RangeTest, RangeInstantion ){

    typedef TypeParam test_t;
    
    auto range = itertools::range( test_t( 10 ) );
    EXPECT_EQ( test_t(0), range.first() );
    EXPECT_EQ( test_t(10), range.last() );
    EXPECT_EQ( test_t(1), range.step() );

    range = itertools::range( test_t( 5 ), test_t( -5 ) );
    EXPECT_EQ( test_t(5), range.first() );
    EXPECT_EQ( test_t(-5), range.last() );
    EXPECT_EQ( std::is_signed<test_t>::value ? test_t(-1) : test_t(1), range.step() );

    range = itertools::range( test_t(2.5), test_t(25), test_t(1.5) );
    EXPECT_EQ( test_t(2.5), range.first() );
    EXPECT_EQ( test_t(25), range.last() );
    EXPECT_EQ( test_t(1.5), range.step() );

    if( std::is_signed<test_t>::value ){
        EXPECT_THROW( 
            range = itertools::range( test_t(2), test_t(10), test_t(-1) ),
            std::logic_error );
    }
}
