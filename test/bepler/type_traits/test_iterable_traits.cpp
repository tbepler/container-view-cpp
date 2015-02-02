#include "gtest/gtest.h"
#include "bepler/type_traits/iterable_traits.h"

#include <vector>

using namespace type_traits;

struct Stub{

};

struct NotIterableStub{

    Stub begin() const{
        return Stub();
    }

    Stub end() const{
        return Stub();
    }
};

struct IterableStub{

    int* begin(){
        return NULL;
    }

    int* end(){
        return NULL;
    }

};

struct ConstIterableStub{

    const int* begin() const{
        return NULL;
    }

    const int* end() const{
        return NULL;
    }

};


TEST( IterableTraitsTest, HasBegin ){
    EXPECT_TRUE( has_begin<IterableStub>::value );
    EXPECT_FALSE( has_begin<const IterableStub>::value );
    EXPECT_TRUE( has_begin<ConstIterableStub>::value );
    EXPECT_TRUE( has_begin<const ConstIterableStub>::value );
    EXPECT_TRUE( has_begin< std::vector<bool> >::value );
    EXPECT_TRUE( has_begin< const std::vector<bool> >::value );
    EXPECT_FALSE( has_begin< Stub > :: value );
    EXPECT_FALSE( has_begin< const Stub > :: value );
    EXPECT_TRUE( has_begin< NotIterableStub >::value );
    EXPECT_TRUE( has_begin< const NotIterableStub >::value );
}

TEST( IterableTraitsTest, HasEnd ){
    EXPECT_TRUE( has_end<IterableStub>::value );
    EXPECT_FALSE( has_end<const IterableStub>::value );
    EXPECT_TRUE( has_end<ConstIterableStub>::value );
    EXPECT_TRUE( has_end<const ConstIterableStub>::value );
    EXPECT_TRUE( has_end< std::vector<bool> >::value );
    EXPECT_TRUE( has_end< const std::vector<bool> >::value );
    EXPECT_FALSE( has_end< Stub > :: value );
    EXPECT_FALSE( has_end< const Stub > :: value );
    EXPECT_TRUE( has_end< NotIterableStub >::value );
    EXPECT_TRUE( has_end< const NotIterableStub >::value );
}

TEST( IterableTraitsTest, IsIterable ){
    EXPECT_TRUE( is_iterable<IterableStub>::value );
    EXPECT_FALSE( is_const_iterable<IterableStub>::value );
    EXPECT_FALSE( is_iterable< const IterableStub>::value );
    EXPECT_TRUE( is_iterable<ConstIterableStub>::value );
    EXPECT_TRUE( is_const_iterable<ConstIterableStub>::value );
    EXPECT_TRUE( is_iterable< const ConstIterableStub>::value );
    EXPECT_FALSE( is_iterable<Stub>::value );
    EXPECT_FALSE( is_const_iterable<Stub>::value );
    EXPECT_FALSE( is_iterable< const Stub>::value );
    EXPECT_TRUE( is_iterable<std::vector<bool> >::value );
    EXPECT_TRUE( is_const_iterable<std::vector<bool> >::value );
    EXPECT_TRUE( is_iterable< const std::vector<bool> >::value );
    EXPECT_FALSE( is_iterable< NotIterableStub >::value );
    EXPECT_FALSE( is_const_iterable< NotIterableStub >::value );
    EXPECT_FALSE( is_iterable< const NotIterableStub >::value );
}
