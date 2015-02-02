#include "gtest/gtest.h"
#include "bepler/type_traits/iterator_traits.h"

#include <iterator>
#include <vector>
#include <string>
#include <list>
#include <forward_list>
#include <utility>

using namespace type_traits;

TEST( IteratorTraitsTest, IsIteratorCategories ){

    EXPECT_TRUE( is_iterator_category<std::random_access_iterator_tag>::value );
    EXPECT_TRUE( is_input_iterator_category<std::random_access_iterator_tag>::value );
    EXPECT_TRUE( is_output_iterator_category<std::random_access_iterator_tag>::value );
    EXPECT_TRUE( is_forward_iterator_category<std::random_access_iterator_tag>::value );
    EXPECT_TRUE( is_bidirectional_iterator_category<std::random_access_iterator_tag>::value );
    EXPECT_TRUE( is_random_access_iterator_category<std::random_access_iterator_tag>::value );

    EXPECT_TRUE( is_iterator_category<std::bidirectional_iterator_tag>::value );
    EXPECT_TRUE( is_input_iterator_category<std::bidirectional_iterator_tag>::value );
    EXPECT_TRUE( is_output_iterator_category<std::bidirectional_iterator_tag>::value );
    EXPECT_TRUE( is_forward_iterator_category<std::bidirectional_iterator_tag>::value );
    EXPECT_TRUE( is_bidirectional_iterator_category<std::bidirectional_iterator_tag>::value );
    EXPECT_FALSE( is_random_access_iterator_category<std::bidirectional_iterator_tag>::value );

    
    EXPECT_TRUE( is_iterator_category<std::forward_iterator_tag>::value );
    EXPECT_TRUE( is_input_iterator_category<std::forward_iterator_tag>::value );
    EXPECT_TRUE( is_output_iterator_category<std::forward_iterator_tag>::value );
    EXPECT_TRUE( is_forward_iterator_category<std::forward_iterator_tag>::value );
    EXPECT_FALSE( is_bidirectional_iterator_category<std::forward_iterator_tag>::value );
    EXPECT_FALSE( is_random_access_iterator_category<std::forward_iterator_tag>::value );

    
    EXPECT_TRUE( is_iterator_category<std::input_iterator_tag>::value );
    EXPECT_TRUE( is_input_iterator_category<std::input_iterator_tag>::value );
    EXPECT_FALSE( is_output_iterator_category<std::input_iterator_tag>::value );
    EXPECT_FALSE( is_forward_iterator_category<std::input_iterator_tag>::value );
    EXPECT_FALSE( is_bidirectional_iterator_category<std::input_iterator_tag>::value );
    EXPECT_FALSE( is_random_access_iterator_category<std::input_iterator_tag>::value );

    
    EXPECT_TRUE( is_iterator_category<std::output_iterator_tag>::value );
    EXPECT_FALSE( is_input_iterator_category<std::output_iterator_tag>::value );
    EXPECT_TRUE( is_output_iterator_category<std::output_iterator_tag>::value );
    EXPECT_FALSE( is_forward_iterator_category<std::output_iterator_tag>::value );
    EXPECT_FALSE( is_bidirectional_iterator_category<std::output_iterator_tag>::value );
    EXPECT_FALSE( is_random_access_iterator_category<std::output_iterator_tag>::value );

    
    EXPECT_FALSE( is_iterator_category<int>::value );
    EXPECT_FALSE( is_input_iterator_category<int>::value );
    EXPECT_FALSE( is_output_iterator_category<int>::value );
    EXPECT_FALSE( is_forward_iterator_category<int>::value );
    EXPECT_FALSE( is_bidirectional_iterator_category<int>::value );
    EXPECT_FALSE( is_random_access_iterator_category<int>::value );

}

struct DefinesIterTraits{
    typedef std::ptrdiff_t difference_type;
    typedef int value_type;
    typedef int* pointer;
    typedef int& reference;
    typedef std::input_iterator_tag iterator_category;
};

struct NoIterTraits{
    NoIterTraits& operator++(){
        return *this;
    }
    NoIterTraits operator++(int){
        NoIterTraits copy( *this );
        ++(*this);
        return copy;
    }
};

