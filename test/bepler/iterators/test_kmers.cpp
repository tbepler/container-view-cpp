#include "gtest/gtest.h"
#include "bepler/iterators/kmers.h"
#include <set>
#include <vector>
#include <string>

using namespace std;
using namespace iterators;

set<char> dnaSet( ){
    set<char> dna;
    dna.insert( 'A' );
    dna.insert( 'C' );
    dna.insert( 'G' );
    dna.insert( 'T' );
    return dna;
}

vector<char> dnaVec( ){
    vector<char> dna;
    dna.push_back( 'A' );
    dna.push_back( 'C' );
    dna.push_back( 'G' );
    dna.push_back( 'T' );
    return dna;
}

const static set<char> DNA_SET = dnaSet( );
const static vector<char> DNA_VEC = dnaVec( );
const static string DNA_STR = "ACGT";

template< typename T >
bool operator==( const set<T>& s, const vector<T>& v ){
    if( s.size() != v.size() ) return false;
    for( auto vit = v.begin() ; vit != v.end() ; ++vit ){
        if( !s.count( *vit ) ) return false;
    }
    return true;
}

template< typename T >
bool operator==( const vector<T>& v, const set<T>& s ){
    return s == v;
}

bool operator==( const set<char>& s, const string& v ){
    if( s.size() != v.size() ) return false;
    for( auto vit = v.begin() ; vit != v.end() ; ++vit ){
        if( !s.count( *vit ) ) return false;
    }
    return true;
}

bool operator==( const string& v, const set<char>& s ){
    return s == v;
}

TEST( KmersTest, InitializationWorks ){

    Kmers<char> kmers;
    EXPECT_EQ( 0, kmers.k() );
    EXPECT_TRUE( kmers.alphabet().empty() );
    EXPECT_EQ( 0, kmers.size() );
    EXPECT_TRUE( kmers[0].empty() );
    EXPECT_TRUE( kmers[100].empty() );

    Kmers<char,string> kmers2( DNA_SET, 1 );
    EXPECT_EQ( 1, kmers2.k() );
    EXPECT_EQ( DNA_SET, kmers2.alphabet() );
    EXPECT_EQ( 4, kmers2.size() );
    EXPECT_EQ( 1, kmers2[0].size() );
    EXPECT_EQ( 1, kmers2[3].size() );
    EXPECT_NE( kmers2[0], kmers2[1] );
    EXPECT_NE( kmers2[1], kmers2[2] );
    EXPECT_NE( kmers2[2], kmers2[3] );
    
    Kmers<char> kmers3( DNA_VEC, 2 );
    EXPECT_EQ( 2, kmers3.k() );
    EXPECT_EQ( DNA_VEC, kmers3.alphabet() );
    EXPECT_EQ( 16, kmers3.size() );
    for( size_t i = 0 ; i < 16 ; ++i ){
        EXPECT_EQ( 2, kmers3[i].size() );
    }
    Kmers<char>::container_type prev = kmers3[0];
    for( size_t i = 1 ; i < 16 ; ++i ){
        Kmers<char>::container_type cur = kmers3[i];
        EXPECT_NE( prev, cur );
        prev = cur;
    }

}

TEST( KmersTest, IsIterable ){

    Kmers<char,string> kmers( DNA_STR, 3 );
    EXPECT_EQ( kmers.end(), kmers.end() - kmers.begin() );
    Kmers<char,string>::iterator it = kmers.begin();
    for( size_t i = 0 ; i < kmers.size() ; ++i ){
        EXPECT_EQ( kmers[i], kmers.begin()[i] );
        EXPECT_EQ( kmers[i], *( kmers.begin() + i ) );
        EXPECT_EQ( kmers[i], *it );
        ++it;
    }
    vector<string> fromkmer;
    for( size_t i = 0 ; i < kmers.size() ; ++i ){
        fromkmer.push_back( kmers[i] );
    }
    vector<string> fromiter;
    for( auto it = kmers.begin() ; it != kmers.end() ; ++it ){
        fromiter.push_back( *it );
    }
    EXPECT_EQ( fromkmer, fromiter );

}

TEST( KmersTest, IndexingBidirectional ){

    long alphabet[] = { 0, 1, 2 };
    for( size_t k = 0 ; k < 4 ; ++k ){
        Kmers<long> kmers( alphabet, alphabet+3, k );
        Kmers<long>::container_type kmer;
        for( size_t i = 0 ; i < kmers.size() ; ++i ){
            kmer = kmers[i];
            EXPECT_EQ( i, kmers.indexOf( kmer ) );
        }
    }

}

TEST( KmersTest, GetVariants ){

    Kmers<char,string> kmers( DNA_STR, 3 );
    char array[4];
    array[3] = '\0';
    for( size_t i = 0 ; i < kmers.size() ; ++i ){
        string idxop = kmers[i];
        string getret = kmers.get(i);
        kmers.get( i, array );
        EXPECT_EQ( idxop, getret );
        EXPECT_EQ( getret, array );
    }

}

TEST( KmersTest, KmerGeneration ){

    Kmers<char,string> kmers( DNA_STR, 2 );
    char kmer[3];
    kmer[2] = '\0';
    kmers.get( 0, kmer );
    EXPECT_EQ( "AA", kmer );
    kmers.get( 1, kmer );
    EXPECT_EQ( "AC", kmer );
    kmers.get( 2, kmer );
    EXPECT_EQ( "AG", kmer );
    kmers.get( 3, kmer );
    EXPECT_EQ( "AT", kmer );
    kmers.get( 4, kmer );
    EXPECT_EQ( "CA", kmer );
    kmers.get( 14, kmer );
    EXPECT_EQ( "TG", kmer );
    kmers.get( 15, kmer );
    EXPECT_EQ( "TT", kmer );

}
