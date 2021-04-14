// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_IMPLICIT_TRIANGLE_MESH3_HPP
#define CUBBYFLOW_IMPLICIT_TRIANGLE_MESH3_HPP

#include <Core/Geometry/CustomImplicitSurface.hpp>
#include <Core/Geometry/ImplicitSurface.hpp>
#include <Core/Geometry/TriangleMesh3.hpp>
#include <Core/Grid/VertexCenteredScalarGrid.hpp>

namespace CubbyFlow
{
//!
//! \brief  TriangleMesh3 to ImplicitSurface3 converter.
//!
//! This class builds signed-distance field for given TriangleMesh3 instance so
//! that it can be used as an ImplicitSurface3 instance. The mesh is discretized
//! into a regular grid and the signed-distance is measured at each grid point.
//! Thus, there is a sampling error and its magnitude depends on the grid
//! resolution.
//!
class ImplicitTriangleMesh3 final : public ImplicitSurface3
{
 public:
    class Builder;

    //! Constructs an ImplicitSurface3 with mesh and other grid parameters.
    explicit ImplicitTriangleMesh3(TriangleMesh3Ptr mesh,
                                   size_t resolutionX = 32, double margin = 0.2,
                                   const Transform3& _transform = Transform3{},
                                   bool _isNormalFlipped = false);

    //! Default copy constructor.
    ImplicitTriangleMesh3(const ImplicitTriangleMesh3&) = default;

    //! Default move constructor.
    ImplicitTriangleMesh3(ImplicitTriangleMesh3&&) noexcept = default;

    //! Default virtual destructor.
    ~ImplicitTriangleMesh3() override = default;

    //! Default copy assignment operator.
    ImplicitTriangleMesh3& operator=(const ImplicitTriangleMesh3&) = default;

    //! Default move assignment operator.
    ImplicitTriangleMesh3& operator=(ImplicitTriangleMesh3&&) noexcept =
        default;

    //! Returns builder fox ImplicitTriangleMesh3.
    [[nodiscard]] static Builder GetBuilder();

    //! Returns grid data.
    [[nodiscard]] const VertexCenteredScalarGrid3Ptr& GetGrid() const;

 private:
    [[nodiscard]] Vector3D ClosestPointLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] double ClosestDistanceLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] bool IntersectsLocal(const Ray3D& ray) const override;

    [[nodiscard]] BoundingBox3D BoundingBoxLocal() const override;

    [[nodiscard]] Vector3D ClosestNormalLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] double SignedDistanceLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const override;

    TriangleMesh3Ptr m_mesh;
    VertexCenteredScalarGrid3Ptr m_grid;
    CustomImplicitSurface3Ptr m_customImplicitSurface;
};

//! Shared pointer for the ImplicitTriangleMesh3 type.
using ImplicitTriangleMesh3Ptr = std::shared_ptr<ImplicitTriangleMesh3>;

//!
//! \brief Front-end to create ImplicitTriangleMesh3 objects step by step.
//!
class ImplicitTriangleMesh3::Builder final : public SurfaceBuilderBase3<Builder>
{
 public:
    //! Returns builder with triangle mesh.
    [[nodiscard]] Builder& WithTriangleMesh(const TriangleMesh3Ptr& mesh);

    //! Returns builder with resolution in x axis.
    [[nodiscard]] Builder& WithResolutionX(size_t resolutionX);

    //! Returns builder with margin around the mesh.
    [[nodiscard]] Builder& WithMargin(double margin);

    //! Builds ImplicitTriangleMesh3.
    [[nodiscard]] ImplicitTriangleMesh3 Build() const;

    //! Builds shared pointer of ImplicitTriangleMesh3 instance.
    [[nodiscard]] ImplicitTriangleMesh3Ptr MakeShared() const;

 private:
    TriangleMesh3Ptr m_mesh;
    size_t m_resolutionX = 32;
    double m_margin = 0.2;
};
}  // namespace CubbyFlow

#endif