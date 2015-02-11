#include "gtest/gtest.h"
#include "bepler/genomics/dna_encoder.h"

using namespace genomics;

TEST( DNAEncoderTest, BaseEncoding ){

    EXPECT_EQ( DNA::A, DNA::encode( 'A' ) );
    EXPECT_EQ( DNA::A, DNA::encode( 'a' ) );
    EXPECT_EQ( DNA::C, DNA::encode( 'C' ) );
    EXPECT_EQ( DNA::C, DNA::encode( 'c' ) );
    EXPECT_EQ( DNA::G, DNA::encode( 'G' ) );
    EXPECT_EQ( DNA::G, DNA::encode( 'g' ) );
    EXPECT_EQ( DNA::T, DNA::encode( 'T' ) );
    EXPECT_EQ( DNA::T, DNA::encode( 't' ) );
    EXPECT_THROW( DNA::encode( 'N' ), std::out_of_range );
    EXPECT_THROW( DNA::encode( 'x' ), std::out_of_range );

}
