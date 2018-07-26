/*************************************************************************
> File Name: RigidBodyCollider2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D rigid body collider class.
> Created Time: 2017/06/23
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Collider/RigidBodyCollider2.h>

namespace CubbyFlow
{
	RigidBodyCollider2::RigidBodyCollider2(const Surface2Ptr& surface)
	{
		SetSurface(surface);
	}

	RigidBodyCollider2::RigidBodyCollider2(const Surface2Ptr& surface,
		const Vector2D& linearVelocity_, double angularVelocity_) :
		linearVelocity(linearVelocity_), angularVelocity(angularVelocity_)
	{
		SetSurface(surface);
	}

	Vector2D RigidBodyCollider2::VelocityAt(const Vector2D& point) const
	{
		Vector2D r = point - GetSurface()->transform.GetTranslation();
		return linearVelocity + angularVelocity * Vector2D(-r.y, r.x);
	}

	RigidBodyCollider2::Builder RigidBodyCollider2::GetBuilder()
	{
		return Builder();
	}

	RigidBodyCollider2::Builder& RigidBodyCollider2::Builder::WithSurface(const Surface2Ptr& surface)
	{
		m_surface = surface;
		return *this;
	}

	RigidBodyCollider2::Builder& RigidBodyCollider2::Builder::WithLinearVelocity(const Vector2D& linearVelocity)
	{
		m_linearVelocity = linearVelocity;
		return *this;
	}

	RigidBodyCollider2::Builder& RigidBodyCollider2::Builder::WithAngularVelocity(double angularVelocity)
	{
		m_angularVelocity = angularVelocity;
		return *this;
	}

	RigidBodyCollider2 RigidBodyCollider2::Builder::Build() const
	{
		return RigidBodyCollider2(m_surface, m_linearVelocity, m_angularVelocity);
	}

	RigidBodyCollider2Ptr RigidBodyCollider2::Builder::MakeShared() const
	{
		return std::shared_ptr<RigidBodyCollider2>(
			new RigidBodyCollider2(m_surface, m_linearVelocity, m_angularVelocity),
			[](RigidBodyCollider2* obj)
		{
			delete obj;
		});
	}
}