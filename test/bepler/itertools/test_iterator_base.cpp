#include "gtest/gtest.h"
#include "bepler/itertools/iterator_base.h"

#include "bepler/type_traits/iterator_traits.h"
#include <type_traits>
#include <iterator>

using namespace itertools;

template< typename T, typename C >
class TestIterator
    : public iterator_base< TestIterator<T,C>, C, T, T >{

    friend class iterator_base< TestIterator<T,C>, C, T, T >::base;
    typedef typename iterator_base< TestIterator<T,C>, C, T, T >::base base;
    T val_;
    inline typename base::reference dereference() const{ return val_; }
    inline void inc(){ ++val_; }
    inline void dec(){ --val_; }
    inline void advance( typename base::difference_type n ){ val_ += n; }
    inline bool equals( const TestIterator& rhs ) const{ return val_ == rhs.val_; }
    inline typename base::difference_type compareTo( const TestIterator& rhs ) const{ return val_ - rhs.val_; }

    public:
        TestIterator( ) { }
        TestIterator( const TestIterator& rhs ) : val_( rhs.val_ ) { }
        TestIterator( const T& rhs ) : val_ ( rhs ) { }
    
};

template< typename T >
class IteratorBaseTest : public ::testing::Test { };

typedef ::testing::Types<
    std::input_iterator_tag,
    std::output_iterator_tag,
    std::forward_iterator_tag,
    std::bidirectional_iterator_tag,
    std::random_access_iterator_tag
> MyTypes;
TYPED_TEST_CASE( IteratorBaseTest, MyTypes );

TYPED_TEST( IteratorBaseTest, IteratorBase ){

    TestIterator<int,TypeParam> int_iter;
    EXPECT_TRUE( type_traits::is_iterator< decltype( int_iter ) >::value );
    EXPECT_EQ(
        ( type_traits::is_input_iterator_category< TypeParam >::value ),
        ( type_traits::is_input_iterator< decltype( int_iter ) >::value )
        );
    EXPECT_EQ(
        ( type_traits::is_output_iterator_category< TypeParam >::value ),
        ( type_traits::is_output_iterator< decltype( int_iter ) >::value )
        );
    EXPECT_EQ(
        ( type_traits::is_forward_iterator_category< TypeParam >::value ),
        ( type_traits::is_forward_iterator< decltype( int_iter ) >::value )
        );
    EXPECT_EQ(
        ( type_traits::is_bidirectional_iterator_category< TypeParam >::value ),
        ( type_traits::is_bidirectional_iterator< decltype( int_iter ) >::value )
        );
    EXPECT_EQ(
        ( type_traits::is_random_access_iterator_category< TypeParam >::value ),
        ( type_traits::is_random_access_iterator< decltype( int_iter ) >::value )
        );
        
}

TEST( IteratorBaseTest, InputBaseTest ){

    TestIterator<int,std::input_iterator_tag> begin( 0 );
    TestIterator<int,std::input_iterator_tag> end( 50 );
    
    auto it = begin;
    for( int cur = 0 ; it != end ; ++cur, ++it ){
        EXPECT_EQ( cur, *it );
    }

}

TEST( IteratorBaseTest, ForwardBaseTest ){

    TestIterator<int,std::forward_iterator_tag> begin( 0 );
    TestIterator<int,std::forward_iterator_tag> end( 50 );

    auto it = begin;
    for( int cur = 0 ; it != end ; ++cur, ++it ){
        EXPECT_EQ( cur, *it );
    }
}

TEST( IteratorBaseTest, BidirectionalBaseTest ){

    TestIterator<int,std::bidirectional_iterator_tag> begin( 0 );
    TestIterator<int,std::bidirectional_iterator_tag> end( 50 );

    auto it = begin;
    for( int cur = 0 ; it != end ; ++cur, ++it ){
        EXPECT_EQ( cur, *it );
    }
    it = end;
    for( int cur = 50 ; it != begin ; --cur, --it ){
        EXPECT_EQ( cur, *it );
    }
}

TEST( IteratorBaseTest, RandomAccessBaseTest ){

    using iterator = TestIterator<int,std::random_access_iterator_tag>;

    TestIterator<int,std::random_access_iterator_tag> begin( 0 );
    TestIterator<int,std::random_access_iterator_tag> end( 50 );

    auto it = begin;
    for( int cur = 0 ; it != end ; ++cur, ++it ){
        EXPECT_EQ( cur, *it );
    }
    it = end;
    for( int cur = 50 ; it != begin ; --cur, --it ){
        EXPECT_EQ( cur, *it );
    }
    EXPECT_EQ( 50, end - begin );
    for( int cur = 0 ; cur < 50 ; ++cur ){
        EXPECT_EQ( iterator( cur ),
            begin + cur );
        EXPECT_EQ( iterator( 50 - cur ), end - cur );
        iterator it = begin;
        it += cur;
        EXPECT_EQ( iterator( cur ), it );
        it = iterator( end );
        it -= cur;
        EXPECT_EQ( iterator( 50 - cur ), it );
        EXPECT_EQ( cur, begin[ cur ] );
        EXPECT_EQ( 50 - cur, end[ -cur ] ); 
    }
}
