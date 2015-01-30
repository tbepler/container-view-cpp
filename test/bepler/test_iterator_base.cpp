#include "gtest/gtest.h"
#include "bepler/iterator_base.h"
#include "bepler/iterator_specification.h"

//#include <algorithm>
#include <vector>

template< typename I >
struct RangeIter : public RandomAccessIteratorBase< RangeIter<I>, I, I >{
    
    I value_;

    RangeIter( I value ) : value_( value ) { }

    int compareTo( const RangeIter& o ) const{
        return value_ - o.value_;
    }
    
    I dereference( ) const{
        return value_;
    }

    I get( ) const{
        return value_;
    }

    void advance( signed n ){
        value_ += n;
    }

    void inc( ){
        ++value_;
    }

    void dec( ){
        --value_;
    }

};

template< typename I >
class Range{

    I begin_;
    I end_;

    public:
        typedef RangeIter<I> iterator;

        Range( I b, I e ) : begin_( b ), end_( e ) { }
        RangeIter<I> begin() const{
            return RangeIter<I>( begin_ );
        }

        RangeIter<I> end() const{
            return RangeIter<I>( end_ );
        }

};

template< typename I >
Range<I> range( I begin, I end ){
    return Range<I>( begin, end );
}

template< typename T >
class IteratorBaseTest : public ::testing::Test{

}; //class IteratorBaseTest

typedef ::testing::Types<
    std::vector<int>::iterator,
    std::vector<int>::const_iterator,
    std::vector<bool>::iterator,
    std::vector<bool>::const_iterator,
    Range<int>::iterator
> MyTypes;
TYPED_TEST_CASE( IteratorBaseTest, MyTypes );

TYPED_TEST( IteratorBaseTest, MeetsIteratorSpecification ){
    using namespace test::iterator;
    EXPECT_TRUE( isCopyConstructable<TypeParam>() );
    EXPECT_TRUE( isCopyAssignable<TypeParam>() );
    EXPECT_TRUE( isDestructable<TypeParam>() );
    EXPECT_TRUE( canBeIncremented<TypeParam>() );
}

TEST( RangeIterTest, RangeIterTest ){

    auto r = range( -10, 10 );
    int expect = -10;
    for( auto it = r.begin() ; it != r.end() ; ++it ){
        EXPECT_EQ( expect++, *it );
    }
    expect = -10;
    int size = r.end() - r.begin();
    for( int i = 0 ; i < size ; ++i ){
        EXPECT_EQ( expect + i , r.begin()[i] );
        EXPECT_EQ( expect + i , *( r.begin() + i ) );
        EXPECT_EQ( expect + i , *( r.begin() += i ) );

        EXPECT_EQ( expect + i , r.end()[ i - size ] );
    }

}
