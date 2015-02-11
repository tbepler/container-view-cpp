#ifndef INCLUDED_BEPLER_ITERTOOLS_TRANSFORM_H
#define INCLUDED_BEPLER_ITERTOOLS_TRANSFORM_H

#include <iterator>
#include "bepler/itertools/iterator_base.h"

namespace itertools{

    template<
        typename InputIterator,
        typename Function,
        typename iter_t = typename std::iterator_traits<InputIterator>::iterator_category
    >
    class TransformIterator;

} //namespace itertools

#endif
