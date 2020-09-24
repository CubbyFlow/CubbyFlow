// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CELL_CENTERED_SCALAR_GRID2_HPP
#define CUBBYFLOW_CELL_CENTERED_SCALAR_GRID2_HPP

#include <Core/Grid/ScalarGrid2.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D Cell-centered scalar grid structure.
//!
//! This class represents 2-D cell-centered scalar grid which extends
//! ScalarGrid2. As its name suggests, the class defines the data point at the
//! center of a grid cell. Thus, the dimension of data points are equal to the
//! dimension of the cells.
//!
class CellCenteredScalarGrid2 final : public ScalarGrid2
{
 public:
    CUBBYFLOW_GRID2_TYPE_NAME(CellCenteredScalarGrid2)

    class Builder;

    //! Constructs zero-sized grid.
    CellCenteredScalarGrid2() = default;

    //! Constructs a grid with given resolution, grid spacing, origin and
    //! initial value.
    CellCenteredScalarGrid2(size_t resolutionX, size_t resolutionY,
                            double gridSpacingX = 1.0,
                            double gridSpacingY = 1.0, double originX = 0.0,
                            double originY = 0.0, double initialValue = 0.0);

    //! Constructs a grid with given resolution, grid spacing, origin and
    //! initial value.
    CellCenteredScalarGrid2(const Size2& resolution,
                            const Vector2D& gridSpacing = Vector2D{ 1.0, 1.0 },
                            const Vector2D& origin = Vector2D{},
                            double initialValue = 0.0);

    //! Copy constructor.
    CellCenteredScalarGrid2(const CellCenteredScalarGrid2& other);

    //! Deleted move constructor.
    CellCenteredScalarGrid2(CellCenteredScalarGrid2&&) noexcept = delete;

    //! Default virtual destructor.
    ~CellCenteredScalarGrid2() override = default;

    //! Copy assignment operator.
    CellCenteredScalarGrid2& operator=(const CellCenteredScalarGrid2& other);

    //! Deleted move assignment operator.
    CellCenteredScalarGrid2& operator=(CellCenteredScalarGrid2&&) noexcept =
        delete;

    //! Returns the actual data point size.
    [[nodiscard]] Size2 GetDataSize() const override;

    //! Returns data position for the grid point at (0, 0).
    //! Note that this is different from origin() since origin() returns
    //! the lower corner point of the bounding box.
    [[nodiscard]] Vector2D GetDataOrigin() const override;

    //!
    //! \brief Swaps the contents with the given \p other grid.
    //!
    //! This function swaps the contents of the grid instance with the given
    //! grid object \p other only if \p other has the same type with this grid.
    //!
    void Swap(Grid2* other) override;

    //! Sets the contents with the given \p other grid.
    void Set(const CellCenteredScalarGrid2& other);

    //! Returns the copy of the grid instance.
    [[nodiscard]] std::shared_ptr<ScalarGrid2> Clone() const override;

    //! Returns builder fox CellCenteredScalarGrid2.
    [[nodiscard]] static Builder GetBuilder();
};

//! Shared pointer for the CellCenteredScalarGrid2 type.
using CellCenteredScalarGrid2Ptr = std::shared_ptr<CellCenteredScalarGrid2>;

//!
//! \brief Front-end to create CellCenteredScalarGrid2 objects step by step.
//!
class CellCenteredScalarGrid2::Builder final : public ScalarGridBuilder2
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
    [[nodiscard]] Builder& WithInitialValue(double initialVal);

    //! Builds CellCenteredScalarGrid2 instance.
    [[nodiscard]] CellCenteredScalarGrid2 Build() const;

    //! Builds shared pointer of CellCenteredScalarGrid2 instance.
    [[nodiscard]] CellCenteredScalarGrid2Ptr MakeShared() const;

    //!
    //! \brief Builds shared pointer of CellCenteredScalarGrid2 instance.
    //!
    //! This is an overriding function that implements ScalarGridBuilder2.
    //!
    [[nodiscard]] ScalarGrid2Ptr Build(const Size2& resolution,
                                       const Vector2D& gridSpacing,
                                       const Vector2D& gridOrigin,
                                       double initialVal) const override;

 private:
    Size2 m_resolution{ 1, 1 };
    Vector2D m_gridSpacing{ 1, 1 };
    Vector2D m_gridOrigin{ 0, 0 };
    double m_initialVal = 0.0;
};
}  // namespace CubbyFlow

#endif