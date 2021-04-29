// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_SAMPLERS_HPP
#define CUBBYFLOW_ARRAY_SAMPLERS_HPP

#include <Core/Array/ArrayView.hpp>

#include <functional>

namespace CubbyFlow
{
//!
//! \brief N-D nearest array sampler class.
//!
//! This class provides nearest sampling interface for a given N-D array.
//!
//! \tparam T - The value type to sample.
//! \tparam N - Dimension.
//!
template <typename T, size_t N>
class NearestArraySampler final
{
 public:
    static_assert(N > 0, "Dimension should be greater than 0");

    using ScalarType = typename GetScalarType<T>::value;

    static_assert(std::is_floating_point<ScalarType>::value,
                  "NearestArraySampler only can be instantiated with floating "
                  "point types");

    using VectorType = Vector<ScalarType, N>;
    using CoordIndexType = Vector<size_t, N>;

    //! Default constructor.
    NearestArraySampler() = default;

    //!
    //! \brief      Constructs a sampler.
    //!
    //! \param[in]  view        The array view.
    //! \param[in]  gridSpacing The grid spacing.
    //! \param[in]  gridOrigin  The grid origin.
    //!
    explicit NearestArraySampler(const ArrayView<const T, N>& view,
                                 const VectorType& gridSpacing,
                                 const VectorType& gridOrigin);

    //! Default destructor.
    ~NearestArraySampler() = default;

    //! Copy constructor.
    NearestArraySampler(const NearestArraySampler& other);

    //! Move constructor.
    NearestArraySampler(NearestArraySampler&& other) noexcept;

    //! Copy assignment operator.
    NearestArraySampler& operator=(const NearestArraySampler& other);

    //! Move assignment operator.
    NearestArraySampler& operator=(NearestArraySampler&& other) noexcept;

    //! Returns sampled value at point \p pt.
    T operator()(const VectorType& pt) const;

    //! Returns the nearest array index for point \p pt.
    CoordIndexType GetCoordinate(const VectorType& pt) const;

    //! Returns a std::function object that wraps this instance.
    [[nodiscard]] std::function<T(const VectorType&)> Functor() const;

 private:
    ArrayView<const T, N> m_view;
    VectorType m_gridSpacing;
    VectorType m_invGridSpacing;
    VectorType m_gridOrigin;
};

template <typename T>
using NearestArraySampler1 = NearestArraySampler<T, 1>;

template <typename T>
using NearestArraySampler2 = NearestArraySampler<T, 2>;

template <typename T>
using NearestArraySampler3 = NearestArraySampler<T, 3>;

//!
//! \brief N-D array sampler using linear interpolation.
//!
//! This class provides linear sampling interface for a given N-D array.
//!
//! \tparam T - The value type to sample.
//! \tparam N - Dimension.
//!
template <typename T, size_t N>
class LinearArraySampler final
{
 public:
    static_assert(N > 0, "N should be greater than 0");

    using ScalarType = typename GetScalarType<T>::value;

    static_assert(std::is_floating_point<ScalarType>::value,
                  "LinearArraySampler only can be instantiated with floating "
                  "point types");

    using VectorType = Vector<ScalarType, N>;
    using CoordIndexType = Vector<size_t, N>;

    static constexpr size_t FLAT_KERNEL_SIZE = 1 << N;

    //! Default constructor.
    LinearArraySampler() = default;

    //!
    //! \brief      Constructs a sampler.
    //!
    //! \param[in]  view        The array view.
    //! \param[in]  gridSpacing The grid spacing.
    //! \param[in]  gridOrigin  The grid origin.
    //!
    explicit LinearArraySampler(const ArrayView<const T, N>& view,
                                const VectorType& gridSpacing,
                                const VectorType& gridOrigin);

    //! Default destructor.
    ~LinearArraySampler() = default;

    //! Copy constructor.
    LinearArraySampler(const LinearArraySampler& other);

    //! Move constructor.
    LinearArraySampler(LinearArraySampler&& other) noexcept;

    //! Copy assignment operator.
    LinearArraySampler& operator=(const LinearArraySampler& other);

    //! Move assignment operator.
    LinearArraySampler& operator=(LinearArraySampler&& other) noexcept;

    //! Returns sampled value at point \p pt.
    T operator()(const VectorType& pt) const;

    //! Returns the indices of points and their sampling weight for given point.
    void GetCoordinatesAndWeights(
        const VectorType& pt,
        std::array<CoordIndexType, FLAT_KERNEL_SIZE>& indices,
        std::array<ScalarType, FLAT_KERNEL_SIZE>& weights) const;

