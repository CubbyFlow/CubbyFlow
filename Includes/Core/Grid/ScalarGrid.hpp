// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SCALAR_GRID_HPP
#define CUBBYFLOW_SCALAR_GRID_HPP

#include <Core/Array/Array.hpp>
#include <Core/Array/ArraySamplers.hpp>
#include <Core/Array/ArrayView.hpp>
#include <Core/Field/ScalarField.hpp>
#include <Core/Grid/Grid.hpp>

namespace CubbyFlow
{
//! Abstract base class for N-D scalar grid structure.
template <size_t N>
class ScalarGrid : public ScalarField<N>, public Grid<N>
{
 public:
    //! Read-write array view type.
    using ScalarDataView = ArrayView<double, N>;

    //! Read-only array view type.
    using ConstScalarDataView = ArrayView<const double, N>;

    // Import Grid members
    using Grid<N>::GridSpacing;
    using Grid<N>::Origin;
    using Grid<N>::Resolution;

    //! Constructs an empty grid.
    ScalarGrid();

    //! Default virtual destructor.
    ~ScalarGrid() override = default;

    //! Copy constructor.
    ScalarGrid(const ScalarGrid& other);

    //! Move constructor.
    ScalarGrid(ScalarGrid&& other) noexcept;

    //! Copy assignment operator.
    ScalarGrid& operator=(const ScalarGrid& other);

    //! Move assignment operator.
    ScalarGrid& operator=(ScalarGrid&& other) noexcept;

    //!
    //! \brief Returns the size of the grid data.
    //!
    //! This function returns the size of the grid data which is not necessarily
    //! equal to the grid resolution if the data is not stored at cell-center.
    //!
    [[nodiscard]] virtual Vector<size_t, N> DataSize() const = 0;

    //!
    //! \brief Returns the origin of the grid data.
    //!
    //! This function returns data position for the grid point at (0, 0, ...).
    //! Note that this is different from origin() since origin() returns
    //! the lower corner point of the bounding box.
    //!
    [[nodiscard]] virtual Vector<double, N> DataOrigin() const = 0;

    //! Returns the copy of the grid instance.
    [[nodiscard]] virtual std::shared_ptr<ScalarGrid> Clone() const = 0;

    //! Clears the contents of the grid.
    void Clear();

    //! Resizes the grid using given parameters.
    void Resize(const Vector<size_t, N>& resolution,
                const Vector<double, N>& gridSpacing =
                    Vector<double, N>::MakeConstant(1),
                const Vector<double, N>& origin = Vector<double, N>{},
                double initialValue = 0.0);

    //! Resizes the grid using given parameters.
    void Resize(const Vector<double, N>& gridSpacing,
                const Vector<double, N>& origin);

    //! Returns the grid data at given data point.
    const double& operator()(const Vector<size_t, N>& idx) const;

    //! Returns the grid data at given data point.
    double& operator()(const Vector<size_t, N>& idx);

    //! Returns the grid data at given data point.
    template <typename... Indices>
    const double& operator()(size_t i, Indices... indices) const
    {
        return (*this)(Vector<size_t, N>(i, indices...));
    }

    //! Returns the grid data at given data point.
    template <typename... Indices>
    double& operator()(size_t i, Indices... indices)
    {
        return (*this)(Vector<size_t, N>(i, indices...));
    }

    //! Returns the gradient vector at given data point.
    [[nodiscard]] Vector<double, N> GradientAtDataPoint(
        const Vector<size_t, N>& idx) const;

    //! Returns the gradient vector at given data point.
    template <typename... Indices>
    [[nodiscard]] Vector<double, N> GradientAtDataPoint(
        size_t i, Indices... indices) const
    {
        return GradientAtDataPoint(Vector<size_t, N>(i, indices...));
    }

    //! Returns the Laplacian at given data point.
    [[nodiscard]] double LaplacianAtDataPoint(
        const Vector<size_t, N>& idx) const;

    //! Returns the Laplacian at given data point.
    template <typename... Indices>
    [[nodiscard]] double LaplacianAtDataPoint(size_t i,
                                              Indices... indices) const
    {
        return LaplacianAtDataPoint(Vector<size_t, N>(i, indices...));
    }

    //! Returns the read-write data array accessor.
    [[nodiscard]] ScalarDataView DataView();

    //! Returns the read-only data array accessor.
    [[nodiscard]] ConstScalarDataView DataView() const;

    //! Returns the function that maps data point to its position.
    [[nodiscard]] GridDataPositionFunc<N> DataPosition() const;

