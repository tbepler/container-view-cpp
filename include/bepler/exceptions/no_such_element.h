#ifndef INCLUDED_BEPLER_EXCEPTIONS_NO_SUCH_ELEMENT_H
#define INCLUDED_BEPLER_EXCEPTIONS_NO_SUCH_ELEMENT_H

#include <stdexcept>
#include "bepler/strings/strops.h"

namespace exceptions{

    class NoSuchElementError : public std::runtime_error{
        public:
            explicit NoSuchElementError( const std::string& what_arg )
                : runtime_error( what_arg ) { }
            explicit NoSuchElementError( const char* what_arg )
                : runtime_error( what_arg ) { }
            template< typename T >
            explicit NoSuchElementError( const T& elem )
                : runtime_error( "Element " + elem + " not found." ) { }
    }; //class NoSuchElementError

} //namespace exceptions

#endif
