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

#include <Core/Array/Array.hpp>
#include <Core/Geometry/BVH.hpp>
#include <Core/Geometry/ImplicitSurface.hpp>

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
    ImplicitSurfaceSet3() = default;

    //! Constructs an implicit surface set using list of other surfaces.
    explicit ImplicitSurfaceSet3(Array1<ImplicitSurface3Ptr> surfaces,
                                 const Transform3& _transform = Transform3{},
                                 bool _isNormalFlipped = false);

    //! Constructs an implicit surface set using list of other surfaces.
    ImplicitSurfaceSet3(const Array1<Surface3Ptr>& surfaces,
                        const Transform3& _transform = Transform3{},
                        bool _isNormalFlipped = false);

    //! Copy constructor.
    ImplicitSurfaceSet3(const ImplicitSurfaceSet3& other);

    //! Move constructor.
    ImplicitSurfaceSet3(ImplicitSurfaceSet3&& other) noexcept;

    //! Default virtual destructor.
    ~ImplicitSurfaceSet3() override = default;

    //! Copy assignment operator.
    ImplicitSurfaceSet3& operator=(const ImplicitSurfaceSet3& other);

    //! Default move assignment operator.
    ImplicitSurfaceSet3& operator=(ImplicitSurfaceSet3&&) = default;

    //! Updates internal spatial query engine.
    void UpdateQueryEngine() override;

    //! Returns true if bounding box can be defined.
    [[nodiscard]] bool IsBounded() const override;

    //! Returns true if the surface is a valid geometry.
    [[nodiscard]] bool IsValidGeometry() const override;

    //! Returns the number of implicit surfaces.
    [[nodiscard]] size_t NumberOfSurfaces() const;

    //! Returns the i-th implicit surface.
    [[nodiscard]] const ImplicitSurface3Ptr& SurfaceAt(size_t i) const;

    //! Adds an explicit surface instance.
    void AddExplicitSurface(const Surface3Ptr& surface);

    //! Adds an implicit surface instance.
    void AddSurface(const ImplicitSurface3Ptr& surface);

    //! Returns builder for ImplicitSurfaceSet3.
    [[nodiscard]] static Builder GetBuilder();

 private:
    // Surface3 implementations.
    [[nodiscard]] Vector3D ClosestPointLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] BoundingBox3D BoundingBoxLocal() const override;

    [[nodiscard]] double ClosestDistanceLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] bool IntersectsLocal(const Ray3D& ray) const override;

    [[nodiscard]] Vector3D ClosestNormalLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const override;

    [[nodiscard]] bool IsInsideLocal(const Vector3D& otherPoint) const override;

    // ImplicitSurface3 implementations.
    [[nodiscard]] double SignedDistanceLocal(
        const Vector3D& otherPoint) const override;

    void InvalidateBVH() const;

    void BuildBVH() const;

    Array1<ImplicitSurface3Ptr> m_surfaces;
    Array1<ImplicitSurface3Ptr> m_unboundedSurfaces;
    mutable BVH3<ImplicitSurface3Ptr> m_bvh;
    mutable bool m_bvhInvalidated = true;
};

//! Shared pointer type for the ImplicitSurfaceSet3.
using ImplicitSurfaceSet3Ptr = std::shared_ptr<ImplicitSurfaceSet3>;

//!
//! \brief Front-end to create ImplicitSurfaceSet3 objects step by step.
//!
class ImplicitSurfaceSet3::Builder final : public SurfaceBuilderBase3<Builder>
{
 public:
    //! Returns builder with surfaces.
    [[nodiscard]] Builder& WithSurfaces(
        const Array1<ImplicitSurface3Ptr>& surfaces);

    //! Returns builder with explicit surfaces.
    [[nodiscard]] Builder& WithExplicitSurfaces(
        const Array1<Surface3Ptr>& surfaces);

    //! Builds ImplicitSurfaceSet3.
    [[nodiscard]] ImplicitSurfaceSet3 Build() const;

    //! Builds shared pointer of ImplicitSurfaceSet3 instance.
    [[nodiscard]] ImplicitSurfaceSet3Ptr MakeShared() const;

 private:
    Array1<ImplicitSurface3Ptr> m_surfaces;
};
}  // namespace CubbyFlow

#endif