// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_IMPLICIT_SURFACE_SET3_HPP
#define CUBBYFLOW_IMPLICIT_SURFACE_SET3_HPP

#include <Core/Geometry/BVH3.hpp>
#include <Core/Surface/ImplicitSurface3.hpp>

#include <vector>

namespace CubbyFlow
{
//!
//! \brief 3-D implicit surface set.
//!
//! This class represents 3-D implicit surface set which extends
//! ImplicitSurface3 by overriding implicit surface-related queries. This is
//! class can hold a collection of other implicit surface instances.
//!
class ImplicitSurfaceSet3 final : public ImplicitSurface3
{
 public:
    class Builder;

    //! Constructs an empty implicit surface set.
    ImplicitSurfaceSet3();

    //! Constructs an implicit surface set using list of other surfaces.
    ImplicitSurfaceSet3(const std::vector<ImplicitSurface3Ptr>& surfaces,
                        const Transform3& transform = Transform3(),
                        bool isNormalFlipped = false);

    //! Constructs an implicit surface set using list of other surfaces.
    ImplicitSurfaceSet3(const std::vector<Surface3Ptr>& surfaces,
                        const Transform3& transform = Transform3(),
                        bool isNormalFlipped = false);

    //! Copy constructor.
    ImplicitSurfaceSet3(const ImplicitSurfaceSet3& other);

    //! Copy assignment operator.
    ImplicitSurfaceSet3& operator=(const ImplicitSurfaceSet3& other);

    //! Updates internal spatial query engine.
    void UpdateQueryEngine() override;

    //! Returns true if bounding box can be defined.
    bool IsBounded() const override;

    //! Returns true if the surface is a valid geometry.
    bool IsValidGeometry() const override;

    //! Returns the number of implicit surfaces.
    size_t NumberOfSurfaces() const;

    //! Returns the i-th implicit surface.
    const ImplicitSurface3Ptr& SurfaceAt(size_t i) const;

    //! Adds an explicit surface instance.
    void AddExplicitSurface(const Surface3Ptr& surface);

    //! Adds an implicit surface instance.
    void AddSurface(const ImplicitSurface3Ptr& surface);

    //! Returns builder for ImplicitSurfaceSet3.
    static Builder GetBuilder();

 private:
    std::vector<ImplicitSurface3Ptr> m_surfaces;
    std::vector<ImplicitSurface3Ptr> m_unboundedSurfaces;
    mutable BVH3<ImplicitSurface3Ptr> m_bvh;
    mutable bool m_bvhInvalidated = true;

    // Surface3 implementations.
    Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

    BoundingBox3D BoundingBoxLocal() const override;

    double ClosestDistanceLocal(const Vector3D& otherPoint) const override;

    bool IntersectsLocal(const Ray3D& ray) const override;

    Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

    SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const override;

    bool IsInsideLocal(const Vector3D& otherPoint) const override;

    // ImplicitSurface3 implementations.
    double SignedDistanceLocal(const Vector3D& otherPoint) const override;

    void InvalidateBVH() const;

    void BuildBVH() const;
};

//! Shared pointer type for the ImplicitSurfaceSet3.
using ImplicitSurfaceSet3Ptr = std::shared_ptr<ImplicitSurfaceSet3>;

//!
//! \brief Front-end to create ImplicitSurfaceSet3 objects step by step.
//!
class ImplicitSurfaceSet3::Builder final
    : public SurfaceBuilderBase3<ImplicitSurfaceSet3::Builder>
{
 public:
    //! Returns builder with surfaces.
    Builder& WithSurfaces(const std::vector<ImplicitSurface3Ptr>& surfaces);

    //! Returns builder with explicit surfaces.
    Builder& WithExplicitSurfaces(const std::vector<Surface3Ptr>& surfaces);

    //! Builds ImplicitSurfaceSet3.
    ImplicitSurfaceSet3 Build() const;

    //! Builds shared pointer of ImplicitSurfaceSet3 instance.
    ImplicitSurfaceSet3Ptr MakeShared() const;

 private:
    bool m_isNormalFlipped = false;
    std::vector<ImplicitSurface3Ptr> m_surfaces;
};
}  // namespace CubbyFlow

#endif