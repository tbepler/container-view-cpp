#include "gtest/gtest.h"
#include "bepler/encoder.h"

#include <stdexcept>

class SetEncoderTest : public ::testing::Test{

    protected:
        virtual void SetUp( ){

        }

        SetEncoder<std::size_t> empty_encoder;
        const SetEncoder<std::size_t> const_empty_encoder;
        SetEncoder<char, unsigned char> dna_encoder;
        SetEncoder<int> int_encoder;

};

TEST_F( SetEncoderTest, InitializationTest ){

    EXPECT_EQ( 0, empty_encoder.size() );
    EXPECT_TRUE( empty_encoder.empty() );

}

TEST_F( SetEncoderTest, EncodeTest ){

    const char* dna_seq = "ACGAATTCGA";
    unsigned char dna_seq_encoded[ 10 ];

    //const encoder should throw when element is not found
    EXPECT_THROW( const_empty_encoder.encode( 0 ), std::out_of_range );
    EXPECT_THROW( const_empty_encoder.encode( dna_seq, dna_seq + 10, dna_seq_encoded ), std::out_of_range );
    EXPECT_THROW( const_empty_encoder[ 0 ], std::out_of_range );
    EXPECT_THROW( const_empty_encoder( 0 ), std::out_of_range );
    EXPECT_THROW( const_empty_encoder( dna_seq, dna_seq + 10, dna_seq_encoded ), std::out_of_range );

}

