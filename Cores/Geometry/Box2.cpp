/*************************************************************************
> File Name: Box2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D box geometry.
> Created Time: 2017/06/23
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Geometry/Box2.h>
#include <Geometry/Plane2.h>

namespace CubbyFlow
{
	Box2::Box2(const Transform2& transform, bool isNormalFlipped) : 
		Surface2(transform, isNormalFlipped)
	{
		// Do nothing
	}

	Box2::Box2(const Vector2D& lowerCorner, const Vector2D& upperCorner,
		const Transform2& transform, bool isNormalFlipped) :
		Box2(BoundingBox2D(lowerCorner, upperCorner), transform, isNormalFlipped)
	{
		// Do nothing
	}

	Box2::Box2(const BoundingBox2D& boundingBox, const Transform2& transform, bool isNormalFlipped) :
	Surface2(transform, isNormalFlipped), bound(boundingBox)
	{
		// Do nothing
	}

	Box2::Box2(const Box2& other) :
		Surface2(other), bound(other.bound)
	{
		// Do nothing
	}

	Vector2D Box2::ClosestPointLocal(const Vector2D& otherPoint) const
	{
		if (bound.Contains(otherPoint))
		{
			Plane2 planes[4] =
			{
				Plane2(Vector2D(1, 0), bound.upperCorner),
				Plane2(Vector2D(0, 1), bound.upperCorner),
				Plane2(Vector2D(-1, 0), bound.lowerCorner),
				Plane2(Vector2D(0, -1), bound.lowerCorner)
			};

			Vector2D result = planes[0].ClosestPoint(otherPoint);
			double distanceSquared = result.DistanceSquaredTo(otherPoint);

			for (int i = 1; i < 4; ++i)
			{
				Vector2D localResult = planes[i].ClosestPoint(otherPoint);
				double localDistanceSquared = localResult.DistanceSquaredTo(otherPoint);

				if (localDistanceSquared < distanceSquared)
				{
					result = localResult;
					distanceSquared = localDistanceSquared;
				}
			}

			return result;
		}
		
		return Clamp(otherPoint, bound.lowerCorner, bound.upperCorner);
	}

	Vector2D Box2::ClosestNormalLocal(const Vector2D& otherPoint) const
	{
		Plane2 planes[4] =
		{
			Plane2(Vector2D(1, 0), bound.upperCorner),
			Plane2(Vector2D(0, 1), bound.upperCorner),
			Plane2(Vector2D(-1, 0), bound.lowerCorner),
			Plane2(Vector2D(0, -1), bound.lowerCorner)
		};

		if (bound.Contains(otherPoint))
		{
			Vector2D closestNormal = planes[0].normal;
			Vector2D closestPoint = planes[0].ClosestPoint(otherPoint);
			double minDistanceSquared = (closestPoint - otherPoint).LengthSquared();

			for (int i = 1; i < 4; ++i)
			{
				Vector2D localClosestPoint = planes[i].ClosestPoint(otherPoint);
				double localDistanceSquared = (localClosestPoint - otherPoint).LengthSquared();

				if (localDistanceSquared < minDistanceSquared)
				{
					closestNormal = planes[i].normal;
					minDistanceSquared = localDistanceSquared;
				}
			}

			return closestNormal;
		}

		Vector2D closestPoint = Clamp(otherPoint, bound.lowerCorner, bound.upperCorner);
		Vector2D closestPointToInputPoint = otherPoint - closestPoint;
		Vector2D closestNormal = planes[0].normal;
		double maxCosineAngle = closestNormal.Dot(closestPointToInputPoint);

		for (int i = 1; i < 4; ++i)
		{
			double cosineAngle = planes[i].normal.Dot(closestPointToInputPoint);

			if (cosineAngle > maxCosineAngle)
			{
				closestNormal = planes[i].normal;
				maxCosineAngle = cosineAngle;
			}
		}

		return closestNormal;
	}

	bool Box2::IntersectsLocal(const Ray2D& ray) const
	{
		return bound.Intersects(ray);
	}

	SurfaceRayIntersection2 Box2::ClosestIntersectionLocal(const Ray2D& ray) const
	{
		SurfaceRayIntersection2 intersection;
		BoundingBoxRayIntersection2D bbRayIntersection = bound.ClosestIntersection(ray);
		intersection.isIntersecting = bbRayIntersection.isIntersecting;

		if (intersection.isIntersecting)
		{
			intersection.distance = bbRayIntersection.near;
			intersection.point = ray.PointAt(bbRayIntersection.near);
			intersection.normal = Box2::ClosestNormal(intersection.point);
		}

		return intersection;
	}

	BoundingBox2D Box2::BoundingBoxLocal() const
	{
		return bound;
	}

	Box2::Builder Box2::GetBuilder()
	{
		return Builder();
	}

	Box2::Builder& Box2::Builder::WithLowerCorner(const Vector2D& pt)
	{
		m_lowerCorner = pt;
		return *this;
	}

	Box2::Builder& Box2::Builder::WithUpperCorner(const Vector2D& pt)
	{
		m_upperCorner = pt;
		return *this;
	}

	Box2::Builder& Box2::Builder::WithBoundingBox(const BoundingBox2D& bbox)
	{
		m_lowerCorner = bbox.lowerCorner;
		m_upperCorner = bbox.upperCorner;
		return *this;
	}

	Box2 Box2::Builder::Build() const
	{
		return Box2(m_lowerCorner, m_upperCorner, m_transform, m_isNormalFlipped);
	}

	Box2Ptr Box2::Builder::MakeShared() const
	{
		return std::shared_ptr<Box2>(
			new Box2(m_lowerCorner, m_upperCorner, m_transform, m_isNormalFlipped),
			[](Box2* obj)
		{
			delete obj;
		});
	}
}