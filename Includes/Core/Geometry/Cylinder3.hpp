// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CYLINDER3_HPP
#define CUBBYFLOW_CYLINDER3_HPP

#include <Core/Surface/Surface3.h>

namespace CubbyFlow
{
//!
//! \brief 3-D cylinder geometry.
//!
//! This class represents 3-D cylinder geometry which extends Surface3 by
//! overriding surface-related queries. The cylinder is aligned with the y-axis.
//!
class Cylinder3 final : public Surface3
{
 public:
    class Builder;

    //! Center of the cylinder.
    Vector3D center;

    //! Radius of the cylinder.
    double radius = 1.0;

    //! Height of the cylinder.
    double height = 1.0;

    //! Constructs a cylinder with
    Cylinder3(const Transform3& transform = Transform3(),
              bool isNormalFlipped = false);

    //! Constructs a cylinder with \p center, \p radius, and \p height.
    Cylinder3(const Vector3D& center, double radius, double height,
              const Transform3& transform = Transform3(),
              bool isNormalFlipped = false);

    //! Copy constructor.
    Cylinder3(const Cylinder3& other);

    //! Returns builder fox Cylinder3.
    static Builder GetBuilder();

 protected:
    // Surface3 implementations

    Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

    double ClosestDistanceLocal(const Vector3D& otherPoint) const override;

    bool IntersectsLocal(const Ray3D& ray) const override;

    BoundingBox3D BoundingBoxLocal() const override;

    Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

    SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const override;
};

//! Shared pointer type for the Cylinder3.
using Cylinder3Ptr = std::shared_ptr<Cylinder3>;

//!
//! \brief Front-end to create Cylinder3 objects step by step.
//!
class Cylinder3::Builder final : public SurfaceBuilderBase3<Cylinder3::Builder>
{
 public:
    //! Returns builder with center.
    Builder& WithCenter(const Vector3D& center);

    //! Returns builder with radius.
    Builder& WithRadius(double radius);

    //! Returns builder with height.
    Builder& WithHeight(double height);

    //! Builds Cylinder3.
    Cylinder3 Build() const;

    //! Builds shared pointer of Cylinder3 instance.
    Cylinder3Ptr MakeShared() const;

 private:
    Vector3D m_center{ 0, 0, 0 };
    double m_radius = 1.0;
    double m_height = 1.0;
};
}  // namespace CubbyFlow

#endif