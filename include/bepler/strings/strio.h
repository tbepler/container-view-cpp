#ifndef INCLUDED_BEPLER_STRINGS_STRIO_H
#define INCLUDED_BEPLER_STRINGS_STRIO_H

#include <string>
#include <cstdarg>
#include <iostream>

namespace string{

    using std::string;

    string sprintf( const string& format, ... );
    string sprintf( const string& format, std::va_list varargs );

    std::ostream& fprintf( std::ostream& out, const string& format, ... );
    std::ostream& fprintf( std::ostream& out, const string& format, std::va_list varargs );

    void printf( const string& format, ... );
    void printf( const string& format, std::va_list varargs );


}

#endif
