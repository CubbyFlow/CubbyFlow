// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CELL_CENTERED_VECTOR_GRID2_HPP
#define CUBBYFLOW_CELL_CENTERED_VECTOR_GRID2_HPP

#include <Core/Grid/CollocatedVectorGrid2.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D Cell-centered vector grid structure.
//!
//! This class represents 2-D cell-centered vector grid which extends
//! VectorGrid2. As its name suggests, the class defines the data point at the
//! center of a grid cell. Thus, the dimension of data points are equal to the
//! dimension of the cells.
//!
class CellCenteredVectorGrid2 final : public CollocatedVectorGrid2
{
 public:
    CUBBYFLOW_GRID2_TYPE_NAME(CellCenteredVectorGrid2);

    class Builder;

    //! Constructs zero-sized grid.
    CellCenteredVectorGrid2() = default;

    //! Constructs a grid with given resolution, grid spacing, origin
    //! and initial value.
    CellCenteredVectorGrid2(size_t resolutionX, size_t resolutionY,
                            double gridSpacingX = 1.0,
                            double gridSpacingY = 1.0, double originX = 0.0,
                            double originY = 0.0, double initialValueU = 0.0,
                            double initialValueV = 0.0);

    //! Constructs a grid with given resolution, grid spacing, origin
    //! and initial value.
    CellCenteredVectorGrid2(const Size2& resolution,
                            const Vector2D& gridSpacing = Vector2D{ 1.0, 1.0 },
                            const Vector2D& origin = Vector2D{},
                            const Vector2D& initialValue = Vector2D{});

    //! Copy constructor.
    CellCenteredVectorGrid2(const CellCenteredVectorGrid2& other);

    //! Deleted move constructor.
    CellCenteredVectorGrid2(CellCenteredVectorGrid2&&) noexcept = delete;

    //! Default virtual destructor.
    ~CellCenteredVectorGrid2() override = default;

    //! Copy assignment operator.
    CellCenteredVectorGrid2& operator=(const CellCenteredVectorGrid2& other);

    //! Deleted move assignment operator.
    CellCenteredVectorGrid2& operator=(CellCenteredVectorGrid2&&) noexcept =
        delete;

    //! Returns the actual data point size.
    [[nodiscard]] Size2 GetDataSize() const override;

    //! Returns data position for the grid point at (0, 0).
    //! Note that this is different from origin() since origin() returns
    //! the lower corner point of the bounding box.
    [[nodiscard]] Vector2D GetDataOrigin() const override;

    //! Returns the copy of the grid instance.
    [[nodiscard]] std::shared_ptr<VectorGrid2> Clone() const override;

    //!
    //! \brief Swaps the contents with the given \p other grid.
    //!
    //! This function swaps the contents of the grid instance with the given
    //! grid object \p other only if \p other has the same type with this grid.
    //!
    void Swap(Grid2* other) override;

    //! Fills the grid with given value.
    void Fill(const Vector2D& value,
              ExecutionPolicy policy = ExecutionPolicy::Parallel) override;

    //! Fills the grid with given function.
    void Fill(const std::function<Vector2D(const Vector2D&)>& func,
              ExecutionPolicy policy = ExecutionPolicy::Parallel) override;

    //! Sets the contents with the given \p other grid.
    void Set(const CellCenteredVectorGrid2& other);

    //! Returns the builder fox CellCenteredVectorGrid2.
    [[nodiscard]] static Builder GetBuilder();
};

//! Shared pointer for the CellCenteredVectorGrid2 type.
using CellCenteredVectorGrid2Ptr = std::shared_ptr<CellCenteredVectorGrid2>;

//!
//! \brief Front-end to create CellCenteredVectorGrid2 objects step by step.
//!
class CellCenteredVectorGrid2::Builder final : public VectorGridBuilder2
{
 public:
    //! Returns builder with resolution.
    [[nodiscard]] Builder& WithResolution(const Size2& resolution);

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

    //! Builds CellCenteredVectorGrid2 instance.
    [[nodiscard]] CellCenteredVectorGrid2 Build() const;

    //! Builds shared pointer of CellCenteredScalarGrid2 instance.
    [[nodiscard]] CellCenteredVectorGrid2Ptr MakeShared() const;

    //!
    //! \brief Builds shared pointer of CellCenteredVectorGrid2 instance.
    //!
    //! This is an overriding function that implements VectorGridBuilder2.
    //!
    [[nodiscard]] VectorGrid2Ptr Build(
        const Size2& resolution, const Vector2D& gridSpacing,
        const Vector2D& gridOrigin, const Vector2D& initialVal) const override;

 private:
    Size2 m_resolution{ 1, 1 };
    Vector2D m_gridSpacing{ 1, 1 };
    Vector2D m_gridOrigin{ 0, 0 };
    Vector2D m_initialVal{ 0, 0 };
};
}  // namespace CubbyFlow

#endif