#include "bepler/strings/strformat.h"

#include <cstdio>

const static std::size_t BUFFER_SIZE = 4096;

using std::string;

namespace strings{

    string sprintf( const string& format, ... ){
        std::va_list varargs;
        va_start( varargs, format );
        string str = sprintf( format, varargs );
        va_end( varargs );
        return str;
    }

    string sprintf( const string& format, std::va_list varargs ){
        string str;
        char buffer[ BUFFER_SIZE ];
        int size = std::vsnprintf( buffer, BUFFER_SIZE, format.c_str(), varargs );
        if( size < 0 ){
            //an encoding error occurred, return empty string
            return str;
        }
        if( size > BUFFER_SIZE ){
            //char array was not large enough, so allocate sufficiently large
            //char array on the heap and use it instead
            char* heap_buffer = new char[ size ];
            if( std::vsnprintf( heap_buffer, size, format.c_str(), varargs ) < 0 ){
                //encoding error occurred, return empty string
                delete heap_buffer;
                return str;
            }
            str = heap_buffer;
            delete heap_buffer;
        }else{
            str = buffer;
        }
        return str;
    }

    std::ostream& fprintf( std::ostream& out, const string& format, ... ){
        std::va_list varargs;
        va_start( varargs, format );
        fprintf( out, format, varargs );
        va_end( varargs );
        return out;
    }

    std::ostream& fprintf( std::ostream& out, const string& format, std::va_list varargs ){
        return out << sprintf( format, varargs );
    }

    void printf( const string& format, ... ){
        std::va_list varargs;
        va_start( varargs, format );
        printf( format, varargs );
        va_end( varargs );
    }

    void printf( const string& format, std::va_list varargs ){
        fprintf( std::cout, format, varargs );
    }

}
