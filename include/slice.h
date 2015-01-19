#ifndef INCLUDE_SLICE_H_
#define INCLUDE_SLICE_H_

#include <cstddef>

template< typename C, typename E = typename C::value_type >
class Slice{

    public:
        Slice( ) { }
        Slice( C& m_container, std::size_t m_start, std::size_t m_size )
            : m_container( &m_container ), m_start( m_start ), m_size( m_size ) { }
        Slice( const Slice& copy )
            : m_container( copy.m_container ), m_start( copy.m_start ), m_size( copy.m_size ) { }
        ~Slice( ) { }

        inline E& operator[] ( std::size_t i ){
            return (*m_container)[ m_start + i ];
        }

        inline const E& operator[] ( std::size_t i ) const{
            return (*m_container)[ m_start + i ];
        }

        inline Slice& operator++( ){
            ++m_start;
            return *this;
        }

        inline Slice& operator--( ){
            --m_start;
            return *this;
        }

        inline Slice operator++(int){
            Slice s( *this );
            ++(*this);
            return s;
        }

        inline Slice operator--(int){
            Slice s( *this );
            --(*this);
            return s;
        }

        inline Slice& operator+= ( std::size_t rhs ){
            m_start += rhs;
            return *this;
        }

        inline Slice& operator-= ( std::size_t rhs ){
            m_start -= rhs;
            return *this;
        }

        friend inline Slice operator+ ( const Slice& a, std::size_t b ){
            Slice<C,E> s( *a.m_container, a.m_start + b, a.m_size );
            return s;
        }

        friend inline Slice operator+ ( std::size_t a, const Slice& b ){
            return b + a;
        }

        friend inline Slice operator- ( const Slice& a, std::size_t b ){
            Slice<C,E> s( *a.m_container, a.m_start - b, a.m_size );
            return s;
        }

        inline std::size_t size( ) const { return m_size; }
        inline void size( std::size_t m_size ) { this->m_size = m_size; }

        inline std::size_t start( ) const { return m_start; }
        inline void start( std::size_t m_start ) { this->m_start = m_start; }

        inline std::size_t end( ) const { return m_start + m_size; }

    private:
        C* m_container;
        std::size_t m_start;
        std::size_t m_size;

};

//template< typename C, typename E >
//inline Slice<C,E> operator+ ( const Slice<C,E>& a, std::size_t b ){
//    Slice<C,E> s( *a.m_container, a.m_start + b, a.m_size );
//    return s;
//}

//template< typename C, typename E >
//inline Slice<C,E> operator+ ( std::size_t a, const Slice<C,E>& b ){
//    return b + a;
//}

//template< typename C, typename E >
//inline Slice<C,E> operator- ( const Slice<C,E>& a, std::size_t b ){
//    Slice<C,E> s( *a.m_container, a.m_start - b, a.m_size );
//    return s;
//}


#endif
