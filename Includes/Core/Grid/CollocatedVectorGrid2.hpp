// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_COLLOCATED_VECTOR_GRID2_HPP
#define CUBBYFLOW_COLLOCATED_VECTOR_GRID2_HPP

#include <Core/Array/Array.hpp>
#include <Core/Array/ArraySamplers.hpp>
#include <Core/Grid/VectorGrid2.hpp>

namespace CubbyFlow
{
//! \brief Abstract base class for 2-D collocated vector grid structure.
class CollocatedVectorGrid2 : public VectorGrid2
{
 public:
    //! Constructs an empty grid.
    CollocatedVectorGrid2();

    //! Default copy constructor.
    CollocatedVectorGrid2(const CollocatedVectorGrid2&) = default;

    //! Default move constructor.
    CollocatedVectorGrid2(CollocatedVectorGrid2&&) noexcept = default;

    //! Default virtual destructor.
    ~CollocatedVectorGrid2() override = default;

    //! Default copy assignment operator.
    CollocatedVectorGrid2& operator=(const CollocatedVectorGrid2&) = default;

    //! Default move assignment operator.
    CollocatedVectorGrid2& operator=(CollocatedVectorGrid2&&) noexcept =
        default;

    //! Returns the actual data point size.
    [[nodiscard]] virtual Vector2UZ GetDataSize() const = 0;

    //!
    //! \brief Returns data position for the grid point at (0, 0).
    //!
    //! Note that this is different from origin() since origin() returns
    //! the lower corner point of the bounding box.
    //!
    [[nodiscard]] virtual Vector2D GetDataOrigin() const = 0;

    //! Returns the grid data at given data point.
    const Vector2D& operator()(size_t i, size_t j) const;

    //! Returns the grid data at given data point.
    Vector2D& operator()(size_t i, size_t j);

    //! Returns divergence at data point location.
    [[nodiscard]] double DivergenceAtDataPoint(size_t i, size_t j) const;

    //! Returns curl at data point location.
    [[nodiscard]] double CurlAtDataPoint(size_t i, size_t j) const;

    //! Returns the read-write data array view.
    [[nodiscard]] VectorDataView DataView();

    //! Returns the read-only data array view.
    [[nodiscard]] ConstVectorDataView DataView() const;

    //! Returns the function that maps data point to its position.
    [[nodiscard]] DataPositionFunc DataPosition() const;

    //!
    //! \brief Invokes the given function \p func for each data point.
    //!
    //! This function invokes the given function object \p func for each data
    //! point in serial manner. The input parameters are i and j indices of a
    //! data point. The order of execution is i-first, j-last.
    //!
    void ForEachDataPointIndex(
        const std::function<void(size_t, size_t)>& func) const;

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
        const std::function<void(size_t, size_t)>& func) const;

    // VectorField2 implementations
    //! Returns sampled value at given position \p x.
    [[nodiscard]] Vector2D Sample(const Vector2D& x) const override;

    //! Returns divergence at given position \p x.
    [[nodiscard]] double Divergence(const Vector2D& x) const override;

    //! Returns curl at given position \p x.
    [[nodiscard]] double Curl(const Vector2D& x) const override;

    //!
    //! \brief Returns the sampler function.
    //!
    //! This function returns the data sampler function object. The sampling
    //! function is linear.
    //!
    [[nodiscard]] std::function<Vector2D(const Vector2D&)> Sampler()
        const override;

 protected:
    //! Swaps the data storage and predefined samplers with given grid.
    void SwapCollocatedVectorGrid(CollocatedVectorGrid2* other);

    //! Sets the data storage and predefined samplers with given grid.
    void SetCollocatedVectorGrid(const CollocatedVectorGrid2& other);

    //! Fetches the data into a continuous linear array.
    void GetData(Array1<double>& data) const override;

    //! Sets the data from a continuous linear array.
    void SetData(const ConstArrayView1<double>& data) override;

 private:
    void OnResize(const Vector2UZ& resolution, const Vector2D& gridSpacing,
                  const Vector2D& origin, const Vector2D& initialValue) final;

    void ResetSampler();

    Array2<Vector2D> m_data;
    LinearArraySampler2<Vector2D> m_linearSampler;
    std::function<Vector2D(const Vector2D&)> m_sampler;
};

//! Shared pointer for the CollocatedVectorGrid2 type.
using CollocatedVectorGrid2Ptr = std::shared_ptr<CollocatedVectorGrid2>;
}  // namespace CubbyFlow

#endif