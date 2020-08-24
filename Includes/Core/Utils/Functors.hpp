// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FUNCTORS_HPP
#define CUBBYFLOW_FUNCTORS_HPP

namespace CubbyFlow
{
//! Type casting operator.
template <typename T, typename U>
struct TypeCast
{
    constexpr U operator()(const T& a) const;
};

//! Reverse minus operator.
template <typename T>
struct RMinus
{
    constexpr T operator()(const T& a, const T& b) const;
};

//! Reverse divides operator.
template <typename T>
struct RDivides
{
    constexpr T operator()(const T& a, const T& b) const;
};
}  // namespace CubbyFlow

#include <Core/Utils/Functors-Impl.hpp>

#endif