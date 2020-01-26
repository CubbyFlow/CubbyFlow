// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_SAMPLERS3_HPP
#define CUBBYFLOW_ARRAY_SAMPLERS3_HPP

#include <Core/Vector/Vector3.h>
#include <Core/Array/ArrayAccessor3.hpp>
#include <Core/Array/ArraySamplers.hpp>

#include <functional>

namespace CubbyFlow
{
//!
//! \brief 3-D nearest array sampler class.
//!
//! This class provides nearest sampling interface for a given 3-D array.
//!
//! \tparam T - The value type to sample.
//! \tparam R - The real number type.
//!
template <typename T, typename R>
class NearestArraySampler<T, R, 3> final
{
 public:
    static_assert(
        std::is_floating_point<R>::value,
        "Samplers only can be instantiated with floating point types");

    //!
    //! \brief      Constructs a sampler using array accessor, spacing between
    //!     the elements, and the position of the first array element.
    //!
    //! \param[in]  accessor    The array accessor.
    //! \param[in]  gridSpacing The grid spacing.
    //! \param[in]  gridOrigin  The grid origin.
    //!
    explicit NearestArraySampler(const ConstArrayAccessor3<T>& accessor,
                                 const Vector3<R>& gridSpacing,
                                 const Vector3<R>& gridOrigin);

    //! Copy constructor.
    NearestArraySampler(const NearestArraySampler& other);

    //! Returns sampled value at point \p pt.
    T operator()(const Vector3<R>& pt) const;

    //! Returns the nearest array index for point \p x.
    void GetCoordinate(const Vector3<R>& pt, Point3UI* index) const;

    //! Returns a function object that wraps this instance.
    std::function<T(const Vector3<R>&)> Functor() const;

 private:
    Vector3<R> m_gridSpacing;
    Vector3<R> m_origin;
    ConstArrayAccessor3<T> m_accessor;
};

//! Type alias for 3-D nearest array sampler.
template <typename T, typename R>
using NearestArraySampler3 = NearestArraySampler<T, R, 3>;

//!
//! \brief 3-D linear array sampler class.
//!
//! This class provides linear sampling interface for a given 3-D array.
//!
//! \tparam T - The value type to sample.
//! \tparam R - The real number type.
//!
template <typename T, typename R>
class LinearArraySampler<T, R, 3> final
{
 public:
    static_assert(
        std::is_floating_point<R>::value,
        "Samplers only can be instantiated with floating point types");

    //!
    //! \brief      Constructs a sampler using array accessor, spacing between
    //!     the elements, and the position of the first array element.
    //!
    //! \param[in]  accessor    The array accessor.
    //! \param[in]  gridSpacing The grid spacing.
    //! \param[in]  gridOrigin  The grid origin.
    //!
    explicit LinearArraySampler(const ConstArrayAccessor3<T>& accessor,
                                const Vector3<R>& gridSpacing,
                                const Vector3<R>& gridOrigin);

    //! Copy constructor.
    LinearArraySampler(const LinearArraySampler& other);

    //! Returns sampled value at point \p pt.
    T operator()(const Vector3<R>& pt) const;

    //! Returns the indices of points and their sampling weight for given point.
    void GetCoordinatesAndWeights(const Vector3<R>& pt,
                                  std::array<Point3UI, 8>* indices,
                                  std::array<R, 8>* weights) const;

    //! Returns the indices of points and their gradient of sampling weight for
    //! given point.
    void GetCoordinatesAndGradientWeights(
        const Vector3<R>& pt, std::array<Point3UI, 8>* indices,
        std::array<Vector3<R>, 8>* weights) const;

    //! Returns a function object that wraps this instance.
    std::function<T(const Vector3<R>&)> Functor() const;

 private:
    Vector3<R> m_gridSpacing;
    Vector3<R> m_invGridSpacing;
    Vector3<R> m_origin;
    ConstArrayAccessor3<T> m_accessor;
};

//! Type alias for 3-D linear array sampler.
template <typename T, typename R>
using LinearArraySampler3 = LinearArraySampler<T, R, 3>;

//!
//! \brief 3-D cubic array sampler class.
//!
//! This class provides cubic sampling interface for a given 3-D array.
//!
//! \tparam T - The value type to sample.
//! \tparam R - The real number type.
//!
template <typename T, typename R>
class CubicArraySampler<T, R, 3> final
{
 public:
    static_assert(
        std::is_floating_point<R>::value,
        "Samplers only can be instantiated with floating point types");

    //!
    //! \brief      Constructs a sampler using array accessor, spacing between
    //!     the elements, and the position of the first array element.
    //!
    //! \param[in]  accessor    The array accessor.
    //! \param[in]  gridSpacing The grid spacing.
    //! \param[in]  gridOrigin  The grid origin.
    //!
    explicit CubicArraySampler(const ConstArrayAccessor3<T>& accessor,
                               const Vector3<R>& gridSpacing,
                               const Vector3<R>& gridOrigin);

    //! Copy constructor.
    CubicArraySampler(const CubicArraySampler& other);

    //! Returns sampled value at point \p pt.
    T operator()(const Vector3<R>& pt) const;

    //! Returns a function object that wraps this instance.
    std::function<T(const Vector3<R>&)> Functor() const;

 private:
    Vector3<R> m_gridSpacing;
    Vector3<R> m_origin;
    ConstArrayAccessor3<T> m_accessor;
};

//! Type alias for 3-D cubic array sampler.
template <typename T, typename R>
using CubicArraySampler3 = CubicArraySampler<T, R, 3>;
}  // namespace CubbyFlow

#include <Core/Array/ArraySamplers3-Impl.hpp>

#endif