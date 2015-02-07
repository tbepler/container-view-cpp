#include "gtest/gtest.h"
#include "bepler/itertools/generator.h"

#include "bepler/itertools/range.h"
#include <iterator>
#include <utility>

#include <iostream>

using namespace itertools;

template< typename T >
struct Identity{
    inline T operator()( const T& obj ) const{ return obj; }
    inline bool operator==( const Identity& b ) const{ return true; }
    inline bool operator!=( const Identity& b ) const{ return false; }
};

template< typename T > T rvalue();

template<
    typename InputIterator,
    typename Function = Identity< typename std::iterator_traits<InputIterator>::value_type >
>
class Mapper : public Generator<
    Mapper<InputIterator,Function>,
    decltype( rvalue<Function>()( *(rvalue<InputIterator>()) ) )
>{
    typedef decltype( std::declval<Function>()( std::declval< typename std::iterator_traits<InputIterator>::value_type >() ) ) return_t;
    InputIterator cur_;
    InputIterator end_;
    Function f_;
    public:
        Mapper() { }
        Mapper( InputIterator begin, InputIterator end )
            : cur_( begin ), end_( end ), f_() { }
        Mapper( InputIterator begin, InputIterator end, Function f)
            : cur_( begin ), end_( end ), f_( f ) { }
        inline bool done() const { return cur_ == end_; }
        inline bool next() { ++cur_; return !done(); }
        inline return_t get() const { return f_(*cur_); }

};

template< typename InputIterator, typename Function >
Mapper<InputIterator,Function> map( Function f, InputIterator begin, InputIterator end ){
    return Mapper<InputIterator,Function>( begin, end, f );
}

template< typename Container, typename Function >
Mapper< decltype( std::begin( std::declval<const Container>() ) ), Function >
map( Function f, const Container& c ){
    return Mapper< decltype( std::begin(c) ), Function >( std::begin(c), std::end(c), f );
}

template<
    typename InputIterator,
    typename Function = Identity< typename std::iterator_traits<InputIterator>::value_type >
>
class MapIterator : public InputIteratorBase<
    MapIterator<InputIterator,Function>,
    decltype( rvalue<Function>()( *(rvalue<InputIterator>()) )),
    decltype( rvalue<Function>()( *(rvalue<InputIterator>()) ))
>{
    typedef decltype( std::declval<Function>()( std::declval< typename std::iterator_traits<InputIterator>::value_type >() ) ) return_t;
    InputIterator pos_;
    Function f_;
    public:
        MapIterator() { }
        MapIterator( InputIterator pos, Function f = Function() )
            : pos_( pos ), f_( f ) { }
        inline bool equals( const MapIterator& b ) const { return pos_ == b.pos_; }
        inline return_t dereference() const{ return f_( *pos_ ); }
        inline return_t* arrow() const{ return &f_( *pos_ ); }
        inline void inc(){ ++pos_; }

};

template< typename T >
class Iterable{
    T begin_;
    T end_;
    public:
        Iterable( const T& b, const T& e ) : begin_( b ), end_( e ) { }
        inline T begin() const { return begin_; }
        inline T end() const { return end_; }
};

template< typename T >
inline T begin( const Iterable<T>& c ){
    std::cout << "begin called." << std::endl;
    return c.begin();
}

template< typename T >
inline T end( const Iterable<T>& c ){
    std::cout << "end called." << std::endl;
    return c.end();
}

template< typename InputIterator, typename Function >
Iterable< MapIterator<InputIterator,Function> >
mapIterator( Function f, InputIterator begin, InputIterator end ){
    return Iterable< MapIterator<InputIterator,Function> >( MapIterator<InputIterator,Function>(begin,f), MapIterator<InputIterator,Function>(end,f) );
}

template< typename Container, typename Function >
Iterable< MapIterator< decltype( std::begin( std::declval<const Container>() ) ), Function > >
mapIterator( Function f, const Container& c ){
    return Iterable< MapIterator< decltype( std::begin(c) ), Function > >( MapIterator<decltype(std::begin(c)),Function>(std::begin(c),f), MapIterator<decltype(std::begin(c)),Function>( std::end(c), f ) );
}

template< typename T >
struct Adder{
    T val;
    Adder( ) : val( ) { }
    Adder( const T& v ) : val( v ) { }
    inline T operator()( const T& t ) const{ return t + val; }
    inline bool operator==( const Adder& b ) const{ return val == b.val; }
    inline bool operator!=( const Adder& b ) const{ return val != b.val; }
};

template< typename T >
Adder<T> add( T val ){ return Adder<T>( val ); }

template< typename T >
T identity( const T& t ){ return t; }

std::size_t rdtsc(){
    unsigned int lo, hi;
    __asm__ __volatile__( "rdtsc" : "=a" ( lo ), "=d" ( hi ) );
    return ((std::size_t) hi << 32) | lo;
}

struct Point{
    int x;
    int y;
    Point() : x(), y() { }
    Point( int xv, int yv ) : x(xv), y(yv) { }
};

