// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_ACCESSOR_HPP
#define CUBBYFLOW_ARRAY_ACCESSOR_HPP

#include <cstddef>

namespace CubbyFlow
{
//!
//! \brief Generic N-dimensional array accessor class interface.
//!
//! This class provides generic template class for N-dimensional array accessor
//! where N must be either 1, 2 or 3. This particular class exists to provide
//! generic interface for 1, 2 or 3 dimensional arrays using template
//! specialization only, but it cannot create any instance by itself.
//! Array accessor provides array-like data read/write functions, but does not
//! handle memory management. Thus, it is more like a random access iterator,
//! but with multi-dimension support.
//!
//! \tparam T - Real number type.
//! \tparam N - Dimension.
//!
template <typename T, size_t N>
class ArrayAccessor final
{
 public:
    static_assert(N < 1 || N > 3,
                  "Not implemented - N should be either 1, 2 or 3.");
};

//!
//! \brief Generic N-dimensional read-only array accessor class interface.
//!
//! This class provides generic template class for N-dimensional read-only array
//! accessor where N must be either 1, 2 or 3. This particular class exists to
//! provide generic interface for 1, 2 or 3 dimensional arrays using template
//! specialization only, but it cannot create any instance by itself.
//! Const array accessor provides array-like data read functions, but does not
//! handle memory management. Thus, it is more like a random access iterator,
//! but with multi-dimension support.
//!
//! \tparam T - Real number type.
//! \tparam N - Dimension.
//!
template <typename T, size_t N>
class ConstArrayAccessor final
{
 public:
    static_assert(N < 1 || N > 3,
                  "Not implemented - N should be either 1, 2 or 3.");
};

}  // namespace CubbyFlow

#endif