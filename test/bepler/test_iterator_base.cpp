#include "gtest/gtest.h"
#include "bepler/iterator_base.h"
#include "bepler/iterator_specification.h"

//#include <algorithm>

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
        typedef RangeIter<I> iterator_type;

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

TEST( IteratorBaseTest, MeetsIteratorSpecification ){
    using namespace test::iterator;
    auto r = range( -10, 10 );
    typedef decltype(r)::iterator_type iterator_type;
    EXPECT_TRUE( canBeIncremented<iterator_type>() );
    EXPECT_FALSE( canBeIncremented< Range<int> >() );
    EXPECT_TRUE( canBeIncremented< int > ( ) );
    EXPECT_TRUE( canBeIncremented< std::vector<int>::iterator >() );
    EXPECT_FALSE( canBeIncremented< std::vector<int> >() );
}

TEST( IteratorBaseTest, RangeIterTest ){

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
