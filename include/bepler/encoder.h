#ifndef INCLUDED_BEPLER_ENCODING_H
#define INCLUDED_BEPLER_ENCODING_H

#include <cstdlib>
#include <iterator>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>

template<
    typename T,
    typename EncodeF,
    typename DecodeF,
    EncodeF encode_f = EncodeF(),
    DecodeF decode_f = DecodeF(),
    typename E = unsigned
>
class Encoded{
    E val_;
    public:
        Encoded() : val_() { }
        Encoded( const Encoded& rhs ) : val_ ( rhs.val_ ) { }
        Encoded( const T& rhs ) : val_( encode_f( rhs ) ) { }
        inline operator E() const { return val_; }
        inline T decode() const{ return decode_f( val_ ); }
        inline Encoded& operator=( const T& rhs ){ val_ = encode_f( rhs ); }
        inline bool operator==( const T& rhs ) const{ return decode_f( val_ ) == rhs; }
        inline bool operator!=( const T& rhs ) const{ return decode_f( val_ ) != rhs; }
};

template< typename A, typename B, typename C, B b, C c, typename D >
std::istream& operator>>( std::istream& in, Encoded<A,B,C,b,c,D>& rhs ){
    A raw;
    in >> raw;
    rhs = raw;
    return in;
}

template< typename A, typename B, typename C, B b, C c, typename D >
inline std::ostream& operator<<( std::ostream& out, const Encoded<A,B,C,b,c,D>& rhs ){
    return out << rhs.decode();
}



/*
* Struct defining an interface for generating an encoding using the curiously recurring template pattern.
* @author Tristan Bepler
*/
template<
    typename Derived,
    typename SourceType,
    typename EncodedType,
    typename SourceReferenceType = SourceType&,
    typename EncodedReferenceType = EncodedType&,
    typename ConstSourceReferenceType = const SourceType&,
    typename ConstEncodedReferenceType = const EncodedType&
>
struct EncoderBase{

    typedef SourceType source_type;
    typedef EncodedType encoded_type;
    typedef SourceReferenceType source_reference_type;
    typedef EncodedReferenceType encoded_reference_type;
    typedef ConstSourceReferenceType const_source_reference_type;
    typedef ConstEncodedReferenceType const_encoded_reference_type;

    inline encoded_reference_type encode( const source_type& source ){
        return derived()->encode_f( source );
    }

    inline const_encoded_reference_type encode( const source_type& source ) const{
        encoded_reference_type const e = derived()->encode_f( source );
        return e;
    }

    template< typename InputIterator, typename OutputIterator >
    void encode( InputIterator begin, InputIterator end, OutputIterator out ){
        for( InputIterator it = begin ; it != end ; ++it ){
            *out = encode( *it );
            ++out;
        }
    }

    template< typename InputIterator, typename OutputIterator >
    void encode( InputIterator begin, InputIterator end, OutputIterator out ) const{
        for( InputIterator it = begin ; it != end ; ++it ){
            *out = encode( *it );
            ++out;
        }
    }

    inline source_reference_type decode( const encoded_type& encoded ){
        return derived()->decode_f( encoded );
    }

    inline const_source_reference_type decode( const encoded_type& encoded ) const{
        return derived()->decode_f( encoded );
    }

    template< typename InputIterator, typename OutputIterator >
    void decode( InputIterator begin, InputIterator end, OutputIterator out ){
        for( InputIterator it = begin ; it != end ; ++it ){
            *out = decode( *it );
            ++out;
        }
    }

    template< typename InputIterator, typename OutputIterator >
    void decode( InputIterator begin, InputIterator end, OutputIterator out ) const{
        for( InputIterator it = begin ; it != end ; ++it ){
            *out = decode( *it );
            ++out;
        }
    }

    inline std::size_t size( ) const{
        return derived()->size_f();
    }

    inline bool empty( ) const{
        return size( ) == 0;
    }

    const encoded_reference_type operator[] ( const source_type& source ){
        return encode( source );
    }

    const const_encoded_reference_type operator[] ( const source_type& source ) const{
        return encode( source );
    }

    const encoded_reference_type operator() ( const source_type& source ){
        return encode( source );
    }

    const const_encoded_reference_type operator() ( const source_type& source ) const{
        return encode( source );
    }

    template< typename InputIterator, typename OutputIterator >
    void operator() ( InputIterator begin, InputIterator end, OutputIterator out ){
        return encode( begin, end, out );
    }

    template< typename InputIterator, typename OutputIterator >
    void operator() ( InputIterator begin, InputIterator end, OutputIterator out ) const{
        return encode( begin, end, out );
    }