    //! Fills the grid with given value.
    void Fill(double value, ExecutionPolicy policy = ExecutionPolicy::Parallel);

    //! Fills the grid with given position-to-value mapping function.
    void Fill(const std::function<double(const Vector<double, N>&)>& func,
              ExecutionPolicy policy = ExecutionPolicy::Parallel);

    //!
    //! \brief Invokes the given function \p func for each data point.
    //!
    //! This function invokes the given function object \p func for each data
    //! point in serial manner. The input parameters are i, j, ... indices of a
    //! data point. The order of execution is i-first, j-next.
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
    //! \brief Invokes the given function \p func for each data point
    //! in parallel.
    //!
    //! This function invokes the given function object \p func for each data
    //! point in parallel manner. The input parameters are i, j, ... indices of
    //! a data point. The order of execution can be arbitrary since it's
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

    //!
    //! \brief Returns the sampled value at given position \p x.
    //!
    //! This function returns the data sampled at arbitrary position \p x.
    //! The sampling function is linear.
    //!
    [[nodiscard]] double Sample(const Vector<double, N>& x) const override;

    //!
    //! \brief Returns the sampler function.
    //!
    //! This function returns the data sampler function object. The sampling
    //! function is linear.
    //!
    [[nodiscard]] std::function<double(const Vector<double, N>&)> Sampler()
        const override;

    //! Returns the gradient vector at given position \p x.
    [[nodiscard]] Vector<double, N> Gradient(
        const Vector<double, N>& x) const override;

    //! Returns the Laplacian at given position \p x.
    [[nodiscard]] double Laplacian(const Vector<double, N>& x) const override;

    //! Serializes the grid instance to the output buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes the input buffer to the grid instance.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

 protected:
    using Grid<N>::SetSizeParameters;
    using Grid<N>::SwapGrid;
    using Grid<N>::SetGrid;

    //! Swaps the data storage and predefined samplers with given grid.
    void SwapScalarGrid(ScalarGrid* other);

    //! Sets the data storage and predefined samplers with given grid.
    void SetScalarGrid(const ScalarGrid& other);

    //! Fetches the data into a continuous linear array.
    void GetData(Array1<double>& data) const override;

    //! Sets the data from a continuous linear array.
    void SetData(const ConstArrayView1<double>& data) override;

 private:
    void ResetSampler();

    Array<double, N> m_data;
    LinearArraySampler<double, N> m_linearSampler;
    std::function<double(const Vector<double, N>&)> m_sampler;
};

//! 2-D ScalarGrid type.
using ScalarGrid2 = ScalarGrid<2>;

//! 3-D ScalarGrid type.
using ScalarGrid3 = ScalarGrid<3>;

//! Shared pointer for the ScalarGrid2 type.
using ScalarGrid2Ptr = std::shared_ptr<ScalarGrid2>;

//! Shared pointer for the ScalarGrid3 type.
using ScalarGrid3Ptr = std::shared_ptr<ScalarGrid3>;

//! Abstract base class for N-D scalar grid builder.
template <size_t N>
class ScalarGridBuilder
{
 public:
    //! Creates a builder.
    ScalarGridBuilder() = default;

    //! Default virtual destructor.
    virtual ~ScalarGridBuilder() = default;

    //! Deleted copy constructor.
    ScalarGridBuilder(const ScalarGridBuilder& other) = delete;

    //! Deleted move constructor.
    ScalarGridBuilder(ScalarGridBuilder&& other) noexcept = delete;

    //! Deleted copy assignment operator.
    ScalarGridBuilder& operator=(const ScalarGridBuilder& other) = delete;

    //! Deleted move assignment operator.
    ScalarGridBuilder& operator=(ScalarGridBuilder&& other) noexcept = delete;

    //! Returns N-D scalar grid with given parameters.
    virtual std::shared_ptr<ScalarGrid<N>> Build(
        const Vector<size_t, N>& resolution,
        const Vector<double, N>& gridSpacing,
        const Vector<double, N>& gridOrigin, double initialVal) const = 0;
};

//! 2-D ScalarGridBuilder type.
using ScalarGridBuilder2 = ScalarGridBuilder<2>;

//! 3-D ScalarGridBuilder type.
using ScalarGridBuilder3 = ScalarGridBuilder<3>;

//! Shared pointer for the ScalarGridBuilder2 type.
using ScalarGridBuilder2Ptr = std::shared_ptr<ScalarGridBuilder2>;

//! Shared pointer for the ScalarGridBuilder3 type.
using ScalarGridBuilder3Ptr = std::shared_ptr<ScalarGridBuilder3>;
}  // namespace CubbyFlow

#endif