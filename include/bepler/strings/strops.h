#ifndef INCLUDED_BEPLER_STRINGS_STROPS_H
#define INCLUDED_BEPLER_STRINGS_STROPS_H

#include <string>
#include <sstream>

namespace strings{

    template< typename T >
    std::string toString( const T& obj ){
        std::stringstream ss;
        ss << obj;
        return ss.str();
    }


    template< typename T >
    std::string& operator+= ( std::string& str, const T& obj ){
        return str += toString( obj );
    }

    template< typename T >
    std::string operator+ ( const std::string& str, const T& obj ){
        std::string s( str );
        s += obj;
        return s;
    }

    template< typename T >
    std::string operator+ ( const T& obj, const std::string& str ){
        std::string s( toString( obj ) );
        s += str;
        return s;
    }

}

#endif

