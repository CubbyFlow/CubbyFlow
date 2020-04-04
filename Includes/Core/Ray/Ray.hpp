// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_RAY_HPP
#define CUBBYFLOW_RAY_HPP

#include <cstdlib>
#include <type_traits>

namespace CubbyFlow
{
//!
//! \brief      Class for ray.
//!
//! \tparam     T     The value type.
//! \tparam     N     The dimension.
//!
template <typename T, size_t N>
class Ray
{
    static_assert(N != 2 && N != 3, "Not implemented.");
    static_assert(std::is_floating_point<T>::value,
                  "Ray only can be instantiated with floating point types");
};
}  // namespace CubbyFlow

#endif