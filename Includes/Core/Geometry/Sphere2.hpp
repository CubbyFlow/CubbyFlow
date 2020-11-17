// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SPHERE2_HPP
#define CUBBYFLOW_SPHERE2_HPP

#include <Core/Geometry/Surface2.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D sphere geometry.
//!
//! This class represents 2-D sphere geometry which extends Surface2 by
//! overriding surface-related queries.
//!
class Sphere2 final : public Surface2
{
 public:
    class Builder;

    //! Constructs a sphere with center at (0, 0) and radius of 1.
    Sphere2(const Transform2& _transform = Transform2{},
            bool _isNormalFlipped = false);

    //! Constructs a sphere with \p _center and \p _radius.
    Sphere2(const Vector2D& _center, double _radius,
            const Transform2& _transform = Transform2{},
            bool _isNormalFlipped = false);

    //! Default copy constructor.
    Sphere2(const Sphere2&) = default;

    //! Default move constructor.
    Sphere2(Sphere2&&) noexcept = default;

    //! Default virtual destructor.
    ~Sphere2() override = default;

    //! Default copy assignment operator.
    Sphere2& operator=(const Sphere2&) = default;

    //! Default move assignment operator.
    Sphere2& operator=(Sphere2&&) noexcept = default;

    //! Returns builder fox Sphere2.
    [[nodiscard]] static Builder GetBuilder();

    //! Center of the sphere.
    Vector2D center;

    //! Radius of the sphere.
    double radius = 1.0;

 private:
    [[nodiscard]] Vector2D ClosestPointLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] double ClosestDistanceLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] bool IntersectsLocal(const Ray2D& ray) const override;

    [[nodiscard]] BoundingBox2D BoundingBoxLocal() const override;

    [[nodiscard]] Vector2D ClosestNormalLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] SurfaceRayIntersection2 ClosestIntersectionLocal(
        const Ray2D& ray) const override;
};

//! Shared pointer for the Sphere2 type.
using Sphere2Ptr = std::shared_ptr<Sphere2>;

//!
//! \brief Front-end to create Sphere2 objects step by step.
//!
class Sphere2::Builder final : public SurfaceBuilderBase2<Builder>
{
 public:
    //! Returns builder with sphere center.
    [[nodiscard]] Builder& WithCenter(const Vector2D& _center);

    //! Returns builder with sphere radius.
    [[nodiscard]] Builder& WithRadius(double _radius);

    //! Builds Sphere2.
    [[nodiscard]] Sphere2 Build() const;

    //! Builds shared pointer of Sphere2 instance.
    [[nodiscard]] Sphere2Ptr MakeShared() const;

 private:
    Vector2D m_center = { 0, 0 };
    double m_radius = 0.0;
};
}  // namespace CubbyFlow

#endif