struct InvalidCategoryIterTraits{
    typedef std::ptrdiff_t difference_type;
    typedef int value_type;
    typedef int* pointer;
    typedef int& reference;
    typedef char iterator_category;
};

TEST( IteratorTraitsTest, DefinesIteratorTraits ){

    EXPECT_TRUE( defines_iterator_traits<DefinesIterTraits>::value );
    EXPECT_FALSE( defines_iterator_traits<NoIterTraits>::value );
    EXPECT_FALSE( defines_iterator_traits<InvalidCategoryIterTraits>::value );
    
    EXPECT_TRUE( defines_iterator_traits<std::vector<int>::iterator>::value );
    EXPECT_TRUE( defines_iterator_traits<std::vector<int>::const_iterator>::value );
    EXPECT_TRUE( defines_iterator_traits<std::string::iterator>::value );
    EXPECT_TRUE( defines_iterator_traits<std::string::const_iterator>::value );
    EXPECT_TRUE( defines_iterator_traits<int*>::value );
    EXPECT_FALSE( defines_iterator_traits<int>::value );
    EXPECT_FALSE( defines_iterator_traits<float>::value );

}

TEST( IteratorTraitsTest, HasIteratorBaseProperties ){

    EXPECT_FALSE( has_iterator_base_properties<int>::value );
    EXPECT_FALSE( has_iterator_base_properties<float>::value );

    EXPECT_TRUE( has_iterator_base_properties<std::vector<int>::iterator>::value );
    EXPECT_TRUE( has_iterator_base_properties<std::vector<int>::const_iterator>::value );
    EXPECT_TRUE( has_iterator_base_properties<std::string::iterator>::value );
    EXPECT_TRUE( has_iterator_base_properties<std::string::const_iterator>::value );
    EXPECT_TRUE( has_iterator_base_properties<int*>::value );

    EXPECT_FALSE( has_iterator_base_properties<DefinesIterTraits>::value );
    EXPECT_FALSE( has_iterator_base_properties<NoIterTraits>::value );
    EXPECT_FALSE( has_iterator_base_properties<InvalidCategoryIterTraits>::value );
}

TEST( IteratorTraitsTest, IsInputIterator ){

    EXPECT_FALSE( is_input_iterator<int>::value );
    EXPECT_FALSE( is_input_iterator<float>::value );

    EXPECT_TRUE( is_input_iterator<std::vector<int>::iterator>::value );
    EXPECT_TRUE( is_input_iterator<std::vector<int>::const_iterator>::value );
    EXPECT_TRUE( is_input_iterator<std::string::iterator>::value );
    EXPECT_TRUE( is_input_iterator<std::string::const_iterator>::value );
    EXPECT_TRUE( is_input_iterator<int*>::value );
    EXPECT_TRUE( is_input_iterator<std::list<char>::iterator>::value );
    EXPECT_TRUE( is_input_iterator<std::list<char>::const_iterator>::value );
    EXPECT_TRUE( is_input_iterator<std::forward_list<char>::iterator>::value );
    EXPECT_TRUE( is_input_iterator<std::forward_list<char>::const_iterator>::value );

    EXPECT_FALSE( is_input_iterator<DefinesIterTraits>::value );
    EXPECT_FALSE( is_input_iterator<NoIterTraits>::value );
    EXPECT_FALSE( is_input_iterator<InvalidCategoryIterTraits>::value );

    EXPECT_FALSE( is_input_iterator< std::back_insert_iterator<std::vector<int> > >::value );

    EXPECT_FALSE( is_input_iterator< std::ostream_iterator<int> >::value ); 
    EXPECT_TRUE( is_input_iterator< std::istream_iterator<int> >::value );
}

