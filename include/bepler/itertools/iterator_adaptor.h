#ifndef INCLUDED_BEPLER_ITERTOOLS_ITERATOR_ADAPTOR_H
#define INCLUDED_BEPLER_ITERTOOLS_ITERATOR_ADAPTOR_H

#include <iterator>
#include "bepler/type_traits/iterator_traits.h"
#include "bepler/itertools/iterator_base.h"

namespace itertools{
    
    template<
        typename Derived,
        typename Iterator,
        typename Value = typename std::iterator_traits<Iterator>::value_type,
        typename Reference = typename std::iterator_traits<Iterator>::reference,
        typename Difference = typename std::iterator_traits<Iterator>::difference_type,
        typename Category = typename std::iterator_traits<Iterator>::iterator_category
    >
    class IteratorAdaptor : public iterator_base< Derived, Category, Value, Reference, Difference > {

        friend class iterator_base< Derived, Category, Value, Reference, Difference >::base;
        public:
            typedef typename iterator_base< Derived, Category, Value, Reference, Difference >::base base;

        private:
        Iterator iter_;

        inline Reference dereference() const{ return *iter_; }
        inline void inc(){ ++iter_; }

        template< typename D, typename I, typename V, typename R, typename Diff, typename C >
        inline bool equals( const IteratorAdaptor<D,I,V,R,Diff,C>& rhs ) const{
            return iter_ == rhs.iter();
        }

        template< typename I >
        inline auto equals( const I& rhs ) const
            -> decltype( iter_ == rhs )
        { return iter_ == rhs; }

        typename std::enable_if<
            type_traits::is_bidirectional_iterator<Iterator>::value,
            void
        >::type inline dec(){ --iter_; }

        template< typename D, typename I, typename V, typename R, typename Diff, typename C >
        typename std::enable_if<
            type_traits::is_random_access_iterator<Iterator>::value,
            typename base::difference_type
        >::type inline compareTo( const IteratorAdaptor<D,I,V,R,Diff,C>& rhs ) const{
            return iter_ - rhs.iter();
        }

        template< typename I >
        typename std::enable_if<
            type_traits::is_random_access_iterator<Iterator>::value,
            typename base::difference_type
        >::type inline compareTo( const I& rhs ) const{ return iter_ - rhs; }

        typename std::enable_if<
            type_traits::is_random_access_iterator<Iterator>::value,
            void
        >::type inline advance( typename base::difference_type n ){
            iter_ += n;
        }

        protected:
            inline Iterator& iter(){ return iter_; }

        public:
            //typedef typename iterator_base< Derived, Category, Value, Reference, Difference >::base base;
            IteratorAdaptor() { }
            explicit IteratorAdaptor( const Iterator& iter ) : iter_( iter ) { }
            inline const Iterator& iter() const{ return iter_; }

    };

    template<
        typename Derived,
        typename Base,
        typename Value = void,
        typename Reference = void,
        typename Difference = void,
    >
    class IteratorAdaptor<Derived,Base,Value,Reference,Difference,std::output_iterator_tag> {

        Base base_;

        protected:
            inline Base& iter(){ return base_; }
            
            template< typename T >
            inline void push( const T& x ){ *base_ = x; }

            inline Derived& derived(){ return static_cast<Derived>(*this); }
            inline const Derived& derived() const{ return static_cast<const Derived>(*this); }

        public:
            typedef std::output_iterator_tag iterator_category;
            typedef void value_type;
            typedef void difference_type;
            typedef void pointer;
            typedef void reference;
            typedef void iterator_type;
            typedef Base base_type;

            IteratorAdaptor() { }
            IteratorAdaptor( const Base& base ) : base_( base ) { }
            IteratorAdaptor( const IteratorAdaptor& rhs ) : base_( rhs.base_ ) { }

            inline const Base& iter() const{ return base_; }

            template< typename T >
            inline Derived& operator= ( const T& x ){ derived().push( x ); return *this; }
            inline Derived& operator*(){ return derived(); }
            inline Derived& operator++(){ return derived(); }
            inline Derived operator++(int){ return derived(); }

    };

} //namespace itertools

#endif
