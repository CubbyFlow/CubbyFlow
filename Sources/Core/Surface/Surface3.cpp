/*************************************************************************
> File Name: Surface3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D surface.
> Created Time: 2017/04/09
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Surface/Surface3.h>

namespace CubbyFlow
{
	Surface3::Surface3(const Transform3& transform_ , bool isNormalFlipped_) : 
		transform(transform_), isNormalFlipped(isNormalFlipped_)
	{
		// Do nothing
	}

	Surface3::Surface3(const Surface3& other) :
		transform(other.transform), isNormalFlipped(other.isNormalFlipped)
	{
		// Do nothing
	}

	Surface3::~Surface3()
	{
		// Do nothing
	}

	Vector3D Surface3::ClosestPoint(const Vector3D& otherPoint) const
	{
		return transform.ToWorld(ClosestPointLocal(transform.ToLocal(otherPoint)));
	}

	BoundingBox3D Surface3::BoundingBox() const
	{
		return transform.ToWorld(BoundingBoxLocal());
	}

	bool Surface3::Intersects(const Ray3D& ray) const
	{
		return IntersectsLocal(transform.ToLocal(ray));
	}

	double Surface3::ClosestDistance(const Vector3D& otherPoint) const
	{
		return ClosestDistanceLocal(transform.ToLocal(otherPoint));
	}

	SurfaceRayIntersection3 Surface3::ClosestIntersection(const Ray3D& ray) const
	{
		auto result = ClosestIntersectionLocal(transform.ToLocal(ray));

		result.point = transform.ToWorld(result.point);
		result.normal = transform.ToWorldDirection(result.normal);
		result.normal *= (isNormalFlipped) ? -1.0 : 1.0;

		return result;
	}

	Vector3D Surface3::ClosestNormal(const Vector3D& otherPoint) const
	{
		auto result = transform.ToWorldDirection(ClosestNormalLocal(transform.ToLocal(otherPoint)));
		result *= (isNormalFlipped) ? -1.0 : 1.0;
		return result;
	}

	bool Surface3::IntersectsLocal(const Ray3D& ray) const
	{
		auto result = ClosestIntersectionLocal(ray);
		return result.isIntersecting;
	}

	void Surface3::UpdateQueryEngine()
	{
		// Do nothing
	}

	double Surface3::ClosestDistanceLocal(const Vector3D& otherPoint) const
	{
		return otherPoint.DistanceTo(ClosestPointLocal(otherPoint));
	}
}