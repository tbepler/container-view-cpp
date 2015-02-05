#ifndef INCLUDED_BEPLER_ITERTOOLS_POINTER_FACADE_H
#define INCLUDED_BEPLER_ITERTOOLS_POINTER_FACADE_H

namespace itertools{

    template< typename T >
    class PointerFacade{
        T val_;
        public:
            PointerFacade( const T& val ) : val_( val ) { }
            T* operator->() { return &val_; }
            const T* operator->() const { return &val_; }
            T& operator*() { return val_; }
            const T& operator*() { return val_; }
    };

} //namespace itertools

#endif
