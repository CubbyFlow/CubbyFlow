// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_COLLOCATED_VECTOR_GRID3_HPP
#define CUBBYFLOW_COLLOCATED_VECTOR_GRID3_HPP

#include <Core/Array/Array3.hpp>
#include <Core/Array/ArraySamplers3.hpp>
#include <Core/Grid/VectorGrid3.hpp>

namespace CubbyFlow
{
//! \brief Abstract base class for 3-D collocated vector grid structure.
class CollocatedVectorGrid3 : public VectorGrid3
{
 public:
    //! Constructs an empty grid.
    CollocatedVectorGrid3();

    //! Default copy constructor.
    CollocatedVectorGrid3(const CollocatedVectorGrid3&) = default;

    //! Default move constructor.
    CollocatedVectorGrid3(CollocatedVectorGrid3&&) noexcept = default;

    //! Default virtual destructor.
    ~CollocatedVectorGrid3() override = default;

    //! Default copy assignment operator.
    CollocatedVectorGrid3& operator=(const CollocatedVectorGrid3&) = default;

    //! Default move assignment operator.
    CollocatedVectorGrid3& operator=(CollocatedVectorGrid3&&) noexcept =
        default;

    //! Returns the actual data point size.
    [[nodiscard]] virtual Size3 GetDataSize() const = 0;

    //!
    //! \brief Returns data position for the grid point at (0, 0, 0).
    //!
    //! Note that this is different from origin() since origin() returns
    //! the lower corner point of the bounding box.
    //!
    [[nodiscard]] virtual Vector3D GetDataOrigin() const = 0;

    //! Returns the grid data at given data point.
    const Vector3D& operator()(size_t i, size_t j, size_t k) const;

    //! Returns the grid data at given data point.
    Vector3D& operator()(size_t i, size_t j, size_t k);

    //! Returns divergence at data point location.
    [[nodiscard]] double DivergenceAtDataPoint(size_t i, size_t j,
                                               size_t k) const;

    //! Returns curl at data point location.
    [[nodiscard]] Vector3D CurlAtDataPoint(size_t i, size_t j, size_t k) const;

    //! Returns the read-write data array accessor.
    [[nodiscard]] VectorDataAccessor GetDataAccessor();

    //! Returns the read-only data array accessor.
    [[nodiscard]] ConstVectorDataAccessor GetConstDataAccessor() const;

    //! Returns the function that maps data point to its position.
    [[nodiscard]] DataPositionFunc GetDataPosition() const;

    //!
    //! \brief Invokes the given function \p func for each data point.
    //!
    //! This function invokes the given function object \p func for each data
    //! point in serial manner. The input parameters are i and j indices of a
    //! data point. The order of execution is i-first, j-last.
    //!
    void ForEachDataPointIndex(
        const std::function<void(size_t, size_t, size_t)>& func) const;

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
        const std::function<void(size_t, size_t, size_t)>& func) const;

    // VectorField3 implementations
    //! Returns sampled value at given position \p x.
    [[nodiscard]] Vector3D Sample(const Vector3D& x) const override;

    //! Returns divergence at given position \p x.
    [[nodiscard]] double Divergence(const Vector3D& x) const override;

    //! Returns curl at given position \p x.
    [[nodiscard]] Vector3D Curl(const Vector3D& x) const override;

    //!
    //! \brief Returns the sampler function.
    //!
    //! This function returns the data sampler function object. The sampling
    //! function is linear.
    //!
    [[nodiscard]] std::function<Vector3D(const Vector3D&)> Sampler()
        const override;

 protected:
    //! Swaps the data storage and predefined samplers with given grid.
    void SwapCollocatedVectorGrid(CollocatedVectorGrid3* other);

    //! Sets the data storage and predefined samplers with given grid.
    void SetCollocatedVectorGrid(const CollocatedVectorGrid3& other);

    //! Fetches the data into a continuous linear array.
    void GetData(std::vector<double>* data) const override;

    //! Sets the data from a continuous linear array.
    void SetData(const std::vector<double>& data) override;

 private:
    void OnResize(const Size3& resolution, const Vector3D& gridSpacing,
                  const Vector3D& origin, const Vector3D& initialValue) final;

    void ResetSampler();

    Array3<Vector3D> m_data;
    LinearArraySampler3<Vector3D, double> m_linearSampler;
    std::function<Vector3D(const Vector3D&)> m_sampler;
};

//! Shared pointer for the CollocatedVectorGrid3 type.
using CollocatedVectorGrid3Ptr = std::shared_ptr<CollocatedVectorGrid3>;
}  // namespace CubbyFlow

#endif