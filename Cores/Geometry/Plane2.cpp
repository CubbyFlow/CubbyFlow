/*************************************************************************
> File Name: Plane2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D plane geometry.
> Created Time: 2017/06/18
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Geometry/Plane2.h>

namespace CubbyFlow
{
	Plane2::Plane2(const Transform2& transform_, bool isNormalFlipped_) :
		Surface2(transform_, isNormalFlipped_)
	{
		// Do nothing
	}

	Plane2::Plane2(const Vector2D& normal_, const Vector2D& point_,
		const Transform2& transform_, bool isNormalFlipped_) :
		Surface2(transform_, isNormalFlipped_), normal(normal_), point(point_)
	{
		// Do nothing
	}

	Plane2::Plane2(const Plane2& other) :
		Surface2(other), normal(other.normal), point(other.point)
	{
		// Do nothing
	}

	Vector2D Plane2::ClosestPointLocal(const Vector2D& otherPoint) const
	{
		Vector2D r = otherPoint - point;
		return r - normal.Dot(r) * normal + point;
	}

	Vector2D Plane2::ClosestNormalLocal(const Vector2D& otherPoint) const
	{
		UNUSED_VARIABLE(otherPoint);

		return normal;
	}

	bool Plane2::IntersectsLocal(const Ray2D& ray) const
	{
		return std::fabs(ray.direction.Dot(normal)) > 0;
	}

	SurfaceRayIntersection2 Plane2::ClosestIntersectionLocal(const Ray2D& ray) const
	{
		SurfaceRayIntersection2 intersection;
		double dDotN = ray.direction.Dot(normal);

		// Check if not parallel
		if (std::fabs(dDotN) > 0)
		{
			double t = normal.Dot(point - ray.origin) / dDotN;
			if (t >= 0.0)
			{
				intersection.isIntersecting = true;
				intersection.distance = t;
				intersection.point = ray.PointAt(t);
				intersection.normal = normal;
			}
		}

		return intersection;
	}

	BoundingBox2D Plane2::BoundingBoxLocal() const
	{
		static const double eps = std::numeric_limits<double>::epsilon();
		static const double dmax = std::numeric_limits<double>::max();

		if (std::fabs(normal.Dot(Vector2D(1, 0)) - 1.0) < eps)
		{
			return BoundingBox2D(point - Vector2D(0, dmax), point + Vector2D(0, dmax));
		}
		
		if (std::fabs(normal.Dot(Vector2D(0, 1)) - 1.0) < eps)
		{
			return BoundingBox2D(point - Vector2D(dmax, 0), point + Vector2D(dmax, 0));
		}
		
		return BoundingBox2D(Vector2D(dmax, dmax), Vector2D(dmax, dmax));
	}

	Plane2::Builder Plane2::GetBuilder()
	{
		return Builder();
	}

	Plane2::Builder& Plane2::Builder::WithNormal(const Vector2D& normal)
	{
		m_normal = normal;
		return *this;
	}

	Plane2::Builder& Plane2::Builder::WithPoint(const Vector2D& point)
	{
		m_point = point;
		return *this;
	}

	Plane2 Plane2::Builder::Build() const
	{
		return Plane2(m_normal, m_point, m_transform, m_isNormalFlipped);
	}

	Plane2Ptr Plane2::Builder::MakeShared() const
	{
		return std::shared_ptr<Plane2>(new Plane2(m_normal, m_point, m_transform, m_isNormalFlipped),
			[](Plane2* obj)
		{
			delete obj;
		});
	}
}