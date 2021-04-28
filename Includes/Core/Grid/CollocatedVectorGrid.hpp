// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_COLLOCATED_VECTOR_GRID_HPP
#define CUBBYFLOW_COLLOCATED_VECTOR_GRID_HPP

#include <Core/Array/Array.hpp>
#include <Core/Array/ArraySamplers.hpp>
#include <Core/Grid/VectorGrid.hpp>

namespace CubbyFlow
{
//! \brief Abstract base class for N-D collocated vector grid structure.
template <size_t N>
class CollocatedVectorGrid : public VectorGrid<N>
{
 public:
    using typename VectorGrid<N>::VectorDataView;
    using typename VectorGrid<N>::ConstVectorDataView;
    using VectorGrid<N>::GridSpacing;

    //! Constructs an empty grid.
    CollocatedVectorGrid();

    //! Default virtual destructor.
    ~CollocatedVectorGrid() override = default;

    //! Default copy constructor.
    CollocatedVectorGrid(const CollocatedVectorGrid& other) = default;

    //! Default move constructor.
    CollocatedVectorGrid(CollocatedVectorGrid&& other) noexcept = default;

    //! Default copy assignment operator.
    CollocatedVectorGrid& operator=(const CollocatedVectorGrid& other) =
        default;

    //! Default move assignment operator.
    CollocatedVectorGrid& operator=(CollocatedVectorGrid&& other) noexcept =
        default;

    //! Returns the actual data point size.
    [[nodiscard]] virtual Vector<size_t, N> DataSize() const = 0;

    //!
    //! \brief Returns data position for the grid point at (0, 0, ...).
    //!
    //! Note that this is different from origin() since origin() returns
    //! the lower corner point of the bounding box.
    //!
    [[nodiscard]] virtual Vector<double, N> DataOrigin() const = 0;

    //! Returns the grid data at given data point.
    const Vector<double, N>& operator()(const Vector<size_t, N>& idx) const;

    //! Returns the grid data at given data point.
    Vector<double, N>& operator()(const Vector<size_t, N>& idx);

    //! Returns the grid data at given data point.
    template <typename... Indices>
    const Vector<double, N>& operator()(size_t i, Indices... indices) const
    {
        return (*this)(Vector<size_t, N>(i, indices...));
    }

    //! Returns the grid data at given data point.
    template <typename... Indices>
    Vector<double, N>& operator()(size_t i, Indices... indices)
    {
        return (*this)(Vector<size_t, N>(i, indices...));
    }

    //! Returns divergence at data point location.
    [[nodiscard]] double DivergenceAtDataPoint(
        const Vector<size_t, N>& idx) const;

    //! Returns divergence at data point location.
    template <typename... Indices>
    [[nodiscard]] double DivergenceAtDataPoint(size_t i,
                                               Indices... indices) const
    {
        return DivergenceAtDataPoint(Vector<size_t, N>(i, indices...));
    }

    //! Returns curl at data point location.
    [[nodiscard]] typename GetCurl<N>::Type CurlAtDataPoint(
        const Vector<size_t, N>& idx) const;

    //! Returns curl at data point location.
    template <typename... Indices>
    [[nodiscard]] typename GetCurl<N>::Type CurlAtDataPoint(
        size_t i, Indices... indices) const
    {
        return CurlAtDataPoint(Vector<size_t, N>(i, indices...));
    }

    //! Returns the read-write data array view.
    [[nodiscard]] VectorDataView DataView();

    //! Returns the read-only data array view.
    [[nodiscard]] ConstVectorDataView DataView() const;

    //! Returns the function that maps data point to its position.
    [[nodiscard]] GridDataPositionFunc<N> DataPosition() const;

    //!
    //! \brief Invokes the given function \p func for each data point.
    //!
    //! This function invokes the given function object \p func for each data
    //! point in serial manner. The input parameters are i and j indices of a
    //! data point. The order of execution is i-first, j-last.
    //!
    void ForEachDataPointIndex(
        const std::function<void(const Vector<size_t, N>&)>& func) const;

