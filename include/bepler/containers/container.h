#ifndef INCLUDED_BEPLER_CONTAINERS_CONTAINER_H
#define INCLUDED_BEPLER_CONTAINERS_CONTAINER_H

#include <cstddef>

/**
* Defines structs/classes useful for defining STL compliant containers
*
* @author Tristan Bepler
**/

namespace containers{

    /**
    * Struct defining Container types for general STL compliance
    **/
    template< typename T,
        typename Iterator, typename ConstIterator,
        typename Reference = T&,
        typename ConstReference = const Reference,
        typename DiffType = std::ptrdiff_t,
        typename SizeType = std::size_t >
    struct ContainerTypes{

        typedef T value_type;
        typedef Reference reference;
        typedef ConstReference const_reference;
        typedef Iterator iterator;
        typedef ConstIterator const_iterator;
        typedef DiffType difference_type;
        typedef SizeType size_type;

    }; //struct ContainerTypes

   
    /**
    * Virtual class describing a Container interface for general STL compliance
    **/
    template< typename T,
        typename Iterator, typename ConstIterator,
        typename Reference = T&,
        typename ConstReference = const Reference,
        typename DiffType = std::ptrdiff_t,
        typename SizeType = std::size_t >
    class Container : public ContainerTypes< T, Iterator, ConstIterator,
        Reference, ConstReference, DiffType, SizeType > {

        typedef ContainerTypes< T, Iterator, ConstIterator, Reference, ConstReference, DiffType, SizeType > super;

        public:
            virtual ~Container( );
            virtual Container& operator= ( const Container& );
            virtual void swap( Container& x );
            virtual typename super::iterator begin( );
            virtual typename super::iterator end( );
            virtual typename super::const_iterator begin( ) const;
            virtual typename super::const_iterator end( ) const;
            virtual typename super::size_type size( ) const;
            virtual bool empty( ) const;

    }; //class Container

} //namespace containers

#endif
