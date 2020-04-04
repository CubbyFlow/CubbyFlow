// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PDE_HPP
#define CUBBYFLOW_PDE_HPP

#include <array>

namespace CubbyFlow
{
//!
//! \brief 1st order upwind differencing.
//! d0[1] is the origin.
//!
//! \code
//!   |------|------|
//! d0[0]  d0[1]  d0[2]
//! \endcode
//!
//! Returns two solutions for each side.
//!
template <typename T>
std::array<T, 2> Upwind1(T* d0, T dx);

//!
//! \brief 1st order upwind differencing.
//! d0[1] is the origin.
//!
//! \code
//!   |------|------|
//! d0[0]  d0[1]  d0[2]
//! \endcode
//!
template <typename T>
T Upwind1(T* d0, T dx, bool isDirectionPositive);

//!
//! \brief 2nd order central differencing.
//! d0[1] is the origin.
//!
//! \code
//!   |------|------|
//! d0[0]  d0[1]  d0[2]
//! \endcode
//!
template <typename T>
T CD2(T* d0, T dx);

//!
//! \brief 3rd order ENO.
//! d0[3] is the origin.
//!
//! \code
//!   |------|------|------|------|------|------|
//! d0[0]  d0[1]  d0[2]  d0[3]  d0[4]  d0[5]  d0[6]
//! \endcode
//!
//! Returns two solutions for each side.
//!
template <typename T>
std::array<T, 2> ENO3(T* d0, T dx);

//!
//! \brief 3rd order ENO.
//! d0[3] is the origin.
//!
//! \code
//!   |------|------|------|------|------|------|
//! d0[0]  d0[1]  d0[2]  d0[3]  d0[4]  d0[5]  d0[6]
//! \endcode
//!
//!
template <typename T>
T ENO3(T* d0, T dx, bool isDirectionPositive);

//!
//! \brief 5th order WENO.
//! d0[3] is the origin.
//!
//! \code
//!   |------|------|------|------|------|------|
//! d0[0]  d0[1]  d0[2]  d0[3]  d0[4]  d0[5]  d0[6]
//! \endcode
//!
//! Returns two solutions for each side.
//!
//!
template <typename T>
std::array<T, 2> WENO5(T* v, T h, T eps = 1.0e-8);

//!
//! \brief 5th order WENO.
//! d0[3] is the origin.
//!
//! \code
//!   |------|------|------|------|------|------|
//! d0[0]  d0[1]  d0[2]  d0[3]  d0[4]  d0[5]  d0[6]
//! \endcode
//!
template <typename T>
T WENO5(T* v, T h, bool is_velocity_positive, T eps = 1.0e-8);
}  // namespace CubbyFlow

#include <Core/Math/PDE-Impl.hpp>

#endif