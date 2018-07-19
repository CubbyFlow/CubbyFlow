/*************************************************************************
> File Name: RigidBodyCollider3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D rigid body collider class.
> Created Time: 2017/06/23
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Collider/RigidBodyCollider3.h>

namespace CubbyFlow
{
	RigidBodyCollider3::RigidBodyCollider3(const Surface3Ptr& surface)
	{
		SetSurface(surface);
	}

	RigidBodyCollider3::RigidBodyCollider3(const Surface3Ptr& surface,
		const Vector3D& linearVelocity_, const Vector3D& angularVelocity_) :
		linearVelocity(linearVelocity_), angularVelocity(angularVelocity_)
	{
		SetSurface(surface);
	}

	Vector3D RigidBodyCollider3::VelocityAt(const Vector3D& point) const
	{
		Vector3D r = point - GetSurface()->transform.GetTranslation();
		return linearVelocity + angularVelocity.Cross(r);
	}

	RigidBodyCollider3::Builder RigidBodyCollider3::GetBuilder()
	{
		return Builder();
	}

	RigidBodyCollider3::Builder& RigidBodyCollider3::Builder::WithSurface(const Surface3Ptr& surface)
	{
		m_surface = surface;
		return *this;
	}

	RigidBodyCollider3::Builder& RigidBodyCollider3::Builder::WithLinearVelocity(const Vector3D& linearVelocity)
	{
		m_linearVelocity = linearVelocity;
		return *this;
	}

	RigidBodyCollider3::Builder& RigidBodyCollider3::Builder::WithAngularVelocity(const Vector3D& angularVelocity)
	{
		m_angularVelocity = angularVelocity;
		return *this;
	}

	RigidBodyCollider3 RigidBodyCollider3::Builder::Build() const
	{
		return RigidBodyCollider3(m_surface, m_linearVelocity, m_angularVelocity);
	}

	RigidBodyCollider3Ptr RigidBodyCollider3::Builder::MakeShared() const
	{
		return std::shared_ptr<RigidBodyCollider3>(
			new RigidBodyCollider3(m_surface, m_linearVelocity, m_angularVelocity),
			[](RigidBodyCollider3* obj)
		{
			delete obj;
		});
	}
}