    private:
        inline Derived* derived( ){
            return static_cast<Derived*>( this );
        }
        inline const Derived* derived( ) const{
            return static_cast<const Derived*>( this );
        }

}; //struct EncoderBase

/*
* Class defining an encoder for a set alphabet
* @author Tristan Bepler
*/
template< typename T, typename E = unsigned int, typename Map = std::unordered_map<T,E> >
class SetEncoder : public EncoderBase<
    SetEncoder< T, E, Map >,
    T,
    E,
    const T&,
    const E&
>{

    friend class EncoderBase< SetEncoder< T, E, Map >, T, E, const T&, const E& >;
    typedef EncoderBase< SetEncoder< T, E, Map >, T, E > base;

    inline bool inBounds( const E& elem ) const{
        return elem >= 0 && elem < size_ ;
    }

    inline const E& encode_f( const T& source ){
        if( !contains( source ) ){
            //add source
            insert( source );
        }
        return code_.at( source );
    }

    inline const E& encode_f( const T& source ) const{
        return code_.at( source );
    }

    inline T& decode_f( const E& encoded ){
        if( inBounds( encoded ) ){
            return alphabet_[ encoded ];
        }
        std::stringstream ss;
        ss << "Index out of range: " << encoded;
        throw std::out_of_range( ss.str() );
    }

    inline const T& decode_f( const E& encoded ) const{
        if( inBounds( encoded ) ){
            return alphabet_[ encoded ];
        }
        std::stringstream ss;
        ss << "Index out of range: " << encoded;
        throw std::out_of_range( ss.str() );
    }

    inline std::size_t size_f() const{
        return size_;
    }

    public:

        SetEncoder( ) : size_( 0 ) { }
        
        template< typename InputIterator >
        SetEncoder( InputIterator begin, InputIterator end ) : size_( 0 ) {
            for( InputIterator it = begin ; it != end ; ++it ){
                insert( *it );
            }
        }

        ~SetEncoder( ){ }

        inline bool contains( const T& source ) const{
            return code_.count( source );
        }

        SetEncoder& insert( const T& source ){
            if( !contains( source ) ){
                //add encoding for source
                alphabet_.push_back( source );
                code_[ source ] = size_;
                ++size_;
            }
            return *this;
        }

        SetEncoder& clear( ){
            code_.clear();
            alphabet_.clear();
            size_ = 0;
            return *this;
        }

    protected:
        Map code_;
        std::vector<T> alphabet_;
        std::size_t size_;

}; //class SetEncoder

/*
* Class defining a special case encoder in which the alphabet is a range defined by some min and max elements
* @author Tristan Bepler
*/
template< typename SourceType, typename EncodedType = unsigned int >
class RangeEncoder : public EncoderBase<
    RangeEncoder< SourceType, EncodedType >,
    SourceType,
    EncodedType,
    SourceType,
    EncodedType
>{

    friend class EncoderBase< RangeEncoder, SourceType, EncodedType,
        SourceType, EncodedType >;
    typedef EncoderBase< RangeEncoder, SourceType, EncodedType,
        SourceType, EncodedType > base;

    inline bool sourceInBounds( const SourceType& source ) const{
        return source >= min_ && source < max_; 
    }

    inline bool encodedInBounds( const EncodedType& encoded ) const{
        return encoded >= 0 && encoded < base::size();
    }

    inline EncodedType encode_f( const SourceType& source ) const{
        if( sourceInBounds( source ) ){
            return source - min_;
        }
        std::stringstream ss;
        ss << "Source out of range: " << source;
        throw std::out_of_range( ss.str() );
    }

    inline SourceType decode_f( const EncodedType& encoded ) const{
        if( encodedInBounds( encoded ) ){
            return min_ + encoded;
        }
        std::stringstream ss;
        ss << "Index out of range: " << encoded;
        throw std::out_of_range( ss.str() );
    }

    inline std::size_t size_f() const{
        return max_ - min_;
    }

    public:
        RangeEncoder( ) { }
        RangeEncoder( const SourceType& mi, const SourceType& ma )
            : min_( mi ),
            max_( ma )
            { }

        const SourceType& min() const{
            return min_;
        }

        void min( const SourceType& m ){
            min_ = m;
        }

        const SourceType& max() const{
            return max_;
        }

        void max( const SourceType& m ){
            max_ = m;
        }
    
    protected:
        SourceType min_;
        SourceType max_;
        
    
}; //class RangeEncoder

#endif
