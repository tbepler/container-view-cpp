#include "gtest/gtest.h"
#include "bepler/strings/strio.h"

TEST( StringIOTest, SprintfFormatting ){

    EXPECT_EQ( "test\n", string::sprintf( "%s\n", "test" ) );
    EXPECT_EQ( "13 != 52", string::sprintf( "%d != %d", 13, 52 ) );
    EXPECT_EQ( "float: -1.23", string::sprintf( "%s%.2f", "float: ", -1.23 ) );
    EXPECT_EQ( "%3.9265E+02", string::sprintf( "%%%.4E", 392.65 ) );

}
