#ifndef INCLUDED_BEPLER_BENCHMARK_H
#define INCLUDED_BEPLER_BENCHMARK_H

namespace benchmark{

    std::size_t rdtsc(){
        unsigned int lo, hi;
        __asm__ __volatile__( "rdtsc" : "=a" ( lo ), "=d" ( hi ) );
        return ( ( std::size_t ) hi << 32 ) | lo ;
    }

    template< typename Function, typename ... T >
    std::size_t time( Function f, T... args ){
        std::size_t time = rdtsc();
        f( &args... );
        return rdtsc() - time;
    }

    template< typename Function, typename ... T >
    std::size_t timeIters( std::size_t iters, Function f, T... args ){
        std::size_t time = rdtsc();
        for( std::size_t i = 0 ; i < iters ; ++i ){
            f( &args... );
        }
        return ( rdtsc() - time ) / iters ;
    }

} //namespace benchmark

#endif
