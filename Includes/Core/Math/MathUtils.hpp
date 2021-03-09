// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MATH_UTILS_HPP
#define CUBBYFLOW_MATH_UTILS_HPP

#include <Core/Utils/Macros.hpp>

#include <cstddef>
#include <limits>
#include <type_traits>

namespace CubbyFlow
{
//!
//! \brief      Returns true if \p x and \p y are similar.
//!
//! \param[in]  x     The first value.
//! \param[in]  y     The second value.
//! \param[in]  eps   The tolerance.
//!
//! \tparam     T     Value type.
//!
//! \return     True if similar.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, bool> Similar(
    T x, T y, T eps = std::numeric_limits<T>::epsilon());

//!
//! \brief      Returns the sign of the value.
//!
//! \param[in]  x     Input value.
//!
//! \tparam     T     Value type.
//!
//! \return     The sign.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> Sign(T x);

//!
//! \brief      Returns the minimum value among three inputs.
//!
//! \param[in]  x     The first value.
//! \param[in]  y     The second value.
//! \param[in]  z     The three value.
//!
//! \tparam     T     Value type.
//!
//! \return     The minimum value.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> Min3(T x, T y, T z);

//!
//! \brief      Returns the maximum value among three inputs.
//!
//! \param[in]  x     The first value.
//! \param[in]  y     The second value.
//! \param[in]  z     The three value.
//!
//! \tparam     T     Value type.
//!
//! \return     The maximum value.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> Max3(T x, T y, T z);

//! Returns minimum among n-elements.
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> MinN(const T* x, size_t n);

//! Returns maximum among n-elements.
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> MaxN(const T* x, size_t n);

//!
//! \brief      Returns the absolute minimum value among the two inputs.
//!
//! \param[in]  x     The first value.
//! \param[in]  y     The second value.
//!
//! \tparam     T     Value type.
//!
//! \return     The absolute minimum.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> AbsMin(T x, T y);

//!
//! \brief      Returns the absolute maximum value among the two inputs.
//!
//! \param[in]  x     The first value.
//! \param[in]  y     The second value.
//!
//! \tparam     T     Value type.
//!
//! \return     The absolute maximum.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> AbsMax(T x, T y);

//! Returns absolute minimum among n-elements.
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> AbsMinN(const T* x, size_t n);

//! Returns absolute maximum among n-elements.
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> AbsMaxN(const T* x, size_t n);

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, size_t> ArgMin2(T x, T y);

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, size_t> ArgMax2(T x, T y);

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, size_t> ArgMin3(T x, T y, T z);

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, size_t> ArgMax3(T x, T y, T z);

//!
//! \brief      Returns the square of \p x.
//!
//! \param[in]  x     The input.
//!
//! \tparam     T     Value type.
//!
//! \return     The squared value.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> Square(T x);

//!
//! \brief      Returns the cubic of \p x.
//!
//! \param[in]  x     The input.
//!
//! \tparam     T     Value type.
//!
//! \return     The cubic of \p x.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> Cubic(T x);

//!
//! \brief      Returns the clamped value.
//!
//! \param[in]  val   The value.
//! \param[in]  low   The low value.
//! \param[in]  high  The high value.
//!
//! \tparam     T     Value type.
//!
//! \return     The clamped value.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> Clamp(T val, T low, T high);

//!
//! \brief      Converts degrees to radians.
//!
//! \param[in]  angleInDegrees The angle in degrees.
//!
//! \tparam     T              Value type.
//!
//! \return     Angle in radians.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> DegreesToRadians(
    T angleInDegrees);

//!
//! \brief      Converts radians to degrees.
//!
//! \param[in]  angleInRadians The angle in radians.
//!
//! \tparam     T              Value type.
//!
//! \return     Angle in degrees.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> RadiansToDegrees(
    T angleInRadians);

//!
//! \brief      Computes the barycentric coordinate.
//!
//! This function computes the barycentric coordinate for given array range as
//! shown below:
//!
//! \code
//!
//! begin              end
//! |----|-x--|----|----|
//!      i
//! t = x - i
//!
//! \endcode
//!
//! For instance, if begin = 4, end = 8, and x = 5.4, output i will be 5 and t
//! will be 0.4.
//!
//! \param[in]  x       The input value.
//! \param[in]  begin   Beginning index of the range.
//! \param[in]  end     End index of the range (exclusive).
//! \param[out] i       The output index between iBegin and iEnd - 2.
//! \param[out] t       The offset from \p i.
//!
//! \tparam     T       Value type.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value> GetBarycentric(T x, size_t begin,
                                                              size_t end,
                                                              size_t& i, T& t);

//!
//! \brief      Computes the barycentric coordinate.
//!
//! This function computes the barycentric coordinate for given array range as
//! shown below:
//!
//! \code
//!
//! begin              end
//! |----|-x--|----|----|
//!      i
//! t = x - i
//!
//! \endcode
//!
//! For instance, if begin = 4, end = 8, and x = 5.4, output i will be 5 and t
//! will be 0.4.
//!
//! \param[in]  x       The input value.
//! \param[in]  begin   Beginning index of the range.
//! \param[in]  end     End index of the range (exclusive).
//! \param[out] i       The output index between iBegin and iEnd - 2.
//! \param[out] t       The offset from \p i.
//!
//! \tparam     T       Value type.
//!
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value> GetBarycentric(T x,
                                                              ssize_t begin,
                                                              ssize_t end,
                                                              ssize_t& i, T& t);

//!
//! \brief      Computes linear interpolation.
//!
//! \param[in]  f0    The first value.
//! \param[in]  f1    The second value.
//! \param[in]  t     Relative offset [0, 1] from the first value.
//!
//! \tparam     S     Input value type.
//! \tparam     T     Offset type.
//!
//! \return     The interpolated value.
//!
template <typename S, typename T>
std::enable_if_t<std::is_arithmetic<T>::value, S> Lerp(const S& f0, const S& f1,
                                                       T t);

//! \brief      Computes bilinear interpolation.
template <typename S, typename T>
std::enable_if_t<std::is_arithmetic<T>::value, S> BiLerp(
    const S& f00, const S& f10, const S& f01, const S& f11, T tx, T ty);

//! \brief      Computes trilinear interpolation.
template <typename S, typename T>
std::enable_if_t<std::is_arithmetic<T>::value, S> TriLerp(
    const S& f000, const S& f100, const S& f010, const S& f110, const S& f001,
    const S& f101, const S& f011, const S& f111, T tx, T ty, T tz);

//! \brief      Computes Catmull-Rom interpolation.
template <typename S, typename T>
std::enable_if_t<std::is_arithmetic<T>::value, S> CatmullRom(const S& f0,
                                                             const S& f1,
                                                             const S& f2,
                                                             const S& f3, T t);

//! \brief      Computes monotonic Catmull-Rom interpolation.
template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T> MonotonicCatmullRom(
    const T& f0, const T& f1, const T& f2, const T& f3, T t);
}  // namespace CubbyFlow

#include <Core/Math/MathUtils-Impl.hpp>

#endif