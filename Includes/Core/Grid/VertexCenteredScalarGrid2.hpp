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
    CUBBYFLOW_GRID2_TYPE_NAME(VertexCenteredScalarGrid2)

    class Builder;

    //! Constructs zero-sized grid.
    VertexCenteredScalarGrid2();

    //! Constructs a grid with given resolution, grid spacing, origin and
    //! initial value.
    VertexCenteredScalarGrid2(size_t resolutionX, size_t resolutionY,
                              double gridSpacingX = 1.0,
                              double gridSpacingY = 1.0, double originX = 0.0,
                              double originY = 0.0, double initialValue = 0.0);

    //! Constructs a grid with given resolution, grid spacing, origin and
    //! initial value.
    VertexCenteredScalarGrid2(const Size2& resolution,
                              const Vector2D& gridSpacing = Vector2D(1.0, 1.0),
                              const Vector2D& origin = Vector2D(),
                              double initialValue = 0.0);

    //! Copy constructor.
    VertexCenteredScalarGrid2(const VertexCenteredScalarGrid2& other);

    //! Returns the actual data point size.
    Size2 GetDataSize() const override;

    //! Returns data position for the grid point at (0, 0).
    //! Note that this is different from origin() since origin() returns
    //! the lower corner point of the bounding box.
    Vector2D GetDataOrigin() const override;

    //! Returns the copy of the grid instance.
    std::shared_ptr<ScalarGrid2> Clone() const override;

    //!
    //! \brief Swaps the contents with the given \p other grid.
    //!
    //! This function swaps the contents of the grid instance with the given
    //! grid object \p other only if \p other has the same type with this grid.
    //!
    void Swap(Grid2* other) override;

    //! Sets the contents with the given \p other grid.
    void Set(const VertexCenteredScalarGrid2& other);

    //! Sets the contents with the given \p other grid.
    VertexCenteredScalarGrid2& operator=(
        const VertexCenteredScalarGrid2& other);

    //! Returns builder fox VertexCenteredScalarGrid2.
    static Builder GetBuilder();
};

//! Shared pointer for the VertexCenteredScalarGrid2 type.
using VertexCenteredScalarGrid2Ptr = std::shared_ptr<VertexCenteredScalarGrid2>;

//! A grid builder class that returns 2-D vertex-centered scalar grid.
class VertexCenteredScalarGrid2::Builder final : public ScalarGridBuilder2
{
 public:
    //! Returns builder with resolution.
    Builder& WithResolution(const Size2& resolution);

    //! Returns builder with resolution.
    Builder& WithResolution(size_t resolutionX, size_t resolutionY);

    //! Returns builder with grid spacing.
    Builder& WithGridSpacing(const Vector2D& gridSpacing);

    //! Returns builder with grid spacing.
    Builder& WithGridSpacing(double gridSpacingX, double gridSpacingY);

    //! Returns builder with grid origin.
    Builder& WithOrigin(const Vector2D& gridOrigin);

    //! Returns builder with grid origin.
    Builder& WithOrigin(double gridOriginX, double gridOriginY);

    //! Returns builder with initial value.
    Builder& WithInitialValue(double initialVal);

    //! Builds VertexCenteredScalarGrid2 instance.
    VertexCenteredScalarGrid2 Build() const;

    //! Builds shared pointer of VertexCenteredScalarGrid2 instance.
    VertexCenteredScalarGrid2Ptr MakeShared() const;

    //!
    //! \brief Builds shared pointer of VertexCenteredScalarGrid2 instance.
    //!
    //! This is an overriding function that implements ScalarGridBuilder2.
    //!
    ScalarGrid2Ptr Build(const Size2& resolution, const Vector2D& gridSpacing,
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