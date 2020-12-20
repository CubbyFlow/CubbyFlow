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

#include <Core/Geometry/ImplicitSurface3.hpp>

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
    explicit SurfaceToImplicit3(const Surface3Ptr& surface,
                                const Transform3& _transform = Transform3{},
                                bool _isNormalFlipped = false);

    //! Default copy constructor.
    SurfaceToImplicit3(const SurfaceToImplicit3&) = default;

    //! Default move constructor.
    SurfaceToImplicit3(SurfaceToImplicit3&&) noexcept = default;

    //! Default virtual destructor.
    ~SurfaceToImplicit3() override = default;

    //! Default copy assignment operator.
    SurfaceToImplicit3& operator=(const SurfaceToImplicit3&) = default;

    //! Default move assignment operator.
    SurfaceToImplicit3& operator=(SurfaceToImplicit3&&) = default;

    //! Updates internal spatial query engine.
    void UpdateQueryEngine() override;

    //! Returns true if bounding box can be defined.
    [[nodiscard]] bool IsBounded() const override;

    //! Returns true if the surface is a valid geometry.
    [[nodiscard]] bool IsValidGeometry() const override;

    //! Returns the raw surface instance.
    [[nodiscard]] Surface3Ptr GetSurface() const;

    //! Returns builder for SurfaceToImplicit3.
    [[nodiscard]] static Builder GetBuilder();

 protected:
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

    [[nodiscard]] bool IsInsideLocal(const Vector3D& otherPoint) const override;

 private:
    Surface3Ptr m_surface;
};

//! Shared pointer for the SurfaceToImplicit3
using SurfaceToImplicit3Ptr = std::shared_ptr<SurfaceToImplicit3>;

//!
//! \brief Front-end to create SurfaceToImplicit3 objects step by step.
//!
class SurfaceToImplicit3::Builder final : public SurfaceBuilderBase3<Builder>
{
 public:
    //! Returns builder with surface.
    [[nodiscard]] Builder& WithSurface(const Surface3Ptr& surface);

    //! Builds SurfaceToImplicit3.
    [[nodiscard]] SurfaceToImplicit3 Build() const;

    //! Builds shared pointer of SurfaceToImplicit3 instance.
    [[nodiscard]] SurfaceToImplicit3Ptr MakeShared() const;

 private:
    Surface3Ptr m_surface;
};
}  // namespace CubbyFlow

#endif