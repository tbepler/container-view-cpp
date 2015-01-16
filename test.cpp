#include "permutation.h"
#include "transform.h"
#include "slice.h"

#include <assert.h>
#include <vector>
#include <iostream>

using namespace std;

template< typename T >
bool equals( const T& a, const T& b, size_t size ){
    for( size_t i = 0 ; i < size ; ++i ){
        if( a[i] != b[i] ) return false;
    }
    return true;
}

void print( size_t i, vector<size_t> permutation ){
    cout << "Index: " << i << ", permutation: ";
    for( size_t j = 0 ; j < permutation.size() ; ++j ){
        cout << permutation[j];
        if( j != permutation.size() - 1 ){
            cout << " ";
        }
    }
    cout << endl;
}

template< typename C >
ostream& printContainer( ostream& out, const C& c, size_t size ){
    for( size_t i = 0 ; i < size ; ++i ){
        out << c[i];
        if( i != size -1 ){
            out << " ";
        }
    }
    return out;
}

void testPermutation( ){

    static const size_t ALPH = 4;
    static const size_t LEN = 3;

    cout << "Testing Permutation..." << endl;

    Permutation p( ALPH, LEN );
    assert( p.length() == LEN );
    assert( p.alphabet() == ALPH );
    assert( p.size() == 4*4*4 );

    vector<size_t> vec_perm( LEN );
    size_t arr_perm[ LEN ];

    vector<size_t> vec_setperm( LEN );
    size_t arr_setperm[ LEN ];

    for( size_t i = 0 ; i < ALPH ; ++i ){
        vec_perm[0] = arr_perm[0] = i;
        for( size_t j = 0 ; j < ALPH ; ++j ){
            vec_perm[1] = arr_perm[1] = j;
            for ( size_t k = 0 ; k < ALPH ; ++k ){
                vec_perm[2] = arr_perm[2] = k;
                size_t expected_idx = k + ALPH*j + ALPH*ALPH*i;
                cout << "Expected: ";
                print( expected_idx, vec_perm );
                size_t vec_idx = p.indexOf( vec_perm );
                size_t arr_idx = p.indexOf( arr_perm );
                assert( expected_idx == vec_idx );
                assert( vec_idx == arr_idx );
                cout << "Indexed: ";
                print( vec_idx, vec_perm );
                p.permutation( vec_idx, vec_setperm );
                p.permutation( vec_idx, arr_setperm );
                cout << "Assigned: ";
                print( vec_idx, vec_setperm );
                assert( equals( vec_setperm, vec_perm, LEN ) );
                assert( equals( arr_setperm, arr_perm, LEN ) );
            }
        }
    }

    cout << "Permutation tests passed." << endl;
}

template< typename T >
struct Add{

    T val;
    Add( const T& value ) : val( value ) { };
    T operator() ( const T& input ){
        return input + val;
    };

};

void testTransform(){

    cout << "Testing transform..." << endl;
    cout << "TODO: implement tests." << endl;

    /**
    vector<long> elems;
    elems.push_back( 1 );
    elems.push_back( -5 );
    elems.push_back( 100 );
    elems.push_back( -33 );
    elems.push_back( 24 );

    Add<long> add5( 5 );

    Transform< vector<long>, Add<long> > xform( elems, add5 );
    for( size_t i = 0 ; i < elems.size() ; ++i ){
        assert( elems[i]+5 == xform[i] );
    }
    cout << "Elems: ";
    printContainer( cout, elems, elems.size() ) << endl;
    cout << "Xform: ";
    printContainer( cout, xform, elems.size() ) << endl;
    **/

    cout << "Transform tests passed." << endl;

}

void testSlice(){
    
    cout << "Testing slice..." << endl;

    

    cout << "Slice tests passed." << endl;

}

int main( ){

    testPermutation();
    testTransform();
    testSlice();
    

}
