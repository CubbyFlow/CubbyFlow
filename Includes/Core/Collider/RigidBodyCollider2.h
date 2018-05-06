/*************************************************************************
> File Name: RigidBodyCollider2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D rigid body collider class.
> Created Time: 2017/06/23
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_RIGID_BODY_COLLIDER2_H
#define CUBBYFLOW_RIGID_BODY_COLLIDER2_H

#include <Core/Collider/Collider2.h>

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

		//! Linear velocity of the rigid body.
		Vector2D linearVelocity;

		//! Angular velocity of the rigid body.
		double angularVelocity = 0.0;

		//! Constructs a collider with a surface.
		explicit RigidBodyCollider2(const Surface2Ptr& surface);

		//! Constructs a collider with a surface and other parameters.
		RigidBodyCollider2(const Surface2Ptr& surface,
			const Vector2D& linearVelocity, double angularVelocity);

		//! Returns the velocity of the collider at given \p point.
		Vector2D VelocityAt(const Vector2D& point) const override;

		//! Returns builder fox RigidBodyCollider2.
		static Builder GetBuilder();
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
		Builder& WithSurface(const Surface2Ptr& surface);

		//! Returns builder with linear velocity.
		Builder& WithLinearVelocity(const Vector2D& linearVelocity);

		//! Returns builder with angular velocity.
		Builder& WithAngularVelocity(double angularVelocity);

		//! Builds RigidBodyCollider2.
		RigidBodyCollider2 Build() const;

		//! Builds shared pointer of RigidBodyCollider2 instance.
		RigidBodyCollider2Ptr MakeShared() const;

	private:
		Surface2Ptr m_surface;
		Vector2D m_linearVelocity{ 0, 0 };
		double m_angularVelocity = 0.0;
	};
}

#endif