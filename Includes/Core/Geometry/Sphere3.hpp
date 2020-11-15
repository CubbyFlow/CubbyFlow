// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SPHERE3_HPP
#define CUBBYFLOW_SPHERE3_HPP

#include <Core/Geometry/Surface3.hpp>

namespace CubbyFlow
{
//!
//! \brief 3-D sphere geometry.
//!
//! This class represents 3-D sphere geometry which extends Surface3 by
//! overriding surface-related queries.
//!
class Sphere3 final : public Surface3
{
 public:
    class Builder;

    //! Constructs a sphere with center at (0, 0, 0) and radius of 1.
    Sphere3(const Transform3& _transform = Transform3{},
            bool _isNormalFlipped = false);

    //! Constructs a sphere with \p _center and \p _radius.
    Sphere3(const Vector3D& _center, double _radius,
            const Transform3& _transform = Transform3{},
            bool _isNormalFlipped = false);

    //! Default copy constructor.
    Sphere3(const Sphere3&) = default;

    //! Default move constructor.
    Sphere3(Sphere3&&) noexcept = default;

    //! Default virtual destructor.
    ~Sphere3() override = default;

    //! Default copy assignment operator.
    Sphere3& operator=(const Sphere3&) = default;

    //! Default move assignment operator.
    Sphere3& operator=(Sphere3&&) noexcept = default;

    //! Returns builder fox Sphere3.
    [[nodiscard]] static Builder GetBuilder();

    //! Center of the sphere.
    Vector3D center;

    //! Radius of the sphere.
    double radius = 1.0;

 private:
    [[nodiscard]] Vector3D ClosestPointLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] double ClosestDistanceLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] bool IntersectsLocal(const Ray3D& ray) const override;

    [[nodiscard]] BoundingBox3D BoundingBoxLocal() const override;

    [[nodiscard]] Vector3D ClosestNormalLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const override;
};

//! Shared pointer for the Sphere3 type.
using Sphere3Ptr = std::shared_ptr<Sphere3>;

//!
//! \brief Front-end to create Sphere3 objects step by step.
//!
class Sphere3::Builder final : public SurfaceBuilderBase3<Builder>
{
 public:
    //! Returns builder with sphere center.
    [[nodiscard]] Builder& WithCenter(const Vector3D& _center);

    //! Returns builder with sphere radius.
    [[nodiscard]] Builder& WithRadius(double _radius);

    //! Builds Sphere3.
    [[nodiscard]] Sphere3 Build() const;

    //! Builds shared pointer of Sphere3 instance.
    [[nodiscard]] Sphere3Ptr MakeShared() const;

 private:
    Vector3D m_center = { 0, 0, 0 };
    double m_radius = 0.0;
};
}  // namespace CubbyFlow

#endif