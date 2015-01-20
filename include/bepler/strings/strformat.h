#ifndef INCLUDED_BEPLER_STRINGS_STRFORMAT_H
#define INCLUDED_BEPLER_STRINGS_STRFORMAT_H

#include <string>
#include <cstdarg>
#include <iostream>

namespace strings{

    std::string sprintf( const std::string& format, ... );
    std::string sprintf( const std::string& format, std::va_list varargs );

    std::ostream& fprintf( std::ostream& out, const std::string& format, ... );
    std::ostream& fprintf( std::ostream& out, const std::string& format, std::va_list varargs );

    void printf( const std::string& format, ... );
    void printf( const std::string& format, std::va_list varargs );


}

#endif
