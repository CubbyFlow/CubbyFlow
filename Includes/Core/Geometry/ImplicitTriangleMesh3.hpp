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

#include <Core/Grid/VertexCenteredScalarGrid3.h>
#include <Core/Surface/CustomImplicitSurface3.h>
#include <Core/Surface/ImplicitSurface3.h>
#include <Core/Geometry/TriangleMesh3.hpp>

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

    ImplicitTriangleMesh3(const TriangleMesh3Ptr& mesh, size_t resolutionX,
                          double margin,
                          const Transform3& transform = Transform3(),
                          bool isNormalFlipped = false);

    virtual ~ImplicitTriangleMesh3();

    //! Returns builder fox ImplicitTriangleMesh3.
    static Builder GetBuilder();

    //! Returns grid data.
    const VertexCenteredScalarGrid3Ptr& GetGrid() const;

 private:
    TriangleMesh3Ptr m_mesh;
    VertexCenteredScalarGrid3Ptr m_grid;
    CustomImplicitSurface3Ptr m_customImplicitSurface;

    Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

    double ClosestDistanceLocal(const Vector3D& otherPoint) const override;

    bool IntersectsLocal(const Ray3D& ray) const override;

    BoundingBox3D BoundingBoxLocal() const override;

    Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

    double SignedDistanceLocal(const Vector3D& otherPoint) const override;

    SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const override;
};

//! Shared pointer for the ImplicitTriangleMesh3 type.
using ImplicitTriangleMesh3Ptr = std::shared_ptr<ImplicitTriangleMesh3>;

//!
//! \brief Front-end to create ImplicitTriangleMesh3 objects step by step.
//!
class ImplicitTriangleMesh3::Builder final
    : public SurfaceBuilderBase3<ImplicitTriangleMesh3::Builder>
{
 public:
    //! Returns builder with triangle mesh.
    Builder& WithTriangleMesh(const TriangleMesh3Ptr& mesh);

    //! Returns builder with resolution in x axis.
    Builder& WithResolutionX(size_t resolutionX);

    //! Returns builder with margin around the mesh.
    Builder& WithMargin(double margin);

    //! Builds ImplicitTriangleMesh3.
    ImplicitTriangleMesh3 Build() const;

    //! Builds shared pointer of ImplicitTriangleMesh3 instance.
    ImplicitTriangleMesh3Ptr MakeShared() const;

 private:
    TriangleMesh3Ptr m_mesh;
    size_t m_resolutionX = 32;
    double m_margin = 0.2;
};
}  // namespace CubbyFlow

#endif