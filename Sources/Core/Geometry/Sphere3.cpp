/*************************************************************************
> File Name: Sphere3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D sphere geometry.
> Created Time: 2017/04/11
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Geometry/Sphere3.h>
#include <Core/Math/MathUtils.h>

namespace CubbyFlow
{
	Sphere3::Sphere3(const Transform3& transform_, bool isNormalFlipped_) :
		Surface3(transform_, isNormalFlipped_)
	{
		// Do nothing
	}

	Sphere3::Sphere3(const Vector3D& center_, double radius_, const Transform3& transform_, bool isNormalFlipped_) :
		Surface3(transform_, isNormalFlipped_), center(center_), radius(radius_)
	{
		// Do nothing
	}

	Sphere3::Sphere3(const Sphere3& other) :
		Surface3(other.transform, other.isNormalFlipped), center(other.center), radius(other.radius)
	{
		// Do nothing
	}

	Vector3D Sphere3::ClosestPointLocal(const Vector3D& otherPoint) const
	{
		return radius * ClosestNormalLocal(otherPoint) + center;
	}

	double Sphere3::ClosestDistanceLocal(const Vector3D& otherPoint) const
	{
		return std::fabs(center.DistanceTo(otherPoint) - radius);
	}

	Vector3D Sphere3::ClosestNormalLocal(const Vector3D& otherPoint) const
	{
		if (center.IsSimilar(otherPoint))
		{
			return Vector3D(1, 0, 0);
		}

		return (otherPoint - center).Normalized();
	}

	bool Sphere3::IntersectsLocal(const Ray3D& ray) const
	{
		Vector3D r = ray.origin - center;
		double b = ray.direction.Dot(r);
		double c = r.LengthSquared() - Square(radius);
		double d = b * b - c;

		if (d > 0.0)
		{
			d = std::sqrt(d);
			double min = -b - d;
			double max = -b + d;
			
			if (min < 0.0)
			{
				min = max;
			}

			if (min >= 0.0)
			{
				return true;
			}
		}

		return false;
	}

	SurfaceRayIntersection3 Sphere3::ClosestIntersectionLocal(const Ray3D& ray) const
	{
		SurfaceRayIntersection3 intersection;
		Vector3D r = ray.origin - center;
		double b = ray.direction.Dot(r);
		double c = r.LengthSquared() - Square(radius);
		double d = b * b - c;

		if (d > 0.0)
		{
			d = std::sqrt(d);
			double min = -b - d;
			double max = -b + d;

			if (min < 0.0)
			{
				min = max;
			}

			if (min < 0.0)
			{
				intersection.isIntersecting = false;
			}
			else
			{
				intersection.isIntersecting = true;
				intersection.distance = min;
				intersection.point = ray.origin + min * ray.direction;
				intersection.normal = (intersection.point - center).Normalized();
			}
		}
		else
		{
			intersection.isIntersecting = false;
		}

		return intersection;
	}

	BoundingBox3D Sphere3::BoundingBoxLocal() const
	{
		Vector3D r(radius, radius, radius);
		return BoundingBox3D(center - r, center + r);
	}

	Sphere3::Builder Sphere3::GetBuilder()
	{
		return Builder();
	}

	Sphere3::Builder& Sphere3::Builder::WithCenter(const Vector3D& center)
	{
		m_center = center;
		return *this;
	}

	Sphere3::Builder& Sphere3::Builder::WithRadius(double radius)
	{
		m_radius = radius;
		return *this;
	}

	Sphere3 Sphere3::Builder::Build() const
	{
		return Sphere3(m_center, m_radius, m_transform, m_isNormalFlipped);
	}

	Sphere3Ptr Sphere3::Builder::MakeShared() const
	{
		return std::shared_ptr<Sphere3>(new Sphere3(m_center, m_radius, m_transform, m_isNormalFlipped),
			[](Sphere3* obj)
		{
			delete obj;
		});
	}
}