#include "bepler/benchmark.h"

namespace benchmark{

    std::size_t rdtsc(){
        unsigned int lo, hi;
        __asm__ __volatile__( "rdtsc" : "=a" ( lo ), "=d" ( hi ) );
        return ( ( std::size_t ) hi << 32 ) | lo ;
    }
    

} //namespace benchmark
