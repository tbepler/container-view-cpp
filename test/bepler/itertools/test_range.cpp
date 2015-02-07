#include "gtest/gtest.h"
#include "bepler/itertools/range.h"

#include <type_traits>

template< typename T >
class TypedRangeTest : public ::testing::Test{ };

typedef ::testing::Types< char, int, unsigned, std::size_t, float, double > MyTypes;
TYPED_TEST_CASE( TypedRangeTest, MyTypes );

/*
TYPED_TEST( TypedRangeTest, RangeInstantion ){

    typedef TypeParam test_t;
    
    auto range = itertools::range<test_t,test_t>( test_t( 10 ) );
    EXPECT_EQ( test_t(0), range.first() );
    EXPECT_EQ( test_t(10), range.last() );
    EXPECT_EQ( test_t(1), range.step() );

    range = itertools::range( test_t( 5 ), test_t( -5 ) );
    EXPECT_EQ( test_t(5), range.first() );
    EXPECT_EQ( test_t(-5), range.last() );
    EXPECT_EQ( std::is_signed<test_t>::value ? -1 : 1, range.step() );

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
*/

/*
TYPED_TEST( TypedRangeTest, RangeSize ){

    typedef TypeParam test_t;

    EXPECT_EQ( 10, itertools::range( test_t(10) ).size() );
    EXPECT_EQ( math::positive(test_t(-10)), itertools::range( test_t(-10) ).size() );
    EXPECT_EQ( 0, itertools::range( test_t(1), test_t(1) ).size() );
    EXPECT_EQ( 1, itertools::range( test_t(1.5), test_t(3.5), test_t(3.33) ).size() );


}
*/
/*
TYPED_TEST( TypedRangeTest, RangeIndexing ){
    typedef TypeParam test_t;

    auto range = itertools::range( test_t(10) );
    EXPECT_EQ( test_t(0), range[0] );
    EXPECT_EQ( test_t(5), range[5] );
    EXPECT_THROW( range[10], std::out_of_range );

    test_t first = std::is_signed<test_t>::value ? -5 : 5;
    test_t step = std::is_signed<test_t>::value ? -1.5 : 1.5;
    auto range2 = itertools::range( first, test_t(-12), step );
    EXPECT_EQ( first, range2[0] );
    EXPECT_EQ( first + ( range2.size() - 1 ) * step, range2[ range2.size() - 1 ] );
    EXPECT_THROW( range2[ range2.size() ], std::out_of_range );

    for( unsigned i = 0 ; i < range2.size() && i < 100 ; ++i ){
        EXPECT_EQ( first + i * step, range2[i] );
    }

}
*/
TYPED_TEST( TypedRangeTest, RangeIterator ){
    typedef TypeParam test_t;

    auto range = itertools::range( test_t( 2.5 ), test_t( 22.25 ), test_t( 3.33 ) );
    //EXPECT_EQ( range.size(), range.end() - range.begin() );
    unsigned i = 0;
    for( auto it = range.begin() ; it != range.end() ; ++it ){
        EXPECT_EQ( range[i++], *it );
    }
    //test for-each
    i = 0;
    for( test_t val : itertools::range( test_t( 2.5 ), test_t( 22.25 ), test_t( 3.33 ) ) ){
        EXPECT_EQ( range[i++], val );
    }

}
/*
TYPED_TEST( TypedRangeTest, ReverseRange ){
    typedef TypeParam test_t;

    auto range = itertools::range( test_t( 15.2 ) );
    EXPECT_EQ( range.size(), range.reverse().size() );
    auto end = range.end();
    for( test_t val : range.reverse() ){
        EXPECT_EQ( *end, val );
        --end;
    }

}

TEST( RangeTest, Comparable ){
    auto range0 = itertools::range( int( 15 ) );
    EXPECT_EQ( itertools::range( int( 15 ) ), range0 );
    EXPECT_TRUE( itertools::range( int( 14 ) ) < range0 );
    EXPECT_TRUE( itertools::range( int(-5), int(15) ) < range0 );
    EXPECT_TRUE( itertools::range( int(17) ) > range0 );
    auto range1 = itertools::range( double( 15 ) );
    auto range2 = itertools::range( float( 0 ), float( 15 ), float( 1 ) );
    EXPECT_EQ( range0, range1 );
    EXPECT_EQ( range1, range2 );
    EXPECT_EQ( range0, range2 );
    EXPECT_TRUE( range0 <= range1 );
    EXPECT_FALSE( range0 < range1 );
    EXPECT_TRUE( range2 >= range1 );
    EXPECT_FALSE( range2 > range1 );
    EXPECT_TRUE( range0 >= range2 );
    EXPECT_FALSE( range0 > range2 );
    EXPECT_TRUE( range1 <= range0 );
    EXPECT_FALSE( range1 < range0 );
    EXPECT_TRUE( itertools::range( -2.5 ) < range2 );
    EXPECT_TRUE( itertools::range( -3.3 ) < range0 );
    EXPECT_FALSE( itertools::range( -3.3 ) > range0 );
    EXPECT_NE( itertools::range( -3.3 ), range0 );
}
*/
