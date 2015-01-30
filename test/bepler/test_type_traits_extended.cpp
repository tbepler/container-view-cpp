#include "gtest/gtest.h"
#include "bepler/type_traits_extended.h"

#include <vector>
#include <string>

struct HasIncrement{
    int i_;
    HasIncrement& operator++(){
        ++i_;
        return *this;
    }
    HasIncrement operator++(int){
        HasIncrement copy( *this );
        ++(*this);
        return copy;
    }
};

struct NoTraits{
    int i_;
};

TEST( TypeTraitsTest, IsIncrementable ){
    using types::is_incrementable;
    using std::vector;
    using std::string;
    EXPECT_TRUE( is_incrementable<int>::value );
    EXPECT_TRUE( is_incrementable<unsigned long>::value );
    EXPECT_TRUE( is_incrementable<char>::value );
    EXPECT_TRUE( is_incrementable<float>::value );
    EXPECT_TRUE( is_incrementable<vector<char>::iterator>::value );
    EXPECT_FALSE( is_incrementable< vector<char> >::value );
    EXPECT_FALSE( is_incrementable< string >::value );
    EXPECT_TRUE( is_incrementable<HasIncrement>::value );
    EXPECT_FALSE( is_incrementable<NoTraits>::value );
}

struct HasDecrement{
    int i_;
    HasDecrement& operator--(){
        --i_;
        return *this;
    }
    HasDecrement operator--(int){
        HasDecrement copy( *this );
        --(*this);
        return copy;
    }
};

TEST( TypeTraitsTest, IsDecrementable ){
    using types::is_decrementable;
    using std::vector;
    using std::string;
    EXPECT_TRUE( is_decrementable<int>::value );
    EXPECT_TRUE( is_decrementable<unsigned long>::value );
    EXPECT_TRUE( is_decrementable<char>::value );
    EXPECT_TRUE( is_decrementable<float>::value );
    EXPECT_TRUE( is_decrementable<vector<char>::iterator>::value );
    EXPECT_FALSE( is_decrementable< vector<char> >::value );
    EXPECT_FALSE( is_decrementable< string >::value );
    EXPECT_TRUE( is_decrementable<HasDecrement>::value );
    EXPECT_FALSE( is_decrementable<NoTraits>::value );
}

struct IsEqualsComparable{
    int i_;
    bool operator== ( const IsEqualsComparable& b ) const{
        return i_ == b.i_;
    }
    bool operator!= ( const IsEqualsComparable& b ) const{
        return i_ != b.i_;
    }
};

TEST( TypeTraitsTest, IsEqualsComparable ){
    using types::is_equals_comparable;
    EXPECT_TRUE( is_equals_comparable<int>::value );
    EXPECT_TRUE( is_equals_comparable<float>::value );
    EXPECT_TRUE( is_equals_comparable<bool>::value );
    EXPECT_TRUE( is_equals_comparable<std::string>::value );
    EXPECT_TRUE( is_equals_comparable<std::vector<int> >::value );

    EXPECT_TRUE( is_equals_comparable<IsEqualsComparable>::value );
    EXPECT_FALSE( is_equals_comparable<NoTraits>::value );
}

struct DereferenceAndArrow{
    int* i_;
    int& operator* ( ){
        return *i_;
    }
    int* operator-> ( ){
        return i_;
    }
};

TEST( TypeTraitsTest, HasDereferenceOperator ){
    using types::has_dereference_operator;

    EXPECT_TRUE( has_dereference_operator<int*>::value );
    EXPECT_TRUE( has_dereference_operator<NoTraits*>::value );
    EXPECT_TRUE( has_dereference_operator<std::string::iterator>::value );
    EXPECT_TRUE( has_dereference_operator<std::string::const_iterator>::value );
    EXPECT_TRUE( has_dereference_operator<DereferenceAndArrow>::value );

    EXPECT_FALSE( has_dereference_operator<int>::value );
    EXPECT_FALSE( has_dereference_operator<std::string>::value );
    EXPECT_FALSE( has_dereference_operator<NoTraits>::value );
}

TEST( TypeTraitsTest, HasArrowOperator ){
    using types::has_arrow_operator;

    EXPECT_TRUE( has_arrow_operator<int*>::value );
    EXPECT_TRUE( has_arrow_operator<NoTraits*>::value );
    EXPECT_TRUE( has_arrow_operator<std::string::iterator>::value );
    EXPECT_TRUE( has_arrow_operator<std::string::const_iterator>::value );
    EXPECT_TRUE( has_arrow_operator<DereferenceAndArrow>::value );

    EXPECT_FALSE( has_arrow_operator<int>::value );
    EXPECT_FALSE( has_arrow_operator<std::string>::value );
    EXPECT_FALSE( has_arrow_operator<NoTraits>::value );

}

struct IsInequalityComparable{
    int i_;
    bool operator< ( const IsInequalityComparable& b ) const{
        return i_ < b.i_;
    }
    bool operator<= ( const IsInequalityComparable& b ) const{
        return i_ <= b.i_;
    }
    bool operator> ( const IsInequalityComparable& b ) const{
        return i_ > b.i_;
    }
    bool operator>= ( const IsInequalityComparable& b ) const{
        return i_ >= b.i_;
    }
};

