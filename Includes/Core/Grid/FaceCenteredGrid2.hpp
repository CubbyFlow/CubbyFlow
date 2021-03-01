// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FACE_CENTERED_GRID2_HPP
#define CUBBYFLOW_FACE_CENTERED_GRID2_HPP

#include <Core/Array/Array.hpp>
#include <Core/Array/ArraySamplers.hpp>
#include <Core/Grid/VectorGrid2.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D face-centered (a.k.a MAC or staggered) grid.
//!
//! This class implements face-centered grid which is also known as
//! marker-and-cell (MAC) or staggered grid. This vector grid stores each vector
//! component at face center. Thus, u and v components are not collocated.
//!
class FaceCenteredGrid2 final : public VectorGrid2
{
 public:
    CUBBYFLOW_GRID2_TYPE_NAME(FaceCenteredGrid2)

    class Builder;

    //! Read-write scalar data view type.
    using ScalarDataView = ArrayView2<double>;

    //! Read-only scalar data view type.
    using ConstScalarDataView = ConstArrayView2<double>;

    //! Constructs empty grid.
    FaceCenteredGrid2();

    //! Resizes the grid using given parameters.
    FaceCenteredGrid2(size_t resolutionX, size_t resolutionY,
                      double gridSpacingX = 1.0, double gridSpacingY = 1.0,
                      double originX = 0.0, double originY = 0.0,
                      double initialValueU = 0.0, double initialValueV = 0.0);

    //! Resizes the grid using given parameters.
    FaceCenteredGrid2(const Vector2UZ& resolution,
                      const Vector2D& gridSpacing = Vector2D{ 1.0, 1.0 },
                      const Vector2D& origin = Vector2D{},
                      const Vector2D& initialValue = Vector2D{});

    //! Copy constructor.
    FaceCenteredGrid2(const FaceCenteredGrid2& other);

    //! Deleted move constructor.
    FaceCenteredGrid2(FaceCenteredGrid2&&) noexcept = delete;

    //! Default virtual destructor.
    ~FaceCenteredGrid2() override = default;

    //! Copy assignment operator.
    FaceCenteredGrid2& operator=(const FaceCenteredGrid2& other);

    //! Deleted move assignment operator.
    FaceCenteredGrid2& operator=(FaceCenteredGrid2&&) noexcept = delete;

    //!
    //! \brief Swaps the contents with the given \p other grid.
    //!
    //! This function swaps the contents of the grid instance with the given
    //! grid object \p other only if \p other has the same type with this grid.
    //!
    void Swap(Grid2* other) override;

    //! Sets the contents with the given \p other grid.
    void Set(const FaceCenteredGrid2& other);

    //! Returns u-value at given data point.
    [[nodiscard]] double& GetU(size_t i, size_t j);

    //! Returns u-value at given data point.
    [[nodiscard]] const double& GetU(size_t i, size_t j) const;

    //! Returns v-value at given data point.
    [[nodiscard]] double& GetV(size_t i, size_t j);

    //! Returns v-value at given data point.
    [[nodiscard]] const double& GetV(size_t i, size_t j) const;

    //! Returns interpolated value at cell center.
    [[nodiscard]] Vector2D ValueAtCellCenter(size_t i, size_t j) const;

    //! Returns divergence at cell-center location.
    [[nodiscard]] double DivergenceAtCellCenter(size_t i, size_t j) const;

    //! Returns curl at cell-center location.
    [[nodiscard]] double CurlAtCellCenter(size_t i, size_t j) const;

    //! Returns u data view.
    [[nodiscard]] ScalarDataView UView();

    //! Returns read-only u data view.
    [[nodiscard]] ConstScalarDataView UView() const;

    //! Returns v data view.
    [[nodiscard]] ScalarDataView VView();

    //! Returns read-only v data view.
    [[nodiscard]] ConstScalarDataView VView() const;

    //! Returns function object that maps u data point to its actual position.
    [[nodiscard]] DataPositionFunc UPosition() const;

    //! Returns function object that maps v data point to its actual position.
    [[nodiscard]] DataPositionFunc VPosition() const;

    //! Returns data size of the u component.
    [[nodiscard]] Vector2UZ USize() const;

    //! Returns data size of the v component.
    [[nodiscard]] Vector2UZ VSize() const;

    //!
    //! \brief Returns u-data position for the grid point at (0, 0).
    //!
    //! Note that this is different from origin() since origin() returns
    //! the lower corner point of the bounding box.
    //!
    [[nodiscard]] Vector2D UOrigin() const;

    //!
    //! \brief Returns v-data position for the grid point at (0, 0).
    //!
    //! Note that this is different from origin() since origin() returns
    //! the lower corner point of the bounding box.
    //!
    [[nodiscard]] Vector2D VOrigin() const;

    //! Fills the grid with given value.
    void Fill(const Vector2D& value,
              ExecutionPolicy policy = ExecutionPolicy::Parallel) override;

