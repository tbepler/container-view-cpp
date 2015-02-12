#ifndef INCLUDED_BEPLER_ITERTOOLS_TRANSFORM_H
#define INCLUDED_BEPLER_ITERTOOLS_TRANSFORM_H

#include <type_traits>
#include "bepler/type_traits/iterator_traits.h"
#include "bepler/itertools/iterator_adaptor.h"
#include "bepler/itertools/range.h"

namespace itertools{

    //forward declare for helpers types
    template< typename Iterator, typename Function >
    class TransformIterator;

    namespace helpers{

        template< typename I, typename F >
        using reference_t = decltype( std::declval<F>()( *std::declval<I>() ) );

        template< typename I, typename F >
        using value_t = std::decay< reference_t<I,F> >();

        template< typename I, typename F >
        using iterator_adaptor_type =
            typename IteratorAdaptor< TransformIterator< I, F >, I,
            value_t< I, F >, reference_t< I, F > >;

    } //namespace helpers

    template< typename Iterator, typename Function >
    class TransformIterator
        : public helpers::iterator_adaptor_type< Iterator, Function >{

        friend class helpers::iterator_adaptor_type< Iterator, Function >::base;
        typedef helpers::iterator_adpator_type< Iterator, Function > super_t
        typedef super_t::base base;

        Function f_;
        inline base::reference dereference() const{ return f_( *iter() ); }

        public:
            TransformIterator() { }
            TransformIterator( Iterator i, Function f = Function() )
                : super_t( i ), f_( f ) { }
            TransformIterator( const TransformIterator& rhs )
                : super_t( rhs.iter() ), f_( rhs.f_ ) { }
    };

    template< typename Iterator, typename Function >
    std::enable_if< type_traits::is_iterator<Iterator>::value,
        typename TransformIterator<Iterator,Function> >::type
    inline transform( Iterator i, Function f = Function() ){
        return TransformIterator<Iterator,Function>( i, f );
    }

    template< typename Iterator, typename Function >
    inline auto transform( Iterator begin, Iterator end, Function f = Function() )
        -> decltype( irange( transform( begin, f ), transform( end, f ) ) )
    {
        return irange( transform( begin, f ), transform( end, f ) );
    }

    template< typename Range, typename Function >
    inline auto transform( const Range& r, Function f = Function() )
        -> decltype( transform( std::begin( r ), std::end( r ), f ) )
    {
        return transform( std::begin( r ), std::end( r ), f );
    }



} //namespace itertools

#endif
