#ifndef INCLUDED_BEPLER_BENCHMARK_H
#define INCLUDED_BEPLER_BENCHMARK_H

#include <cstdlib>

namespace benchmark{

    std::size_t rdtsc();

    template< typename Function, typename ... T >
    std::size_t time( Function f, T... args ){
        std::size_t time = rdtsc();
        f( &args... );
        return rdtsc() - time;
    }

    template< typename Function, typename ... T >
    double timeIters( std::size_t iters, Function f, T... args ){
        std::size_t time = rdtsc();
        for( std::size_t i = 0 ; i < iters ; ++i ){
            f( &args... );
        }
        return ( rdtsc() - time ) / (double) iters ;
    }

} //namespace benchmark

#endif