TEST( IteratorTraitsTest, IsOutputIterator ){

    EXPECT_FALSE( is_output_iterator<int>::value );
    EXPECT_FALSE( is_output_iterator<float>::value );

    EXPECT_TRUE( is_output_iterator<std::vector<int>::iterator>::value );
    EXPECT_TRUE( is_output_iterator<std::vector<int>::const_iterator>::value );
    EXPECT_TRUE( is_output_iterator<std::string::iterator>::value );
    EXPECT_TRUE( is_output_iterator<std::string::const_iterator>::value );
    EXPECT_TRUE( is_output_iterator<int*>::value );
    EXPECT_TRUE( is_output_iterator<std::list<char>::iterator>::value );
    EXPECT_TRUE( is_output_iterator<std::list<char>::const_iterator>::value );
    EXPECT_TRUE( is_output_iterator<std::forward_list<char>::iterator>::value );
    EXPECT_TRUE( is_output_iterator<std::forward_list<char>::const_iterator>::value );

    EXPECT_FALSE( is_output_iterator<DefinesIterTraits>::value );
    EXPECT_FALSE( is_output_iterator<NoIterTraits>::value );
    EXPECT_FALSE( is_output_iterator<InvalidCategoryIterTraits>::value );

    EXPECT_TRUE( is_output_iterator< std::back_insert_iterator<std::vector<int> > >::value );

    EXPECT_TRUE( is_output_iterator< std::ostream_iterator<int> >::value );
    EXPECT_FALSE( is_output_iterator< std::istream_iterator<int> >::value );
}

TEST( IteratorTraitsTest, IsForwardIterator ){

    EXPECT_FALSE( is_forward_iterator<int>::value );
    EXPECT_FALSE( is_forward_iterator<float>::value );

    EXPECT_TRUE( is_forward_iterator<std::vector<int>::iterator>::value );
    EXPECT_TRUE( is_forward_iterator<std::vector<int>::const_iterator>::value );
    EXPECT_TRUE( is_forward_iterator<std::string::iterator>::value );
    EXPECT_TRUE( is_forward_iterator<std::string::const_iterator>::value );
    EXPECT_TRUE( is_forward_iterator<int*>::value );
    EXPECT_TRUE( is_forward_iterator<std::list<char>::iterator>::value );
    EXPECT_TRUE( is_forward_iterator<std::list<char>::const_iterator>::value );
    EXPECT_TRUE( is_forward_iterator<std::forward_list<char>::iterator>::value );
    EXPECT_TRUE( is_forward_iterator<std::forward_list<char>::const_iterator>::value );

    EXPECT_FALSE( is_forward_iterator<DefinesIterTraits>::value );
    EXPECT_FALSE( is_forward_iterator<NoIterTraits>::value );
    EXPECT_FALSE( is_forward_iterator<InvalidCategoryIterTraits>::value );

    EXPECT_FALSE( is_forward_iterator< std::back_insert_iterator<std::vector<int> > >::value );

    EXPECT_FALSE( is_forward_iterator< std::ostream_iterator<int> >::value );
    EXPECT_FALSE( is_forward_iterator< std::istream_iterator<int> >::value );
}

TEST( IteratorTraitsTest, IsBidirectionalIterator ){

    EXPECT_FALSE( is_bidirectional_iterator<int>::value );
    EXPECT_FALSE( is_bidirectional_iterator<float>::value );

    EXPECT_TRUE( is_bidirectional_iterator<std::vector<int>::iterator>::value );
    EXPECT_TRUE( is_bidirectional_iterator<std::vector<int>::const_iterator>::value );
    EXPECT_TRUE( is_bidirectional_iterator<std::string::iterator>::value );
    EXPECT_TRUE( is_bidirectional_iterator<std::string::const_iterator>::value );
    EXPECT_TRUE( is_bidirectional_iterator<int*>::value );
    EXPECT_TRUE( is_bidirectional_iterator<std::list<char>::iterator>::value );
    EXPECT_TRUE( is_bidirectional_iterator<std::list<char>::const_iterator>::value );
    EXPECT_FALSE( is_bidirectional_iterator<std::forward_list<char>::iterator>::value );
    EXPECT_FALSE( is_bidirectional_iterator<std::forward_list<char>::const_iterator>::value );

    EXPECT_FALSE( is_bidirectional_iterator<DefinesIterTraits>::value );
    EXPECT_FALSE( is_bidirectional_iterator<NoIterTraits>::value );
    EXPECT_FALSE( is_bidirectional_iterator<InvalidCategoryIterTraits>::value );

    EXPECT_FALSE( is_bidirectional_iterator< std::back_insert_iterator<std::vector<int> > >::value );

    EXPECT_FALSE( is_bidirectional_iterator< std::ostream_iterator<int> >::value );
    EXPECT_FALSE( is_bidirectional_iterator< std::istream_iterator<int> >::value );
}

