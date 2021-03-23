// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_IMPLICIT_SURFACE_SET2_HPP
#define CUBBYFLOW_IMPLICIT_SURFACE_SET2_HPP

#include <Core/Array/Array.hpp>
#include <Core/Geometry/BVH.hpp>
#include <Core/Geometry/ImplicitSurface.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D implicit surface set.
//!
//! This class represents 2-D implicit surface set which extends
//! ImplicitSurface2 by overriding implicit surface-related queries. This is
//! class can hold a collection of other implicit surface instances.
//!
class ImplicitSurfaceSet2 final : public ImplicitSurface2
{
 public:
    class Builder;

    //! Constructs an empty implicit surface set.
    ImplicitSurfaceSet2() = default;

    //! Constructs an implicit surface set using list of other surfaces.
    explicit ImplicitSurfaceSet2(Array1<ImplicitSurface2Ptr> surfaces,
                                 const Transform2& _transform = Transform2{},
                                 bool _isNormalFlipped = false);

    //! Constructs an implicit surface set using list of other surfaces.
    ImplicitSurfaceSet2(const Array1<Surface2Ptr>& surfaces,
                        const Transform2& _transform = Transform2{},
                        bool _isNormalFlipped = false);

    //! Copy constructor.
    ImplicitSurfaceSet2(const ImplicitSurfaceSet2& other);

    //! Move constructor.
    ImplicitSurfaceSet2(ImplicitSurfaceSet2&& other) noexcept;

    //! Default virtual destructor.
    ~ImplicitSurfaceSet2() override = default;

    //! Copy assignment operator.
    ImplicitSurfaceSet2& operator=(const ImplicitSurfaceSet2& other);

    //! Default move assignment operator.
    ImplicitSurfaceSet2& operator=(ImplicitSurfaceSet2&&) = default;

    //! Updates internal spatial query engine.
    void UpdateQueryEngine() override;

    //! Returns true if bounding box can be defined.
    [[nodiscard]] bool IsBounded() const override;

    //! Returns true if the surface is a valid geometry.
    [[nodiscard]] bool IsValidGeometry() const override;

    //! Returns the number of implicit surfaces.
    [[nodiscard]] size_t NumberOfSurfaces() const;

    //! Returns the i-th implicit surface.
    [[nodiscard]] const ImplicitSurface2Ptr& SurfaceAt(size_t i) const;

    //! Adds an explicit surface instance.
    void AddExplicitSurface(const Surface2Ptr& surface);

    //! Adds an implicit surface instance.
    void AddSurface(const ImplicitSurface2Ptr& surface);

    //! Returns builder fox ImplicitSurfaceSet2.
    [[nodiscard]] static Builder GetBuilder();

 private:
    // Surface2 implementations
    [[nodiscard]] Vector2D ClosestPointLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] BoundingBox2D BoundingBoxLocal() const override;

    [[nodiscard]] double ClosestDistanceLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] bool IntersectsLocal(const Ray2D& ray) const override;

    [[nodiscard]] Vector2D ClosestNormalLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] SurfaceRayIntersection2 ClosestIntersectionLocal(
        const Ray2D& ray) const override;

    [[nodiscard]] bool IsInsideLocal(const Vector2D& otherPoint) const override;

    // ImplicitSurface2 implementations
    [[nodiscard]] double SignedDistanceLocal(
        const Vector2D& otherPoint) const override;

    void InvalidateBVH() const;

    void BuildBVH() const;

    Array1<ImplicitSurface2Ptr> m_surfaces;
    Array1<ImplicitSurface2Ptr> m_unboundedSurfaces;
    mutable BVH2<ImplicitSurface2Ptr> m_bvh;
    mutable bool m_bvhInvalidated = true;
};

//! Shared pointer type for the ImplicitSurfaceSet2.
using ImplicitSurfaceSet2Ptr = std::shared_ptr<ImplicitSurfaceSet2>;

//!
//! \brief Front-end to create ImplicitSurfaceSet2 objects step by step.
//!
class ImplicitSurfaceSet2::Builder final : public SurfaceBuilderBase2<Builder>
{
 public:
    //! Returns builder with surfaces.
    [[nodiscard]] Builder& WithSurfaces(
        const Array1<ImplicitSurface2Ptr>& surfaces);

    //! Returns builder with explicit surfaces.
    [[nodiscard]] Builder& WithExplicitSurfaces(
        const Array1<Surface2Ptr>& surfaces);

    //! Builds ImplicitSurfaceSet2.
    [[nodiscard]] ImplicitSurfaceSet2 Build() const;

    //! Builds shared pointer of ImplicitSurfaceSet2 instance.
    [[nodiscard]] ImplicitSurfaceSet2Ptr MakeShared() const;

 private:
    Array1<ImplicitSurface2Ptr> m_surfaces;
};
}  // namespace CubbyFlow

#endif