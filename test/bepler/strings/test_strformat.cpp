#include "gtest/gtest.h"
#include "bepler/strings/strformat.h"

#include <sstream>

TEST( StringFormattingTest, SprintfFormatting ){

    EXPECT_EQ( "test\n", strings::sprintf( "%s\n", "test" ) );
    EXPECT_EQ( "13 != 52", strings::sprintf( "%d != %d", 13, 52 ) );
    EXPECT_EQ( "float: -1.23", strings::sprintf( "%s%.2f", "float: ", -1.23 ) );
    EXPECT_EQ( "%3.9265E+02", strings::sprintf( "%%%.4E", 392.65 ) );

}

TEST( StringFormattingTest, FprintfFormatting ){

    std::stringstream ss;
    strings::fprintf( ss, "%s\n", "test" );
    EXPECT_EQ( "test\n", ss.str() );

    strings::fprintf( ss, "%d != %d", 13, 52 );
    EXPECT_EQ( "test\n13 != 52", ss.str() );

    ss.str( "" );
    ss.clear();
    strings::fprintf( ss, "%s%.2f", "float: ", -1.23 ) << "\n";
    EXPECT_EQ( "float: -1.23\n", ss.str() );

    ss.str( "" );
    ss.clear();
    strings::fprintf( ss, "%%%.4E", 392.65 );
    EXPECT_EQ( "%3.9265E+02", ss.str() );

}
