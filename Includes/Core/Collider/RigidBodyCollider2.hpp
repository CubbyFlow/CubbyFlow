// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_RIGID_BODY_COLLIDER2_HPP
#define CUBBYFLOW_RIGID_BODY_COLLIDER2_HPP

#include <Core/Collider/Collider2.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D rigid body collider class.
//!
//! This class implements 2-D rigid body collider. The collider can only take
//! rigid body motion with linear and rotational velocities.
//!
class RigidBodyCollider2 final : public Collider2
{
 public:
    class Builder;

    //! Constructs a collider with a surface.
    explicit RigidBodyCollider2(const Surface2Ptr& surface);

    //! Constructs a collider with a surface and other parameters.
    RigidBodyCollider2(const Surface2Ptr& surface,
                       const Vector2D& _linearVelocity,
                       double _angularVelocity);

    //! Returns the velocity of the collider at given \p point.
    [[nodiscard]] Vector2D VelocityAt(const Vector2D& point) const override;

    //! Returns builder fox RigidBodyCollider2.
    [[nodiscard]] static Builder GetBuilder();

    //! Linear velocity of the rigid body.
    Vector2D linearVelocity;

    //! Angular velocity of the rigid body.
    double angularVelocity = 0.0;
};

//! Shared pointer for the RigidBodyCollider2 type.
using RigidBodyCollider2Ptr = std::shared_ptr<RigidBodyCollider2>;

//!
//! \brief Front-end to create RigidBodyCollider2 objects step by step.
//!
class RigidBodyCollider2::Builder final
{
 public:
    //! Returns builder with surface.
    [[nodiscard]] Builder& WithSurface(const Surface2Ptr& surface);

    //! Returns builder with linear velocity.
    [[nodiscard]] Builder& WithLinearVelocity(const Vector2D& _linearVelocity);

    //! Returns builder with angular velocity.
    [[nodiscard]] Builder& WithAngularVelocity(double _angularVelocity);

    //! Builds RigidBodyCollider2.
    [[nodiscard]] RigidBodyCollider2 Build() const;

    //! Builds shared pointer of RigidBodyCollider2 instance.
    [[nodiscard]] RigidBodyCollider2Ptr MakeShared() const;

 private:
    Surface2Ptr m_surface;
    Vector2D m_linearVelocity{ 0, 0 };
    double m_angularVelocity = 0.0;
};
}  // namespace CubbyFlow

#endif