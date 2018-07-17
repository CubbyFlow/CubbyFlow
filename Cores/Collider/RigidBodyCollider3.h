/*************************************************************************
> File Name: RigidBodyCollider3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D rigid body collider class.
> Created Time: 2017/06/23
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_RIGID_BODY_COLLIDER3_H
#define CUBBYFLOW_RIGID_BODY_COLLIDER3_H

#include <Collider/Collider3.h>

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

		//! Linear velocity of the rigid body.
		Vector3D linearVelocity;

		//! Angular velocity of the rigid body.
		Vector3D angularVelocity;

		//! Constructs a collider with a surface.
		explicit RigidBodyCollider3(const Surface3Ptr& surface);

		//! Constructs a collider with a surface and other parameters.
		RigidBodyCollider3(const Surface3Ptr& surface,
			const Vector3D& linearVelocity, const Vector3D& angularVelocity);

		//! Returns the velocity of the collider at given \p point.
		Vector3D VelocityAt(const Vector3D& point) const override;

		//! Returns builder fox RigidBodyCollider3.
		static Builder GetBuilder();
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
		Builder& WithSurface(const Surface3Ptr& surface);

		//! Returns builder with linear velocity.
		Builder& WithLinearVelocity(const Vector3D& linearVelocity);

		//! Returns builder with angular velocity.
		Builder& WithAngularVelocity(const Vector3D& angularVelocity);

		//! Builds RigidBodyCollider3.
		RigidBodyCollider3 Build() const;

		//! Builds shared pointer of RigidBodyCollider3 instance.
		RigidBodyCollider3Ptr MakeShared() const;

	private:
		Surface3Ptr m_surface;
		Vector3D m_linearVelocity{ 0, 0, 0 };
		Vector3D m_angularVelocity{ 0, 0, 0 };
	};
}

#endif