TEST( IteratorTraitsTest, IsRandomAccessIterator ){

    EXPECT_FALSE( is_random_access_iterator<int>::value );
    EXPECT_FALSE( is_random_access_iterator<float>::value );

    EXPECT_TRUE( is_random_access_iterator<std::vector<int>::iterator>::value );
    EXPECT_TRUE( is_random_access_iterator<std::vector<int>::const_iterator>::value );
    EXPECT_TRUE( is_random_access_iterator<std::vector<bool>::iterator>::value );
    EXPECT_TRUE( is_random_access_iterator<std::vector<bool>::const_iterator>::value );
    EXPECT_TRUE( is_random_access_iterator<std::string::iterator>::value );
    EXPECT_TRUE( is_random_access_iterator<std::string::const_iterator>::value );
    EXPECT_TRUE( is_random_access_iterator<int*>::value );
    EXPECT_FALSE( is_random_access_iterator<std::list<char>::iterator>::value );
    EXPECT_FALSE( is_random_access_iterator<std::list<char>::const_iterator>::value );
    EXPECT_FALSE( is_random_access_iterator<std::forward_list<char>::iterator>::value );
    EXPECT_FALSE( is_random_access_iterator<std::forward_list<char>::const_iterator>::value );

    EXPECT_FALSE( is_random_access_iterator<DefinesIterTraits>::value );
    EXPECT_FALSE( is_random_access_iterator<NoIterTraits>::value );
    EXPECT_FALSE( is_random_access_iterator<InvalidCategoryIterTraits>::value );

    EXPECT_FALSE( is_random_access_iterator< std::back_insert_iterator<std::vector<int> > >::value );

    EXPECT_FALSE( is_random_access_iterator< std::ostream_iterator<int> >::value );
    EXPECT_FALSE( is_random_access_iterator< std::istream_iterator<int> >::value );
}

TEST( IteratorTraitsTest, IsIterator ){

    EXPECT_FALSE( is_iterator<int>::value );
    EXPECT_FALSE( is_iterator<float>::value );

    EXPECT_TRUE( is_iterator<std::vector<int>::iterator>::value );
    EXPECT_TRUE( is_iterator<std::vector<int>::const_iterator>::value );
    EXPECT_TRUE( is_iterator<std::string::iterator>::value );
    EXPECT_TRUE( is_iterator<std::string::const_iterator>::value );
    EXPECT_TRUE( is_iterator<int*>::value );
    EXPECT_TRUE( is_iterator<std::list<char>::iterator>::value );
    EXPECT_TRUE( is_iterator<std::list<char>::const_iterator>::value );
    EXPECT_TRUE( is_iterator<std::forward_list<char>::iterator>::value );
    EXPECT_TRUE( is_iterator<std::forward_list<char>::const_iterator>::value );

    EXPECT_FALSE( is_iterator<DefinesIterTraits>::value );
    EXPECT_FALSE( is_iterator<NoIterTraits>::value );
    EXPECT_FALSE( is_iterator<InvalidCategoryIterTraits>::value );

    EXPECT_TRUE( is_iterator< std::back_insert_iterator<std::vector<int> > >::value );

    EXPECT_TRUE( is_iterator< std::ostream_iterator<int> >::value );
    EXPECT_TRUE( is_iterator< std::istream_iterator<int> >::value );
}

TEST( IteratorTraitsTest, IsConstIterator ){

    EXPECT_FALSE( is_const_iterator< std::vector<int>::iterator >::value );
    EXPECT_TRUE( is_const_iterator< std::vector<int>::const_iterator >::value );

    EXPECT_FALSE( is_const_iterator< std::vector<bool>::iterator >::value );
    EXPECT_TRUE( is_const_iterator< std::vector<bool>::const_iterator >::value );

    EXPECT_FALSE( is_const_iterator< std::forward_list<char>::iterator >::value );
    EXPECT_TRUE( is_const_iterator< std::forward_list<char>::const_iterator >::value );

    EXPECT_FALSE( is_const_iterator< std::string::iterator >::value );
    EXPECT_TRUE( is_const_iterator< std::string::const_iterator >::value );
}










