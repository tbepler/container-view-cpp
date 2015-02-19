#ifndef INCLUDED_BEPLER_FUNCTIONAL_GENERATOR_H
#define INCLUDED_BEPLER_FUNCTIONAL_GENERATOR_H

#include <functional>

namespace functional{

    template< typename T >
    using acceptor_f = std::function< void( T ) >;

    template< typename T >
    using generator_f = std::function< void( acceptor_f<T> ) >;


} //namespace functional

#endif
