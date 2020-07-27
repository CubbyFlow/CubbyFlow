// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SURFACE_TO_IMPLICIT3_HPP
#define CUBBYFLOW_SURFACE_TO_IMPLICIT3_HPP

#include <Core/Surface/ImplicitSurface3.hpp>

namespace CubbyFlow
{
//!
//! \brief 3-D implicit surface wrapper for generic Surface3 instance.
//!
//! This class represents 3-D implicit surface that converts Surface3 instance.
//! to an ImplicitSurface3 object. The conversion is made by evaluating closest
//! point and normal from a given point for the given (explicit) surface. Thus,
//! this conversion won't work for every single surfaces, especially
//! TriangleMesh3. To use TriangleMesh3 as an ImplicitSurface3 instance,
//! please take a look at ImplicitTriangleMesh3. Use this class only
//! for the basic primitives such as Sphere3 or Box3.
//!
class SurfaceToImplicit3 final : public ImplicitSurface3
{
 public:
    class Builder;

    //! Constructs an instance with generic Surface3 instance.
    SurfaceToImplicit3(const Surface3Ptr& surface,
                       const Transform3& transform = Transform3(),
                       bool isNormalFlipped = false);

    //! Copy constructor.
    SurfaceToImplicit3(const SurfaceToImplicit3& other);

    //! Updates internal spatial query engine.
    void UpdateQueryEngine() override;

    //! Returns true if bounding box can be defined.
    bool IsBounded() const override;

    //! Returns true if the surface is a valid geometry.
    bool IsValidGeometry() const override;

    //! Returns the raw surface instance.
    Surface3Ptr GetSurface() const;

    //! Returns builder for SurfaceToImplicit3.
    static Builder GetBuilder();

 protected:
    Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

    double ClosestDistanceLocal(const Vector3D& otherPoint) const override;

    bool IntersectsLocal(const Ray3D& ray) const override;

    BoundingBox3D BoundingBoxLocal() const override;

    Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

    double SignedDistanceLocal(const Vector3D& otherPoint) const override;

    SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const override;

    bool IsInsideLocal(const Vector3D& otherPoint) const override;

 private:
    Surface3Ptr m_surface;
};

//! Shared pointer for the SurfaceToImplicit3
using SurfaceToImplicit3Ptr = std::shared_ptr<SurfaceToImplicit3>;

//!
//! \brief Front-end to create SurfaceToImplicit3 objects step by step.
//!
class SurfaceToImplicit3::Builder final
    : public SurfaceBuilderBase3<SurfaceToImplicit3::Builder>
{
 public:
    //! Returns builder with surface.
    Builder& WithSurface(const Surface3Ptr& surface);

    //! Builds SurfaceToImplicit3.
    SurfaceToImplicit3 Build() const;

    //! Builds shared pointer of SurfaceToImplicit3 instance.
    SurfaceToImplicit3Ptr MakeShared() const;

 private:
    Surface3Ptr m_surface;
};
}  // namespace CubbyFlow

#endif