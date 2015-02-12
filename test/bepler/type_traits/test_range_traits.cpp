#include "gtest/gtest.h"
#include "bepler/type_traits/range_traits.h"

#include <vector>

using namespace type_traits;

struct Stub{

};

struct NotRangeStub{

    Stub begin() const{
        return Stub();
    }

    Stub end() const{
        return Stub();
    }
};

struct RangeStub{

    int* begin(){
        return NULL;
    }

    int* end(){
        return NULL;
    }

};

struct ConstRangeStub{

    const int* begin() const{
        return NULL;
    }

    const int* end() const{
        return NULL;
    }

};


TEST( RangeTraitsTest, HasBegin ){
    EXPECT_TRUE( has_begin<RangeStub>::value );
    EXPECT_FALSE( has_begin<const RangeStub>::value );
    EXPECT_TRUE( has_begin<ConstRangeStub>::value );
    EXPECT_TRUE( has_begin<const ConstRangeStub>::value );
    EXPECT_TRUE( has_begin< std::vector<bool> >::value );
    EXPECT_TRUE( has_begin< const std::vector<bool> >::value );
    EXPECT_FALSE( has_begin< Stub > :: value );
    EXPECT_FALSE( has_begin< const Stub > :: value );
    EXPECT_TRUE( has_begin< NotRangeStub >::value );
    EXPECT_TRUE( has_begin< const NotRangeStub >::value );
}

TEST( RangeTraitsTest, HasEnd ){
    EXPECT_TRUE( has_end<RangeStub>::value );
    EXPECT_FALSE( has_end<const RangeStub>::value );
    EXPECT_TRUE( has_end<ConstRangeStub>::value );
    EXPECT_TRUE( has_end<const ConstRangeStub>::value );
    EXPECT_TRUE( has_end< std::vector<bool> >::value );
    EXPECT_TRUE( has_end< const std::vector<bool> >::value );
    EXPECT_FALSE( has_end< Stub > :: value );
    EXPECT_FALSE( has_end< const Stub > :: value );
    EXPECT_TRUE( has_end< NotRangeStub >::value );
    EXPECT_TRUE( has_end< const NotRangeStub >::value );
}

TEST( RangeTraitsTest, IsRange ){
    EXPECT_TRUE( is_range<RangeStub>::value );
    EXPECT_FALSE( is_const_range<RangeStub>::value );
    EXPECT_FALSE( is_range< const RangeStub>::value );
    EXPECT_TRUE( is_range<ConstRangeStub>::value );
    EXPECT_TRUE( is_const_range<ConstRangeStub>::value );
    EXPECT_TRUE( is_range< const ConstRangeStub>::value );
    EXPECT_FALSE( is_range<Stub>::value );
    EXPECT_FALSE( is_const_range<Stub>::value );
    EXPECT_FALSE( is_range< const Stub>::value );
    EXPECT_TRUE( is_range<std::vector<bool> >::value );
    EXPECT_TRUE( is_const_range<std::vector<bool> >::value );
    EXPECT_TRUE( is_range< const std::vector<bool> >::value );
    EXPECT_FALSE( is_range< NotRangeStub >::value );
    EXPECT_FALSE( is_const_range< NotRangeStub >::value );
    EXPECT_FALSE( is_range< const NotRangeStub >::value );
}