    //! Returns the indices of points and their gradient of sampling weight for
    //! given point.
    void GetCoordinatesAndGradientWeights(
        const VectorType& pt,
        std::array<CoordIndexType, FLAT_KERNEL_SIZE>& indices,
        std::array<VectorType, FLAT_KERNEL_SIZE>& weights) const;

    //! Returns a std::function instance that wraps this instance.
    [[nodiscard]] std::function<T(const VectorType&)> Functor() const;

 private:
    ArrayView<const T, N> m_view;
    VectorType m_gridSpacing;
    VectorType m_invGridSpacing;
    VectorType m_gridOrigin;
};

template <typename T>
using LinearArraySampler1 = LinearArraySampler<T, 1>;

template <typename T>
using LinearArraySampler2 = LinearArraySampler<T, 2>;

template <typename T>
using LinearArraySampler3 = LinearArraySampler<T, 3>;

//!
//! \brief N-D cubic array sampler class.
//!
//! This class provides cubic sampling interface for a given N-D array.
//!
//! \tparam T - The value type to sample.
//! \tparam N - Dimension.
//!
template <typename T, size_t N, typename CubicInterpolationOp>
class CubicArraySampler final
{
 public:
    static_assert(N > 0, "N should be greater than 0");

    using ScalarType = typename GetScalarType<T>::value;

    static_assert(std::is_floating_point<ScalarType>::value,
                  "CubicArraySampler only can be instantiated with floating "
                  "point types");

    using VectorType = Vector<ScalarType, N>;
    using CoordIndexType = Vector<size_t, N>;

    //! Default constructor.
    CubicArraySampler() = default;

    //!
    //! \brief      Constructs a sampler.
    //!
    //! \param[in]  view        The array view.
    //! \param[in]  gridSpacing The grid spacing.
    //! \param[in]  gridOrigin  The grid origin.
    //!
    explicit CubicArraySampler(const ArrayView<const T, N>& view,
                               const VectorType& gridSpacing,
                               const VectorType& gridOrigin);

    //! Default destructor.
    ~CubicArraySampler() = default;

    //! Copy constructor.
    CubicArraySampler(const CubicArraySampler& other);

    //! Move constructor.
    CubicArraySampler(CubicArraySampler&& other) noexcept;

    //! Copy assignment operator.
    CubicArraySampler& operator=(const CubicArraySampler& other);

    //! Move assignment operator.
    CubicArraySampler& operator=(CubicArraySampler&& other) noexcept;

    //! Returns sampled value at point \p pt.
    T operator()(const VectorType& pt) const;

    //! Returns a std::function object that wraps this instance.
    [[nodiscard]] std::function<T(const VectorType&)> Functor() const;

 private:
    ArrayView<const T, N> m_view;
    VectorType m_gridSpacing;
    VectorType m_invGridSpacing;
    VectorType m_gridOrigin;
};

template <typename T>
struct DoCatmullRom
{
    using ScalarType = typename GetScalarType<T>::value;

    T operator()(const T& f0, const T& f1, const T& f2, const T& f3,
                 ScalarType t) const
    {
        return CatmullRom(f0, f1, f2, f3, t);
    }
};

template <typename T>
struct DoMonotonicCatmullRom
{
    using ScalarType = typename GetScalarType<T>::value;

    T operator()(const T& f0, const T& f1, const T& f2, const T& f3,
                 ScalarType t) const
    {
        return MonotonicCatmullRom(f0, f1, f2, f3, t);
    }
};

template <typename T>
using CatmullRomArraySampler1 = CubicArraySampler<T, 1, DoCatmullRom<T>>;

template <typename T>
using CatmullRomArraySampler2 = CubicArraySampler<T, 2, DoCatmullRom<T>>;

template <typename T>
using CatmullRomArraySampler3 = CubicArraySampler<T, 3, DoCatmullRom<T>>;

template <typename T>
using MonotonicCatmullRomArraySampler1 =
    CubicArraySampler<T, 1, DoMonotonicCatmullRom<T>>;

template <typename T>
using MonotonicCatmullRomArraySampler2 =
    CubicArraySampler<T, 2, DoMonotonicCatmullRom<T>>;

template <typename T>
using MonotonicCatmullRomArraySampler3 =
    CubicArraySampler<T, 3, DoMonotonicCatmullRom<T>>;
}  // namespace CubbyFlow

#include <Core/Array/ArraySamplers-Impl.hpp>

#endif