TEST( TypeTraitsTest, IsInequalityComparable ){
    using types::is_inequality_comparable;
    EXPECT_TRUE( is_inequality_comparable<int>::value );
    EXPECT_TRUE( is_inequality_comparable<float>::value );
    EXPECT_TRUE( is_inequality_comparable<bool>::value );
    EXPECT_TRUE( is_inequality_comparable<std::string>::value );
    EXPECT_TRUE( is_inequality_comparable<std::vector<int> >::value );

    EXPECT_TRUE( is_inequality_comparable<IsInequalityComparable>::value );
    EXPECT_FALSE( is_inequality_comparable<IsEqualsComparable>::value );
    EXPECT_FALSE( is_inequality_comparable<NoTraits>::value );
}

struct Addable{
    int i_;
    Addable& operator+=( const Addable& b ){
        i_ += b.i_;
        return *this;
    }
    Addable& operator+=( int i ){
        i_ += i;
        return *this;
    }
};

Addable operator+ ( const Addable& a, const Addable& b ){
    Addable copy( a );
    copy += b;
    return copy;
}

Addable operator+ ( const Addable& a, int b ){
    Addable copy( a );
    copy += b;
    return copy;
}

struct Subtractable{
    int i_;
    Subtractable& operator-= ( int i ){
        i_ -= i;
        return *this;
    }
    Subtractable& operator-= ( const Subtractable& b ){
        return *this -= b.i_;
    }
};

Subtractable operator- ( const Subtractable& a, int i ){
    Subtractable copy( a );
    copy -= i;
    return copy;
}

Subtractable operator- ( const Subtractable& a, const Subtractable& b ){
    return a - b.i_;
}

struct AddAndSubtractable : public Addable, public Subtractable{

};

TEST( TypeTraitsTest, IsAddable ){
    using types::is_addable;

    EXPECT_TRUE( is_addable<int>::value );
    EXPECT_TRUE( (is_addable<int,float>::value) );
    EXPECT_TRUE( is_addable<float>::value );
    EXPECT_TRUE( is_addable<Addable>::value );
    EXPECT_TRUE( (is_addable<Addable,int>::value) );
    EXPECT_TRUE( is_addable<std::string>::value );
    EXPECT_TRUE( (is_addable<std::string,char>::value) );
    EXPECT_TRUE( is_addable<AddAndSubtractable>::value );
    
    EXPECT_FALSE( (is_addable<Addable,Subtractable>::value) );
    EXPECT_FALSE( (is_addable<Addable,std::string>::value) );
    EXPECT_FALSE( (is_addable<std::string,float>::value) );

}

TEST( TypeTraitsTest, IsSubtractable ){
    using types::is_subtractable;

    EXPECT_TRUE( is_subtractable<int>::value );
    EXPECT_TRUE( (is_subtractable<int,float>::value) );
    EXPECT_TRUE( is_subtractable<float>::value );
    EXPECT_TRUE( is_subtractable<Subtractable>::value );
    EXPECT_TRUE( (is_subtractable<Subtractable,int>::value) );
    EXPECT_TRUE( is_subtractable<AddAndSubtractable>::value );
    
    EXPECT_FALSE( is_subtractable<std::string>::value );
    EXPECT_FALSE( (is_subtractable<std::string,char>::value) );
    EXPECT_FALSE( (is_subtractable<Addable,Subtractable>::value) );
    EXPECT_FALSE( (is_subtractable<Addable,std::string>::value) );
    EXPECT_FALSE( (is_subtractable<std::string,float>::value) );

}

TEST( TypeTraitsTest, IsCompoundAssignable ){
    using types::is_compound_assignable;

    EXPECT_TRUE( is_compound_assignable<int>::value );
    EXPECT_TRUE( (is_compound_assignable<int,float>::value) );
    EXPECT_TRUE( is_compound_assignable<float>::value );
    EXPECT_FALSE( is_compound_assignable<Addable>::value );
    EXPECT_FALSE( (is_compound_assignable<Subtractable,int>::value) );
    EXPECT_TRUE( (is_compound_assignable<AddAndSubtractable,int>::value) );
    EXPECT_TRUE( (is_compound_assignable<Addable*,int>::value ) );
    
    EXPECT_FALSE( is_compound_assignable<std::string>::value );
    EXPECT_FALSE( (is_compound_assignable<std::string,char>::value) );
    EXPECT_FALSE( (is_compound_assignable<Addable,Subtractable>::value) );
    EXPECT_FALSE( (is_compound_assignable<Addable,std::string>::value) );
    EXPECT_FALSE( (is_compound_assignable<std::string,float>::value) );

}

struct Indexable{
    int v_;
    int operator[] ( int i ){
        return v_ + i;
    }
};

TEST( TypeTraitsTest, IsIndexableBy ){
    using types::is_indexable_by;

    EXPECT_TRUE( (is_indexable_by<std::vector<int>,int>::value) );
    EXPECT_TRUE( (is_indexable_by<std::string, int>::value) );
    EXPECT_FALSE( (is_indexable_by<std::string, std::string>::value) );
    EXPECT_TRUE( (is_indexable_by<Indexable, int>::value) );
    EXPECT_FALSE( (is_indexable_by<int,int>::value) );
    EXPECT_FALSE( (is_indexable_by<Addable,int>::value) );
    EXPECT_TRUE( (is_indexable_by<int*,unsigned long>::value) );
}

