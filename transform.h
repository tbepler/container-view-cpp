#ifndef INCLUDE_TRANSFORM_H_
#define INCLUDE_TRANSFORM_H_

#include <cstddef>
#include <type_traits>

/**
* Provides a view of the specified container mapped through the transformation * function
*
* @author Tristan Bepler
**/

template< typename C, typename F >
class Transform{

    public:
        Transform( ) { }
        Transform( C& container, const F& function )
            : container( &container ), function( &function ) { }
        ~Transform( ) { }

        typename std::result_of<F>::type operator[]( std::size_t i ) const{
            return function( (*container)[i] );
        }

        typename std::result_of<F>::type operator[]( std::size_t i ) {
            return function( (*container)[i] );
        }

        C operator() ( ) const {
            C trans = (*container);
            for( typename C::iterator it = trans.begin() ; it != trans.end() ; ++it ){
                *it = function( (*it) );
            }
            return trans;
        }

    private:
        C* container;
        F function;

};

#endif