    template <size_t M = N>
    std::enable_if_t<M == 2, void> ForEachDataPointIndex(
        const std::function<void(size_t, size_t)>& func) const
    {
        ForEachDataPointIndex(
            [&func](const Vector2UZ& idx) { func(idx.x, idx.y); });
    }

    template <size_t M = N>
    std::enable_if_t<M == 3, void> ForEachDataPointIndex(
        const std::function<void(size_t, size_t, size_t)>& func) const
    {
        ForEachDataPointIndex(
            [&func](const Vector3UZ& idx) { func(idx.x, idx.y, idx.z); });
    }

    //!
    //! \brief Invokes the given function \p func for each data point in
    //! parallel.
    //!
    //! This function invokes the given function object \p func for each data
    //! point in parallel manner. The input parameters are i and j indices of a
    //! data point. The order of execution can be arbitrary since it's
    //! multi-threaded.
    //!
    void ParallelForEachDataPointIndex(
        const std::function<void(const Vector<size_t, N>&)>& func) const;

    template <size_t M = N>
    std::enable_if_t<M == 2, void> ParallelForEachDataPointIndex(
        const std::function<void(size_t, size_t)>& func) const
    {
        ParallelForEachDataPointIndex(
            [&func](const Vector2UZ& idx) { func(idx.x, idx.y); });
    }

    template <size_t M = N>
    std::enable_if_t<M == 3, void> ParallelForEachDataPointIndex(
        const std::function<void(size_t, size_t, size_t)>& func) const
    {
        ParallelForEachDataPointIndex(
            [&func](const Vector3UZ& idx) { func(idx.x, idx.y, idx.z); });
    }

    //! Returns sampled value at given position \p x.
    [[nodiscard]] Vector<double, N> Sample(
        const Vector<double, N>& x) const override;

    //! Returns divergence at given position \p x.
    [[nodiscard]] double Divergence(const Vector<double, N>& x) const override;

    //! Returns curl at given position \p x.
    [[nodiscard]] typename GetCurl<N>::Type Curl(
        const Vector<double, N>& x) const override;

    //!
    //! \brief Returns the sampler function.
    //!
    //! This function returns the data sampler function object. The sampling
    //! function is linear.
    //!
    [[nodiscard]] std::function<Vector<double, N>(const Vector<double, N>&)>
    Sampler() const override;

 protected:
    using VectorGrid<N>::SwapGrid;
    using VectorGrid<N>::SetGrid;

    //! Swaps the data storage and predefined samplers with given grid.
    void SwapCollocatedVectorGrid(CollocatedVectorGrid* other);

    //! Sets the data storage and predefined samplers with given grid.
    void SetCollocatedVectorGrid(const CollocatedVectorGrid& other);

    //! Fetches the data into a continuous linear array.
    void GetData(Array1<double>& data) const override;

    //! Sets the data from a continuous linear array.
    void SetData(const ConstArrayView1<double>& data) override;

 private:
    Array<Vector<double, N>, N> m_data;
    LinearArraySampler<Vector<double, N>, N> m_linearSampler;
    std::function<Vector<double, N>(const Vector<double, N>&)> m_sampler;

    void OnResize(const Vector<size_t, N>& resolution,
                  const Vector<double, N>& gridSpacing,
                  const Vector<double, N>& origin,
                  const Vector<double, N>& initialValue) final;

    void ResetSampler();
};

//! 2-D CollocatedVectorGrid type.
using CollocatedVectorGrid2 = CollocatedVectorGrid<2>;

//! 3-D CollocatedVectorGrid type.
using CollocatedVectorGrid3 = CollocatedVectorGrid<3>;

//! Shared pointer for the CollocatedVectorGrid2 type.
using CollocatedVectorGrid2Ptr = std::shared_ptr<CollocatedVectorGrid2>;

//! Shared pointer for the CollocatedVectorGrid3 type.
using CollocatedVectorGrid3Ptr = std::shared_ptr<CollocatedVectorGrid3>;
}  // namespace CubbyFlow

#endif