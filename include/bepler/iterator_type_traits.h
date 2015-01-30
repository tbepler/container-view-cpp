#ifndef INCLUDED_BEPLER_ITERATOR_TYPE_TRAITS_H
#define INCLUDED_BEPLER_ITERATOR_TYPE_TRAITS_H

#include "bepler/type_traits_extended.h"
#include <type_traits>
#include <iterator>

namespace types{

    template< typename T >
    struct is_random_access_iterator_category{
        enum{ value = false };
    };

    template<>
    struct is_random_access_iterator_category<std::random_access_iterator_tag>{
        enum{ value = true };
    };

    template< typename T >
    struct is_bidirectional_iterator_category{
        enum{ value = is_random_access_iterator_category<T>::value };
    };

    template<>
    struct is_bidirectional_iterator_category<std::bidirectional_iterator_tag>{
        enum{ value = true };
    };

    template< typename T >
    struct is_forward_iterator_category{
        enum{ value = is_bidirectional_iterator_category<T>::value };
    };

    template<>
    struct is_forward_iterator_category<std::forward_iterator_tag>{
        enum{ value = true };
    };

    template< typename T >
    struct is_output_iterator_category{
        enum{ value = is_forward_iterator_category<T>::value };
    };

    template<>
    struct is_output_iterator_category<std::output_iterator_tag>{
        enum{ value = true };
    };

    template< typename T >
    struct is_input_iterator_category{
        enum{ value = is_forward_iterator_category<T>::value };
    };

    template<>
    struct is_input_iterator_category<std::input_iterator_tag>{
        enum{ value = true };
    };

    template< typename T >
    struct is_iterator_category{
        enum{
            value = is_input_iterator_category<T>::value || is_output_iterator_category<T>::value
        };
    };

    template< typename T, bool B = is_iterator_category<T>::value >
    struct iterator_category{
        
    };

    template< typename T >
    struct iterator_category<T,true>{
        typedef T type;
    };

    template< typename T >
    struct defines_iterator_traits{
        template<
            typename U,
            typename Difference = typename std::iterator_traits<U>::difference_type,
            typename Value = typename std::iterator_traits<U>::value_type,
            typename Pointer = typename std::iterator_traits<U>::pointer,
            typename Reference = typename std::iterator_traits<U>::reference,
            typename Category = typename iterator_category< typename std::iterator_traits<U>::iterator_category >::type
        > static char test( int );
    
        template< typename U > static long test(...);

        enum{ value = 1 == sizeof test<T>(0) };
    }; //struct defines_iterator_traits

    template< typename T >
    struct has_iterator_base_properties{
        enum{
            value = 
                std::is_copy_constructible<T>::value
                && std::is_copy_assignable<T>::value
                && std::is_destructible<T>::value
                && is_incrementable<T>::value
                && defines_iterator_traits<T>::value
        };
    }; //struct has_iterator_base_properties

    template< typename T, bool has_iter_base = has_iterator_base_properties<T>::value >
    struct is_input_iterator{
        enum { value = false };
    }; //struct is_input_iterator

    template< typename T >
    struct is_input_iterator<T,true>{
        enum{
            value = 
                is_input_iterator_category< typename std::iterator_traits<T>::iterator_category>::value
                && is_equals_comparable<T>::value
                && has_dereference_operator<T>::value
                && ( (!std::is_class<typename std::iterator_traits<T>::value_type>::value && !std::is_union<typename std::iterator_traits<T>::value_type>::value) || has_arrow_operator<T>::value )
        };
    }; //struct is_input_iterator has_iter_base = true specialization

    template< typename T, bool has_iter_base = has_iterator_base_properties<T>::value >
    struct is_output_iterator{
        enum { value = false };
    }; //struct is_output_iterator

    template< typename T >
    struct is_output_iterator<T,true>{
        enum{
            value =
                is_output_iterator_category< typename std::iterator_traits<T>::iterator_category >::value
                && has_dereference_operator<T>::value
                //should be dereferenced as lvalue TODO
        };
    }; //struct is_output_iterator has_iter_base = true specialization

    template<
        typename T,
        bool is_input_iter = is_input_iterator<T>::value,
        bool is_output_iter = is_output_iterator<T>::value
    >
    struct is_forward_iterator{
        enum { value = false };
    }; //struct is_forward_iterator
     
    template< typename T >
    struct is_forward_iterator<T,true,true>{
        enum{
            value =
                is_forward_iterator_category< typename std::iterator_traits<T>::iterator_category>::value
                && std::is_default_constructible<T>::value
                //also needs to be multi-pass, but may not be checkable at compile time TODO
        };
    }; //struct is_forward_iterator, is_input_iter = true and is_output_iter = true specialization

    template< typename T, bool is_fwd_iter = is_forward_iterator<T>::value >
    struct is_bidirectional_iterator{
        enum { value = false };
    }; //struct is_bidirectional_iterator

    template< typename T >
    struct is_bidirectional_iterator<T,true>{
        enum {
            value =
                is_bidirectional_iterator_category< typename std::iterator_traits<T>::iterator_category>::value
                && is_decrementable<T>::value
        };
    }; //struct is_bidirectional_iterator, is_fwd_iter = true specialization

    template< typename T, bool is_bid_iter = is_bidirectional_iterator<T>::value >
    struct is_random_access_iterator{
        enum { value = false };
    }; //struct is_random_access_iterator

    template< typename T >
    struct is_random_access_iterator<T,true>{
        enum {
            value = 
                is_random_access_iterator_category< typename std::iterator_traits<T>::iterator_category>::value
                && is_addable< T,int >::value
                && is_addable< int, T >::value
                && is_subtractable< T,int >::value
                && is_subtractable< T, T >::value
                && is_inequality_comparable<T>::value
                && is_compound_assignable<T,int>::value
                && is_indexable_by<T,int>::value
        };
    }; //struct is_random_access_iterator, is_bid_iter = true specialization

    template< typename T >
    struct is_iterator{
        enum { value = is_input_iterator<T>::value || is_output_iterator<T>::value };
    }; //struct is_iterator

    template< typename T, bool is_iter = is_iterator<T>::value >
    struct is_const_iterator{
        enum { value = false };
    }; //struct is_const_iterator

    template< typename T >
    struct is_const_iterator<T,true>{
        enum{ value = !std::is_assignable< typename std::iterator_traits<T>::reference, typename std::iterator_traits<T>::value_type >::value };
    }; //struct is_const_iterator, is_iter = true specialization

} //namespace types

#endif