    //! Fills the grid with given function.
    void Fill(const std::function<Vector2D(const Vector2D&)>& func,
              ExecutionPolicy policy = ExecutionPolicy::Parallel) override;

    //! Returns the copy of the grid instance.
    [[nodiscard]] std::shared_ptr<VectorGrid2> Clone() const override;

    //!
    //! \brief Invokes the given function \p func for each u-data point.
    //!
    //! This function invokes the given function object \p func for each u-data
    //! point in serial manner. The input parameters are i and j indices of a
    //! u-data point. The order of execution is i-first, j-last.
    //!
    void ForEachUIndex(const std::function<void(size_t, size_t)>& func) const;

    //!
    //! \brief Invokes the given function \p func for each u-data point in
    //! parallel.
    //!
    //! This function invokes the given function object \p func for each u-data
    //! point in parallel manner. The input parameters are i and j indices of a
    //! u-data point. The order of execution can be arbitrary since it's
    //! multi-threaded.
    //!
    void ParallelForEachUIndex(
        const std::function<void(size_t, size_t)>& func) const;

    //!
    //! \brief Invokes the given function \p func for each v-data point.
    //!
    //! This function invokes the given function object \p func for each v-data
    //! point in serial manner. The input parameters are i and j indices of a
    //! v-data point. The order of execution is i-first, j-last.
    //!
    void ForEachVIndex(const std::function<void(size_t, size_t)>& func) const;

    //!
    //! \brief Invokes the given function \p func for each v-data point in
    //! parallel.
    //!
    //! This function invokes the given function object \p func for each v-data
    //! point in parallel manner. The input parameters are i and j indices of a
    //! v-data point. The order of execution can be arbitrary since it's
    //! multi-threaded.
    //!
    void ParallelForEachVIndex(
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

    //! Returns builder fox FaceCenteredGrid2.
    [[nodiscard]] static Builder GetBuilder();

 protected:
    // VectorGrid2 implementations
    void OnResize(const Vector2UZ& resolution, const Vector2D& gridSpacing,
                  const Vector2D& origin,
                  const Vector2D& initialValue) override;

    //! Fetches the data into a continuous linear array.
    void GetData(std::vector<double>* data) const override;

    //! Sets the data from a continuous linear array.
    void SetData(const std::vector<double>& data) override;

 private:
    void ResetSampler();

    Array2<double> m_dataU;
    Array2<double> m_dataV;
    Vector2D m_dataOriginU;
    Vector2D m_dataOriginV;
    LinearArraySampler2<double> m_uLinearSampler;
    LinearArraySampler2<double> m_vLinearSampler;
    std::function<Vector2D(const Vector2D&)> m_sampler;
};

//! Shared pointer type for the FaceCenteredGrid2.
using FaceCenteredGrid2Ptr = std::shared_ptr<FaceCenteredGrid2>;

//!
//! \brief Front-end to create FaceCenteredGrid2 objects step by step.
//!
class FaceCenteredGrid2::Builder final : public VectorGridBuilder2
{
 public:
    //! Returns builder with resolution.
    [[nodiscard]] Builder& WithResolution(const Vector2UZ& resolution);

    //! Returns builder with resolution.
    [[nodiscard]] Builder& WithResolution(size_t resolutionX,
                                          size_t resolutionY);

    //! Returns builder with grid spacing.
    [[nodiscard]] Builder& WithGridSpacing(const Vector2D& gridSpacing);

    //! Returns builder with grid spacing.
    [[nodiscard]] Builder& WithGridSpacing(double gridSpacingX,
                                           double gridSpacingY);

    //! Returns builder with grid origin.
    [[nodiscard]] Builder& WithOrigin(const Vector2D& gridOrigin);

    //! Returns builder with grid origin.
    [[nodiscard]] Builder& WithOrigin(double gridOriginX, double gridOriginY);

    //! Returns builder with initial value.
    [[nodiscard]] Builder& WithInitialValue(const Vector2D& initialVal);

    //! Returns builder with initial value.
    [[nodiscard]] Builder& WithInitialValue(double initialValX,
                                            double initialValY);

    //! Builds FaceCenteredGrid2 instance.
    [[nodiscard]] FaceCenteredGrid2 Build() const;

    //! Builds shared pointer of FaceCenteredGrid2 instance.
    [[nodiscard]] FaceCenteredGrid2Ptr MakeShared() const;

    //!
    //! \brief Builds shared pointer of FaceCenteredGrid2 instance.
    //!
    //! This is an overriding function that implements VectorGridBuilder2.
    //!
    [[nodiscard]] VectorGrid2Ptr Build(
        const Vector2UZ& resolution, const Vector2D& gridSpacing,
        const Vector2D& gridOrigin, const Vector2D& initialVal) const override;

 private:
    Vector2UZ m_resolution{ 1, 1 };
    Vector2D m_gridSpacing{ 1, 1 };
    Vector2D m_gridOrigin{ 0, 0 };
    Vector2D m_initialVal{ 0, 0 };
};
}  // namespace CubbyFlow

#endif