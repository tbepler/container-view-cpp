#include "gtest/gtest.h"
#include "bepler/strings/strops.h"

#include <iostream>

using namespace strings;
using std::string;

struct TestStruct{
    int a;
    double b;
    string c;

    TestStruct( int a, double b, string c )
        : a( a ), b( b ), c( c ) { }

};

std::ostream& operator<< ( std::ostream& out, const TestStruct& obj ){
    return out << obj.a << ", " << obj.b << ", " << obj.c;
}

TEST( StringOpsTest, ToString ){

    EXPECT_EQ( "1", toString( 1 ) );
    EXPECT_EQ( "-53", toString( -53 ) );
    EXPECT_EQ( "1.7801", toString( 1.7801 ) );
    EXPECT_EQ( "string", toString( "string" ) );
    TestStruct obj( 5, -37.33, "string_here" ); 
    EXPECT_EQ( "5, -37.33, string_here", toString( obj ) );

}

TEST( StringOpsTest, AppendOperator ){

    string s;
    s += 42;
    EXPECT_EQ( "42", s );
    double pi = 3.1415;
    s += ", ";
    s += pi;
    EXPECT_EQ( "42, 3.1415", s );

    TestStruct obj( 5, -2.11, "foobar" );
    s += obj;
    EXPECT_EQ( "42, 3.14155, -2.11, foobar", s );

}

TEST( StringOpsTest, PlusOperator ){

    string a = "left";
    int b = 42;
    EXPECT_EQ( "left42", a + b );
    EXPECT_EQ( "42left", b + a );

    TestStruct obj( -1, 5.5, "!!!" );
    EXPECT_EQ( "left-1, 5.5, !!!", a + obj );
    EXPECT_EQ( "-1, 5.5, !!!left", obj + a );

}

