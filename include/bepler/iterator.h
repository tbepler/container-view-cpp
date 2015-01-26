#ifndef INCLUDED_BEPLER_ITERATOR_BASE_H
#define INCLUDED_BEPLER_ITERATOR_BASE_H

#include <type_traits>
#include <iterator>

template<
    typename Derived,
    typename Value,
    typename Reference = Value&,
    typename Difference = ptrdiff_t
>
struct IteratorBase : Comparable< T >{

    typedef Difference difference_type;
    typedef std::remove_const<Value>::type value_type;
    typedef Value* pointer;
    typedef Reference reference;
    typedef std::random_access_iterator_tag iterator_category;



}; //struct IteratorBase

#endif
