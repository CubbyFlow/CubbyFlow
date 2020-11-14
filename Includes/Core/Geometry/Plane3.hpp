// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PLANE3_HPP
#define CUBBYFLOW_PLANE3_HPP

#include <Core/Surface/Surface3.hpp>

namespace CubbyFlow
{
//!
//! \brief 3-D plane geometry.
//!
//! This class represents 3-D plane geometry which extends Surface3 by
//! overriding surface-related queries.
//!
class Plane3 final : public Surface3
{
 public:
    class Builder;

    //! Constructs a plane that crosses (0, 0, 0) with surface normal (0, 1, 0).
    Plane3(const Transform3& _transform = Transform3{},
           bool _isNormalFlipped = false);

    //! Constructs a plane that cross \p point with surface normal \p normal.
    Plane3(const Vector3D& _normal, const Vector3D& _point,
           const Transform3& _transform = Transform3{},
           bool _isNormalFlipped = false);

    //! Constructs a plane with three points on the surface. The normal will be
    //! set using the counter clockwise direction.
    Plane3(const Vector3D& point0, const Vector3D& point1,
           const Vector3D& point2, const Transform3& _transform = Transform3{},
           bool _isNormalFlipped = false);

    //! Default copy constructor.
    Plane3(const Plane3&) = default;

    //! Default move constructor.
    Plane3(Plane3&&) noexcept = default;

    //! Default virtual destructor.
    ~Plane3() override = default;

    //! Default copy assignment operator.
    Plane3& operator=(const Plane3&) = default;

    //! Default move assignment operator.
    Plane3& operator=(Plane3&&) noexcept = default;

    //! Returns true if bounding box can be defined.
    [[nodiscard]] bool IsBounded() const override;

    //! Returns builder fox Plane3.
    [[nodiscard]] static Builder GetBuilder();

    //! Plane normal.
    Vector3D normal = Vector3D{ 0, 1, 0 };

    //! Point that lies on the plane.
    Vector3D point;

 protected:
    [[nodiscard]] Vector3D ClosestPointLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] bool IntersectsLocal(const Ray3D& ray) const override;

    [[nodiscard]] BoundingBox3D BoundingBoxLocal() const override;

    [[nodiscard]] Vector3D ClosestNormalLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const override;
};

//! Shared pointer for the Plane3 type.
using Plane3Ptr = std::shared_ptr<Plane3>;

//!
//! \brief Front-end to create Plane3 objects step by step.
//!
class Plane3::Builder final : public SurfaceBuilderBase3<Builder>
{
 public:
    //! Returns builder with plane normal.
    [[nodiscard]] Builder& WithNormal(const Vector3D& _normal);

    //! Returns builder with point on the plane.
    [[nodiscard]] Builder& WithPoint(const Vector3D& _point);

    //! Builds Plane3.
    [[nodiscard]] Plane3 Build() const;

    //! Builds shared pointer of Plane3 instance.
    [[nodiscard]] Plane3Ptr MakeShared() const;

 private:
    Vector3D m_normal{ 0, 1, 0 };
    Vector3D m_point{ 0, 0, 0 };
};
}  // namespace CubbyFlow

#endif