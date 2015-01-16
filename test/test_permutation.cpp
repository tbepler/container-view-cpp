#include "gtest/gtest.h"
#include "permutation.h"

const static std::size_t A0 = 4, L0 = 3, A1 = 5, L1 = 5, A2 = 2, L2 = 10;
const static std::size_t S0 = 4*4*4, S1 = 5*5*5*5*5, S2 = 2*2*2*2*2*2*2*2*2*2;

class PermutationTest : public ::testing::Test {

    public:
        PermutationTest( )
            :p0( A0, L0 ), p1( A1, L1 ), p2( A2, L2 ) { }

    protected:
        virtual void SetUp( ){
            
        }

    Permutation p0;
    Permutation p1;
    Permutation p2;

};

TEST_F( PermutationTest, InitializationWorks ){
    EXPECT_EQ( A0, p0.alphabet() );
    EXPECT_EQ( L0, p0.length() );
    EXPECT_EQ( A1, p1.alphabet() );
    EXPECT_EQ( L1, p1.length() );
    EXPECT_EQ( A2, p2.alphabet() );
    EXPECT_EQ( L2, p2.length() );
}

TEST_F( PermutationTest, SizeWorks ){
    EXPECT_EQ( S0, p0.size() );
    EXPECT_EQ( S1, p1.size() );
    EXPECT_EQ( S2, p2.size() );
}
