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

#include <Core/Surface/Surface3.hpp>

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

    //! Center of the sphere.
    Vector3D center;

    //! Radius of the sphere.
    double radius = 1.0;

    //! Constructs a sphere with center at (0, 0, 0) and radius of 1.
    Sphere3(const Transform3& transform = Transform3(),
            bool isNormalFlipped = false);

    //! Constructs a sphere with \p center and \p radius.
    Sphere3(const Vector3D& center, double radius,
            const Transform3& transform = Transform3(),
            bool isNormalFlipped = false);

    //! Copy constructor.
    Sphere3(const Sphere3& other);

    //! Default copy assignment operator.
    Sphere3& operator=(const Sphere3& other) = default;

    //! Returns builder fox Sphere3.
    static Builder GetBuilder();

 private:
    Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

    double ClosestDistanceLocal(const Vector3D& otherPoint) const override;

    bool IntersectsLocal(const Ray3D& ray) const override;

    BoundingBox3D BoundingBoxLocal() const override;

    Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

    SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const override;
};

//! Shared pointer for the Sphere3 type.
using Sphere3Ptr = std::shared_ptr<Sphere3>;

//!
//! \brief Front-end to create Sphere3 objects step by step.
//!
class Sphere3::Builder final : public SurfaceBuilderBase3<Sphere3::Builder>
{
 public:
    //! Returns builder with sphere center.
    Builder& WithCenter(const Vector3D& center);

    //! Returns builder with sphere radius.
    Builder& WithRadius(double radius);

    //! Builds Sphere3.
    Sphere3 Build() const;

    //! Builds shared pointer of Sphere3 instance.
    Sphere3Ptr MakeShared() const;

 private:
    Vector3D m_center = { 0, 0, 0 };
    double m_radius = 0.0;
};
}  // namespace CubbyFlow

#endif