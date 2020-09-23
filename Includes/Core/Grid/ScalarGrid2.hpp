// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SCALAR_GRID2_HPP
#define CUBBYFLOW_SCALAR_GRID2_HPP

#include <Core/Array/Array2.hpp>
#include <Core/Array/ArrayAccessor2.hpp>
#include <Core/Array/ArraySamplers2.hpp>
#include <Core/Field/ScalarField2.hpp>
#include <Core/Grid/Grid2.hpp>

namespace CubbyFlow
{
//! Abstract base class for 2-D scalar grid structure.
class ScalarGrid2 : public ScalarField2, public Grid2
{
 public:
    //! Read-write array accessor type.
    using ScalarDataAccessor = ArrayAccessor2<double>;

    //! Read-only array accessor type.
    using ConstScalarDataAccessor = ConstArrayAccessor2<double>;

    //! Constructs an empty grid.
    ScalarGrid2();

    //! Default copy constructor.
    ScalarGrid2(const ScalarGrid2&) = default;

    //! Default move constructor.
    ScalarGrid2(ScalarGrid2&&) noexcept = default;

    //! Default virtual destructor.
    ~ScalarGrid2() override = default;

    //! Default copy assignment operator.
    ScalarGrid2& operator=(const ScalarGrid2&) = default;

    //! Default move assignment operator.
    ScalarGrid2& operator=(ScalarGrid2&&) noexcept = default;

    //!
    //! \brief Returns the size of the grid data.
    //!
    //! This function returns the size of the grid data which is not necessarily
    //! equal to the grid resolution if the data is not stored at cell-center.
    //!
    [[nodiscard]] virtual Size2 GetDataSize() const = 0;

    //!
    //! \brief Returns the origin of the grid data.
    //!
    //! This function returns data position for the grid point at (0, 0).
    //! Note that this is different from origin() since origin() returns
    //! the lower corner point of the bounding box.
    //!
    [[nodiscard]] virtual Vector2D GetDataOrigin() const = 0;

    //! Returns the copy of the grid instance.
    [[nodiscard]] virtual std::shared_ptr<ScalarGrid2> Clone() const = 0;

    //! Clears the contents of the grid.
    void Clear();

    //! Resizes the grid using given parameters.
    void Resize(size_t resolutionX, size_t resolutionY,
                double gridSpacingX = 1.0, double gridSpacingY = 1.0,
                double originX = 0.0, double originY = 0.0,
                double initialValue = 0.0);

    //! Resizes the grid using given parameters.
    void Resize(const Size2& resolution,
                const Vector2D& gridSpacing = Vector2D(1, 1),
                const Vector2D& origin = Vector2D(), double initialValue = 0.0);

    //! Resizes the grid using given parameters.
    void Resize(double gridSpacingX, double gridSpacingY, double originX,
                double originY);

    //! Resizes the grid using given parameters.
    void Resize(const Vector2D& gridSpacing, const Vector2D& origin);

    //! Returns the grid data at given data point.
    const double& operator()(size_t i, size_t j) const;

    //! Returns the grid data at given data point.
    double& operator()(size_t i, size_t j);

    //! Returns the gradient vector at given data point.
    [[nodiscard]] Vector2D GradientAtDataPoint(size_t i, size_t j) const;

    //! Returns the Laplacian at given data point.
    [[nodiscard]] double LaplacianAtDataPoint(size_t i, size_t j) const;

    //! Returns the read-write data array accessor.
    [[nodiscard]] ScalarDataAccessor GetDataAccessor();

    //! Returns the read-only data array accessor.
    [[nodiscard]] ConstScalarDataAccessor GetConstDataAccessor() const;

    //! Returns the function that maps data point to its position.
    [[nodiscard]] DataPositionFunc GetDataPosition() const;

    //! Fills the grid with given value.
    void Fill(double value, ExecutionPolicy policy = ExecutionPolicy::Parallel);

    //! Fills the grid with given position-to-value mapping function.
    void Fill(const std::function<double(const Vector2D&)>& func,
              ExecutionPolicy policy = ExecutionPolicy::Parallel);

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

    // ScalarField2 implementations

    //!
    //! \brief Returns the sampled value at given position \p x.
    //!
    //! This function returns the data sampled at arbitrary position \p x.
    //! The sampling function is linear.
    //!
    [[nodiscard]] double Sample(const Vector2D& x) const override;

    //!
    //! \brief Returns the sampler function.
    //!
    //! This function returns the data sampler function object. The sampling
    //! function is linear.
    //!
    [[nodiscard]] std::function<double(const Vector2D&)> Sampler()
        const override;

    //! Returns the gradient vector at given position \p x.
    [[nodiscard]] Vector2D Gradient(const Vector2D& x) const override;

    //! Returns the Laplacian at given position \p x.
    [[nodiscard]] double Laplacian(const Vector2D& x) const override;

    //! Serializes the grid instance to the output buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes the input buffer to the grid instance.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

 protected:
    //! Swaps the data storage and predefined samplers with given grid.
    void SwapScalarGrid(ScalarGrid2* other);

    //! Sets the data storage and predefined samplers with given grid.
    void SetScalarGrid(const ScalarGrid2& other);

    //! Fetches the data into a continuous linear array.
    void GetData(std::vector<double>* data) const override;

    //! Sets the data from a continuous linear array.
    void SetData(const std::vector<double>& data) override;

 private:
    void ResetSampler();

    Array2<double> m_data;
    LinearArraySampler2<double, double> m_linearSampler;
    std::function<double(const Vector2D&)> m_sampler;
};

//! Shared pointer for the ScalarGrid2 type.
using ScalarGrid2Ptr = std::shared_ptr<ScalarGrid2>;

//! Abstract base class for 2-D scalar grid builder.
class ScalarGridBuilder2
{
 public:
    //! Default constructor.
    ScalarGridBuilder2() = default;

    //! Default copy constructor.
    ScalarGridBuilder2(const ScalarGridBuilder2&) = default;

    //! Default move constructor.
    ScalarGridBuilder2(ScalarGridBuilder2&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~ScalarGridBuilder2() = default;

    //! Default copy assignment operator.
    ScalarGridBuilder2& operator=(const ScalarGridBuilder2&) = default;

    //! Default move assignment operator.
    ScalarGridBuilder2& operator=(ScalarGridBuilder2&&) noexcept = default;

    //! Returns 2-D scalar grid with given parameters.
    [[nodiscard]] virtual ScalarGrid2Ptr Build(const Size2& resolution,
                                               const Vector2D& gridSpacing,
                                               const Vector2D& gridOrigin,
                                               double initialVal) const = 0;
};

//! Shared pointer for the ScalarGridBuilder2 type.
using ScalarGridBuilder2Ptr = std::shared_ptr<ScalarGridBuilder2>;
}  // namespace CubbyFlow

#endif