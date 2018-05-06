/*************************************************************************
> File Name: ColliderSet3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Collection of 3-D colliders.
> Created Time: 2017/05/20
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Collider/ColliderSet3.h>
#include <Core/Surface/SurfaceSet3.h>

namespace CubbyFlow
{
	ColliderSet3::ColliderSet3() :
		ColliderSet3(std::vector<Collider3Ptr>())
	{
		// Do nothing
	}

	ColliderSet3::ColliderSet3(const std::vector<Collider3Ptr>& others)
	{
		SetSurface(std::make_shared<SurfaceSet3>());
		for (const auto& collider : others)
		{
			AddCollider(collider);
		}
	}

	Vector3D ColliderSet3::VelocityAt(const Vector3D& point) const
	{
		size_t closestCollider = std::numeric_limits<size_t>::max();
		double closestDist = std::numeric_limits<double>::max();

		for (size_t i = 0; i < m_colliders.size(); ++i)
		{
			double dist = m_colliders[i]->GetSurface()->ClosestDistance(point);
			
			if (dist < closestDist)
			{
				closestDist = dist;
				closestCollider = i;
			}
		}

		if (closestCollider != std::numeric_limits<size_t>::max())
		{
			return m_colliders[closestCollider]->VelocityAt(point);
		}
		
		return Vector3D();
	}

	void ColliderSet3::AddCollider(const Collider3Ptr& collider)
	{
		auto surfaceSet = std::dynamic_pointer_cast<SurfaceSet3>(GetSurface());
		m_colliders.push_back(collider);
		surfaceSet->AddSurface(collider->GetSurface());
	}

	size_t ColliderSet3::NumberOfColliders() const
	{
		return m_colliders.size();
	}

	Collider3Ptr ColliderSet3::Collider(size_t i) const
	{
		return m_colliders[i];
	}

	ColliderSet3::Builder ColliderSet3::GetBuilder()
	{
		return Builder();
	}

	ColliderSet3::Builder& ColliderSet3::Builder::WithColliders(const std::vector<Collider3Ptr>& others)
	{
		m_colliders = others;
		return *this;
	}

	ColliderSet3 ColliderSet3::Builder::Build() const
	{
		return ColliderSet3(m_colliders);
	}

	ColliderSet3Ptr ColliderSet3::Builder::MakeShared() const
	{
		return std::shared_ptr<ColliderSet3>(new ColliderSet3(m_colliders), [](ColliderSet3* obj)
		{
			delete obj;
		});
	}
}