Point operator+( const Point& p, int i ){
    Point cpy( p );
    cpy.x += i;
    cpy.y += i;
    return cpy;
}

Point operator+( int i, const Point& p ){
    return p + i;
}

Point operator+( const Point& a, const Point& b){
    Point cpy( a );
    cpy.x += b.x;
    cpy.y += b.y;
    return cpy;
}

bool operator==( const Point& a, const Point& b){
    return a.x == b.x && a.y == b.y;
}

bool operator!=( const Point& a, const Point& b ){
    return !( a == b );
}

TEST( GeneratorTest, TestBaseClass ){
    
    int array[] = { 1, 2, 3, 4, 5, 6, 7 };
    int num_iters = 0;
    auto it = std::begin( array );
    for( int i : map( Identity<int>(), array ) ){
        EXPECT_EQ( *it, i );
        ++it;
        ++num_iters;
    }
    EXPECT_EQ( 7, num_iters );

    num_iters = 0;
    it = std::begin( array );
    for( int i : map( &identity<int>, array ) ){
        EXPECT_EQ( *it, i );
        ++it;
        ++num_iters;
    }
    EXPECT_EQ( 7, num_iters );

    num_iters = 0;
    it = std::begin( array );
    for( int i : map( add( 5 ), array ) ){
        EXPECT_EQ( *it + 5, i );
        ++it;
        ++num_iters;
    }
    EXPECT_EQ( 7, num_iters );

    num_iters = 0;
    it = std::begin( array );
    for( int i : mapIterator( add( 5 ), array ) ){
        EXPECT_EQ( *it + 5, i );
        ++it;
        ++num_iters;
    }
    EXPECT_EQ( 7, num_iters );

    Point points[10];
    num_iters = 0;
    auto ait = std::begin( points );
    auto xform = mapIterator( add( Point(2,5) ), points );
    for( auto xit = xform.begin() ; xit != xform.end() ; ++xit ){
        EXPECT_EQ( 2, xit->x );
        EXPECT_EQ( 5, xit->y );
        EXPECT_EQ( *ait + Point(2,5), *xit );
        ++ait;
        ++num_iters;
    }
    EXPECT_EQ( 10, num_iters );

    unsigned long min = 0;
    unsigned long max = 5000000;
    unsigned long step = 1;
    const static unsigned long iters = 5000000;
    const static auto add5 = add( 5 );

    unsigned long sum = 0;
    std::size_t time = rdtsc();
    for( unsigned long i = min ; i < max ; i += step ){
        //sum += add5( i );
        sum += i + 5;
    }
    time = rdtsc() - time;
    
    EXPECT_EQ( ( max - min - 1 ) * iters / 2 + 5 * iters, sum ); 
    std::cout << "For-loop:" << std::endl;
    std::cout << "Total: " << time << std::endl;
    std::cout << "Per iter: " << time / iters << std::endl;

    sum = 0;
    //auto generator = map( add5, range( min, max, step ) );
    time = rdtsc();
    for( auto i : map( add5, range( min, max, step ) ) ){
        sum += i;
    }
    time = rdtsc() - time;

    EXPECT_EQ( ( max - min - 1 ) * iters / 2 + 5 * iters, sum ); 
    std::cout << "For-each map over range:" << std::endl;
    std::cout << "Total: " << time << std::endl;
    std::cout << "Per iter: " << time / iters << std::endl;
    
    sum = 0;
    //generator = map( add5, range( min, max, step ) );
    time = rdtsc();
    auto generator = map( add5, range( min, max, step ) );
    while( !generator.done() ){
        sum += generator.get();
        generator.next();
    }
    time = rdtsc() - time;

    EXPECT_EQ( ( max - min - 1 ) * iters / 2 + 5 * iters, sum ); 
    std::cout << "While-loop map over range:" << std::endl;
    std::cout << "Total: " << time << std::endl;
    std::cout << "Per iter: " << time / iters << std::endl;
    
    sum = 0;
    auto map_iterable = mapIterator( add5, range( min, max, step ) );
    time = rdtsc();
    for( auto i : map_iterable ){
        sum += i;
    }
    time = rdtsc() - time;

    EXPECT_EQ( ( max - min - 1 ) * iters / 2 + 5 * iters, sum ); 
    std::cout << "For-each map iterator over range:" << std::endl;
    std::cout << "Total: " << time << std::endl;
    std::cout << "Per iter: " << time / iters << std::endl;

    sum = 0;
    time = rdtsc();
    //map_iterable = mapIterator( add5, range( min, max, step ) );
    for( auto it =  map_iterable.begin() ; it != map_iterable.end() ; ++it ){
        sum += *it;
    }
    time = rdtsc() - time;

    EXPECT_EQ( ( max - min - 1 ) * iters / 2 + 5 * iters, sum ); 
    std::cout << "For map iterator over range:" << std::endl;
    std::cout << "Total: " << time << std::endl;
    std::cout << "Per iter: " << time / iters << std::endl;
}
