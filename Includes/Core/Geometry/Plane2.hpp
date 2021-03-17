// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PLANE2_HPP
#define CUBBYFLOW_PLANE2_HPP

#include <Core/Geometry/Surface.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D plane geometry.
//!
//! This class represents 2-D plane geometry which extends Surface2 by
//! overriding surface-related queries.
//!
class Plane2 final : public Surface2
{
 public:
    class Builder;

    //! Constructs a plane that crosses (0, 0) with surface normal (0, 1).
    Plane2(const Transform2& _transform = Transform2{},
           bool _isNormalFlipped = false);

    //! Constructs a plane that cross \p _point with surface normal \p _normal.
    Plane2(const Vector2D& _normal, const Vector2D& _point,
           const Transform2& _transform = Transform2{},
           bool _isNormalFlipped = false);

    //! Default copy constructor.
    Plane2(const Plane2&) = default;

    //! Default move constructor.
    Plane2(Plane2&&) noexcept = default;

    //! Default virtual destructor.
    ~Plane2() override = default;

    //! Default copy assignment operator.
    Plane2& operator=(const Plane2&) = default;

    //! Default move assignment operator.
    Plane2& operator=(Plane2&&) noexcept = default;

    //! Returns true if bounding box can be defined.
    [[nodiscard]] bool IsBounded() const override;

    //! Returns builder fox Plane2.
    [[nodiscard]] static Builder GetBuilder();

    //! Plane normal.
    Vector2D normal = Vector2D{ 0, 1 };

    //! Point that lies on the plane.
    Vector2D point;

 private:
    [[nodiscard]] Vector2D ClosestPointLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] bool IntersectsLocal(const Ray2D& ray) const override;

    [[nodiscard]] BoundingBox2D BoundingBoxLocal() const override;

    [[nodiscard]] Vector2D ClosestNormalLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] SurfaceRayIntersection2 ClosestIntersectionLocal(
        const Ray2D& ray) const override;
};

//! Shared pointer for the Plane2 type.
using Plane2Ptr = std::shared_ptr<Plane2>;

//!
//! \brief Front-end to create Plane2 objects step by step.
//!
class Plane2::Builder final : public SurfaceBuilderBase2<Builder>
{
 public:
    //! Returns builder with plane normal.
    [[nodiscard]] Builder& WithNormal(const Vector2D& _normal);

    //! Returns builder with point on the plane.
    [[nodiscard]] Builder& WithPoint(const Vector2D& _point);

    //! Builds Plane2.
    [[nodiscard]] Plane2 Build() const;

    //! Builds shared pointer of Plane2 instance.
    [[nodiscard]] Plane2Ptr MakeShared() const;

 private:
    Vector2D m_normal{ 0, 1 };
    Vector2D m_point{ 0, 0 };
};
}  // namespace CubbyFlow

#endif