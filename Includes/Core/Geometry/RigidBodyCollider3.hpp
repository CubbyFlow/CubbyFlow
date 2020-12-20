// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_RIGID_BODY_COLLIDER3_HPP
#define CUBBYFLOW_RIGID_BODY_COLLIDER3_HPP

#include <Core/Geometry/Collider3.hpp>

namespace CubbyFlow
{
//!
//! \brief 3-D rigid body collider class.
//!
//! This class implements 3-D rigid body collider. The collider can only take
//! rigid body motion with linear and rotational velocities.
//!
class RigidBodyCollider3 final : public Collider3
{
 public:
    class Builder;

    //! Constructs a collider with a surface.
    explicit RigidBodyCollider3(const Surface3Ptr& surface);

    //! Constructs a collider with a surface and other parameters.
    RigidBodyCollider3(const Surface3Ptr& surface,
                       const Vector3D& _linearVelocity,
                       const Vector3D& _angularVelocity);

    //! Returns the velocity of the collider at given \p point.
    [[nodiscard]] Vector3D VelocityAt(const Vector3D& point) const override;

    //! Returns builder fox RigidBodyCollider3.
    [[nodiscard]] static Builder GetBuilder();

    //! Linear velocity of the rigid body.
    Vector3D linearVelocity;

    //! Angular velocity of the rigid body.
    Vector3D angularVelocity;
};

//! Shared pointer for the RigidBodyCollider3 type.
using RigidBodyCollider3Ptr = std::shared_ptr<RigidBodyCollider3>;

//!
//! \brief Front-end to create RigidBodyCollider3 objects step by step.
//!
class RigidBodyCollider3::Builder final
{
 public:
    //! Returns builder with surface.
    [[nodiscard]] Builder& WithSurface(const Surface3Ptr& surface);

    //! Returns builder with linear velocity.
    [[nodiscard]] Builder& WithLinearVelocity(const Vector3D& _linearVelocity);

    //! Returns builder with angular velocity.
    [[nodiscard]] Builder& WithAngularVelocity(
        const Vector3D& _angularVelocity);

    //! Builds RigidBodyCollider3.
    [[nodiscard]] RigidBodyCollider3 Build() const;

    //! Builds shared pointer of RigidBodyCollider3 instance.
    [[nodiscard]] RigidBodyCollider3Ptr MakeShared() const;

 private:
    Surface3Ptr m_surface;
    Vector3D m_linearVelocity{ 0, 0, 0 };
    Vector3D m_angularVelocity{ 0, 0, 0 };
};
}  // namespace CubbyFlow

#endif