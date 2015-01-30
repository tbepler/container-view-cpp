#ifndef INCLUDED_BEPLER_ITERATOR_SPEC_H
#define INCLUDED_BEPLER_ITERATOR_SPEC_H

#include <type_traits>
#include "bepler/type_traits_extended.h"
#include <vector>

namespace test{

    namespace iterator{

        template< typename T >
        struct test_f{
            typedef bool(*type)(T,T);
        };

        template< typename I >
        struct IteratorTest{

            typedef typename test_f<I>::type test_f;
            
            static const std::vector< test_f > all_category_tests;
            static const std::vector< test_f > input_tests;
            static const std::vector< test_f > output_tests;
            static const std::vector< test_f > forward_tests;
            static const std::vector< test_f > bidirectional_tests;
            static const std::vector< test_f > random_access_tests;

        }; //class IteratorTest

        template< typename Iterator >
        bool isCopyConstructable(){
            return std::is_copy_constructible< Iterator >::value;
        }

        template< typename Iterator >
        bool isCopyAssignable(){
            return std::is_copy_assignable<Iterator>::value;
        }

        template< typename Iterator >
        bool isDestructable(){
            return std::is_destructible<Iterator>::value;
        }

        template< typename Iterator >
        bool canBeIncremented(){
            return types::is_incrementable<Iterator>::value;
        }

        template< typename Iterator >
        bool isConst(){
            return !std::is_assignable< typename Iterator::reference, typename Iterator::value_type >::value; 
        }


    } //namespace iterator

} //namespace test


#endif
