/*************************************************************************
> File Name: SurfaceToImplicit2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D implicit surface wrapper for generic Surface2 instance.
> Created Time: 2017/04/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Surface/SurfaceToImplicit2.h>

namespace CubbyFlow
{
	SurfaceToImplicit2::SurfaceToImplicit2(
		const Surface2Ptr& surface,
		const Transform2& transform,
		bool isNormalFlipped) :
		ImplicitSurface2(transform, isNormalFlipped), m_surface(surface)
	{
		// Do nothing
	}

	SurfaceToImplicit2::SurfaceToImplicit2(const SurfaceToImplicit2& other) :
		ImplicitSurface2(other), m_surface(other.m_surface)
	{
		// Do nothing
	}

	Surface2Ptr SurfaceToImplicit2::GetSurface() const
	{
		return m_surface;
	}

	SurfaceToImplicit2::Builder SurfaceToImplicit2::GetBuilder()
	{
		return Builder();
	}

	Vector2D SurfaceToImplicit2::ClosestPointLocal(const Vector2D& otherPoint) const
	{
		return m_surface->ClosestPoint(otherPoint);
	}

	Vector2D SurfaceToImplicit2::ClosestNormalLocal(const Vector2D& otherPoint) const
	{
		return m_surface->ClosestNormal(otherPoint);
	}

	double SurfaceToImplicit2::ClosestDistanceLocal(const Vector2D& otherPoint) const
	{
		return m_surface->ClosestDistance(otherPoint);
	}

	bool SurfaceToImplicit2::IntersectsLocal(const Ray2D& ray) const
	{
		return m_surface->Intersects(ray);
	}

	SurfaceRayIntersection2 SurfaceToImplicit2::ClosestIntersectionLocal(const Ray2D& ray) const
	{
		return m_surface->ClosestIntersection(ray);
	}

	BoundingBox2D SurfaceToImplicit2::BoundingBoxLocal() const
	{
		return m_surface->BoundingBox();
	}

	double SurfaceToImplicit2::SignedDistanceLocal(const Vector2D& otherPoint) const
	{
		Vector2D x = m_surface->ClosestPoint(otherPoint);
		Vector2D n = m_surface->ClosestNormal(otherPoint);

		n = (isNormalFlipped) ? -n : n;

		if (n.Dot(otherPoint - x) < 0.0)
		{
			return -x.DistanceTo(otherPoint);
		}

		return x.DistanceTo(otherPoint);
	}

	SurfaceToImplicit2::Builder& SurfaceToImplicit2::Builder::WithSurface(const Surface2Ptr& surface)
	{
		m_surface = surface;
		return *this;
	}

	SurfaceToImplicit2 SurfaceToImplicit2::Builder::Build() const
	{
		return SurfaceToImplicit2(m_surface, m_transform, m_isNormalFlipped);
	}

	SurfaceToImplicit2Ptr SurfaceToImplicit2::Builder::MakeShared() const
	{
		return std::shared_ptr<SurfaceToImplicit2>(
			new SurfaceToImplicit2(m_surface, m_transform, m_isNormalFlipped),
			[](SurfaceToImplicit2* obj)
		{
			delete obj;
		});
	}
}