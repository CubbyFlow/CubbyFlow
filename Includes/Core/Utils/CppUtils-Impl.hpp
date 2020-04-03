// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CPP_UTILS_IMPL_HPP
#define CUBBYFLOW_CPP_UTILS_IMPL_HPP

#include <algorithm>

namespace CubbyFlow
{
// Source code from:
// http://en.cppreference.com/w/cpp/algorithm/lower_bound
template <class ForwardIter, class T, class Compare>
ForwardIter BinaryFind(ForwardIter first, ForwardIter last, const T& value,
                       Compare comp)
{
    // Note: Both type T and the type after ForwardIt is dereferenced
    // must be implicitly convertible to both Type1 and Type2, used in Compare.
    // This is stricter than lower_bound requirement (see above)
    first = std::lower_bound(first, last, value, comp);
    return first != last && !comp(value, *first) ? first : last;
}
}  // namespace CubbyFlow

#endif