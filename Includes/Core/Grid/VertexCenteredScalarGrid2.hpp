// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VERTEX_CENTERED_SCALAR_GRID2_HPP
#define CUBBYFLOW_VERTEX_CENTERED_SCALAR_GRID2_HPP

#include <Core/Grid/ScalarGrid2.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D Vertex-centered scalar grid structure.
//!
//! This class represents 2-D vertex-centered scalar grid which extends
//! ScalarGrid2. As its name suggests, the class defines the data point at the
//! grid vertices (corners). Thus, A x B grid resolution will have (A+1) x (B+1)
//! data points.
//!
class VertexCenteredScalarGrid2 final : public ScalarGrid2
{
 public:
    CUBBYFLOW_GRID_TYPE_NAME(VertexCenteredScalarGrid, 2)

    class Builder;

    //! Constructs zero-sized grid.
    VertexCenteredScalarGrid2() = default;

    //! Constructs a grid with given resolution, grid spacing, origin and
    //! initial value.
    VertexCenteredScalarGrid2(size_t resolutionX, size_t resolutionY,
                              double gridSpacingX = 1.0,
                              double gridSpacingY = 1.0, double originX = 0.0,
                              double originY = 0.0, double initialValue = 0.0);

    //! Constructs a grid with given resolution, grid spacing, origin and
    //! initial value.
    VertexCenteredScalarGrid2(const Vector2UZ& resolution,
                              const Vector2D& gridSpacing = Vector2D{ 1.0,
                                                                      1.0 },
                              const Vector2D& origin = Vector2D{},
                              double initialValue = 0.0);

    //! Copy constructor.
    VertexCenteredScalarGrid2(const VertexCenteredScalarGrid2& other);

    //! Deleted move constructor.
    VertexCenteredScalarGrid2(VertexCenteredScalarGrid2&&) noexcept = delete;

    // Default virtual destructor.
    ~VertexCenteredScalarGrid2() override = default;

    //! Sets the contents with the given \p other grid.
    VertexCenteredScalarGrid2& operator=(
        const VertexCenteredScalarGrid2& other);

    //! Deleted move assignment operator.
    VertexCenteredScalarGrid2& operator=(VertexCenteredScalarGrid2&&) noexcept =
        delete;

    //! Returns the actual data point size.
    [[nodiscard]] Vector2UZ GetDataSize() const override;

    //! Returns data position for the grid point at (0, 0).
    //! Note that this is different from origin() since origin() returns
    //! the lower corner point of the bounding box.
    [[nodiscard]] Vector2D GetDataOrigin() const override;

    //! Returns the copy of the grid instance.
    [[nodiscard]] std::shared_ptr<ScalarGrid2> Clone() const override;

    //!
    //! \brief Swaps the contents with the given \p other grid.
    //!
    //! This function swaps the contents of the grid instance with the given
    //! grid object \p other only if \p other has the same type with this grid.
    //!
    void Swap(Grid2* other) override;

    //! Sets the contents with the given \p other grid.
    void Set(const VertexCenteredScalarGrid2& other);

    //! Returns builder fox VertexCenteredScalarGrid2.
    [[nodiscard]] static Builder GetBuilder();
};

//! Shared pointer for the VertexCenteredScalarGrid2 type.
using VertexCenteredScalarGrid2Ptr = std::shared_ptr<VertexCenteredScalarGrid2>;

//! A grid builder class that returns 2-D vertex-centered scalar grid.
class VertexCenteredScalarGrid2::Builder final : public ScalarGridBuilder2
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
    [[nodiscard]] Builder& WithInitialValue(double initialVal);

    //! Builds VertexCenteredScalarGrid2 instance.
    [[nodiscard]] VertexCenteredScalarGrid2 Build() const;

    //! Builds shared pointer of VertexCenteredScalarGrid2 instance.
    [[nodiscard]] VertexCenteredScalarGrid2Ptr MakeShared() const;

    //!
    //! \brief Builds shared pointer of VertexCenteredScalarGrid2 instance.
    //!
    //! This is an overriding function that implements ScalarGridBuilder2.
    //!
    [[nodiscard]] ScalarGrid2Ptr Build(const Vector2UZ& resolution,
                                       const Vector2D& gridSpacing,
                                       const Vector2D& gridOrigin,
                                       double initialVal) const override;

 private:
    Vector2UZ m_resolution{ 1, 1 };
    Vector2D m_gridSpacing{ 1, 1 };
    Vector2D m_gridOrigin{ 0, 0 };
    double m_initialVal = 0.0;
};
}  // namespace CubbyFlow

#endif