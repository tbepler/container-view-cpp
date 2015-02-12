#ifndef INCLUDED_BEPLER_ITERTOOLS_SLIDING_WINDOW_H
#define INCLUDED_BEPLER_ITERTOOLS_SLIDING_WINDOW_H

#include <deque>
#include <type_traits>
#include <stdexcept>
#include <cmath>
#include "bepler/itertools/iterator_base.h"
#include "bepler/itertools/iterator_adaptor.h"
#include "bepler/itertools/range.h"
#include "bepler/type_traits/iterator_traits.h"

namespace itertools{

    namespace helpers{

        template< typename I >
        using default_container_t = typename std::deque< typename std::iterator_traits<I>::value_type >;

        template< typename I >
        constexpr bool is_input_iterator(){
            return std::is_same<
                        typename std::iterator_traits<I>::iterator_category,
                        std::input_iterator_tag
                    >::value;
        }

        template< typename I >
        using container_t = typename std::conditional<
            is_input_iterator<I>(),
            default_container_t<I>,
            void
        >::type;

        template< typename I, typename C >
        struct value_type{
            using type = C;
        };

        template< typename I >
        struct value_type<I,void>{
            using type = decltype( irange( std::declval<I>(), std::declval<I>() ) );
        };

        template< typename I, typename C >
        struct reference_type{
            using type = const C&;
        };  

        template< typename I >
        struct reference_type<I,void>{
            using type = typename value_type<I,void>::type;
        };

        template< typename D, typename I, typename C >
        using adaptor_type = IteratorAdaptor< D, I, typename value_type<I,C>::type, typename reference_type<I,C>::type >;

    } //namespace helpers

    template< typename Iterator, typename Container = helpers::container_t<Iterator> >
    class WindowIterator : public helpers::adaptor_type<
        WindowIterator< Iterator, Container >, Iterator, Container >{

        

    };

    template< typename Iterator >
    class WindowIterator< Iterator, void > : public helpers::adaptor_type<
        WindowIterator< Iterator, void >, Iterator, void >{

        friend class helpers::adaptor_type< WindowIterator< Iterator, void >,
            Iterator, void >::base;
        typedef helpers::adaptor_type< WindowIterator< Iterator, void >,
            Iterator, void > super_t;
        typedef typename super_t::base base;

        Iterator first_;

        inline Iterator& last() { return this->iter(); }

        inline typename base::reference dereference() const{
            return irange( first_, std::next( this->iter(), 1 ) );
        }
        inline void inc() { ++first_, ++(this->iter()); }
        inline void dec() { --first_, --(this->iter()); }
        inline void advance( typename base::difference_type n ){
            first_ += n, (this->iter()) += n;
        }
        inline bool equals( const WindowIterator& w ) const{
            return ( first_ == last() && ( first_ == w.first_ || first_ == w.last() ) ) || ( w.first_ == w.last() && ( first_ == w.first_ || last() == w.first_ ) ) || ( first_ == w.first_ && last() == w.last() );
        }
        inline typename base::difference_type compareTo( const WindowIterator& w ) const{
            if( first_ == last() ){
                auto to_first = first_ - w.first_;
                auto to_last = first_ - w.last();
                return std::abs( to_first ) < std::abs( to_last ) ? to_first : to_last;
            }
            if( w.first_ == w.last() ){
                auto from_first = first_ - w.first_;
                auto from_last = last() - w.first_;
                return std::abs( from_first ) < std::abs( from_last ) ? from_first : from_last ;
            }
            if( first_ != w.first_ ){
                return first_ - w.first_;
            }
            return last() - w.last();
        }


        public:
            WindowIterator() { }
            WindowIterator( Iterator i ) : super_t ( i ), first_( i ) { }
            WindowIterator( Iterator begin, std::size_t n )
                : super_t( std::next( begin, n != 0 ? n - 1 : 0 ) ), first_( begin ) {
                if( n < 1 ){
                    throw std::out_of_range( "Window size must be > 0" );
                }
            }
            template< typename I = Iterator >
            WindowIterator( std::size_t n,
                typename std::enable_if< type_traits::is_bidirectional_iterator<I>::value, I >::type last
            )
                : super_t( last ), first_( std::prev( last, n != 0 ? n - 1 : 0 ) ) {
                if( n < 1 ){
                    throw std::out_of_range( "Window size must be > 0" );
                }
            }
            template< typename I = Iterator >
            WindowIterator( std::size_t n,
                typename std::enable_if< !type_traits::is_bidirectional_iterator<I>::value, I >::type last
            )
                : WindowIterator( last ) { }
            WindowIterator( Iterator first, Iterator last )
                : super_t( last ), first_( first ) { }
            WindowIterator( const WindowIterator& rhs )
                : super_t( rhs.iter() ), first_( rhs.first_ ) { }

            inline Iterator begin() const{ return first_; }
            inline const Iterator& last() const{ return this->iter(); }
            inline Iterator end() const{ return std::next( this->iter(), 1 ); }

    };

    template< typename Iterator >
    inline WindowIterator<Iterator> makeWindowIterator( Iterator begin, std::size_t n ){
        return WindowIterator<Iterator>( begin, n );
    }

    template< typename Iterator >
    inline WindowIterator<Iterator> makeWindowIterator( std::size_t n, Iterator last ){
        return WindowIterator<Iterator>( n, last );
    }

    template< typename Iterator >
    inline auto window( Iterator begin, Iterator end, std::size_t n )
        -> decltype( irange( makeWindowIterator( begin, n ), makeWindowIterator( n, end ) ) )
    {
        return irange( makeWindowIterator( begin, n ), makeWindowIterator( n, end ) );
    }

    template< typename Range >
    inline auto window( Range& r, std::size_t n )
        -> decltype( window( std::begin( r ), std::end( r ), n ) )
    {
        return window( std::begin( r ), std::end( r ), n );
    }

} //namespace itertools

#endif
