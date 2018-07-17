/*************************************************************************
> File Name: Surface3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D surface.
> Created Time: 2017/03/20
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SURFACE3_H
#define CUBBYFLOW_SURFACE3_H

#include <BoundingBox/BoundingBox3.h>
#include <Ray/Ray3.h>
#include <Transform/Transform3.h>
#include <Vector/Vector3.h>

#include <limits>
#include <memory>

namespace CubbyFlow
{
	//! Structure that represents ray-surface intersection point.
	struct SurfaceRayIntersection3
	{
		bool isIntersecting = false;
		double distance = std::numeric_limits<double>::max();
		Vector3D point;
		Vector3D normal;
	};

	//! Abstract base class for 3-D surface.
	class Surface3
	{
	public:
		//! Local-to-world transform.
		Transform3 transform;

		//! Flips normal when calling Surface3::closestNormal(...).
		bool isNormalFlipped = false;

		//! Constructs a surface with normal direction.
		Surface3(
			const Transform3& transform = Transform3(),
			bool isNormalFlipped = false);

		//! Copy constructor.
		Surface3(const Surface3& other);

		//! Default destructor.
		virtual ~Surface3();

		//! Returns the closest point from the given point \p otherPoint to the surface.
		Vector3D ClosestPoint(const Vector3D& otherPoint) const;

		//! Returns the bounding box of this surface object.
		BoundingBox3D BoundingBox() const;

		//! Returns true if the given \p ray intersects with this surface object.
		bool Intersects(const Ray3D& ray) const;

		//! Returns the closest distance from the given point \p otherPoint to the
		//! point on the surface.
		double ClosestDistance(const Vector3D& otherPoint) const;

		//! Returns the closest intersection point for given \p ray.
		SurfaceRayIntersection3 ClosestIntersection(const Ray3D& ray) const;

		//! Returns the normal to the closest point on the surface from the given
		//! point \p otherPoint.
		Vector3D ClosestNormal(const Vector3D& otherPoint) const;

		//! Updates internal spatial query engine.
		virtual void UpdateQueryEngine();

	protected:
		//! Returns the closest point from the given point \p otherPoint to the
		//! surface in local frame.
		virtual Vector3D ClosestPointLocal(const Vector3D& otherPoint) const = 0;

		//! Returns the bounding box of this surface object in local frame.
		virtual BoundingBox3D BoundingBoxLocal() const = 0;

		//! Returns the closest intersection point for given \p ray in local frame.
		virtual SurfaceRayIntersection3 ClosestIntersectionLocal(const Ray3D& ray) const = 0;

		//! Returns the normal to the closest point on the surface from the given
		//! point \p otherPoint in local frame.
		virtual Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const = 0;

		//! Returns true if the given \p ray intersects with this surface object
		//! in local frame.
		virtual bool IntersectsLocal(const Ray3D& ray) const;

		//! Returns the closest distance from the given point \p otherPoint to the
		//! point on the surface in local frame.
		virtual double ClosestDistanceLocal(const Vector3D& otherPoint) const;
	};

	//! Shared pointer for the Surface3 type.
	using Surface3Ptr = std::shared_ptr<Surface3>;

	//!
	//! \brief Base class for 3-D surface builder.
	//!
	template <typename DerivedBuilder>
	class SurfaceBuilderBase3
	{
	public:
		//! Returns builder with flipped normal flag.
		DerivedBuilder& WithIsNormalFlipped(bool isNormalFlipped);

		//! Returns builder with translation.
		DerivedBuilder& WithTranslation(const Vector3D& translation);

		//! Returns builder with orientation.
		DerivedBuilder& WithOrientation(const QuaternionD& orientation);

		//! Returns builder with transform.
		DerivedBuilder& WithTransform(const Transform3& transform);

	protected:
		bool m_isNormalFlipped = false;
		Transform3 m_transform;
	};

	template <typename T>
	T& SurfaceBuilderBase3<T>::WithIsNormalFlipped(bool isNormalFlipped)
	{
		m_isNormalFlipped = isNormalFlipped;
		return static_cast<T&>(*this);
	}

	template <typename T>
	T& SurfaceBuilderBase3<T>::WithTranslation(const Vector3D& translation)
	{
		m_transform.SetTranslation(translation);
		return static_cast<T&>(*this);
	}

	template <typename T>
	T& SurfaceBuilderBase3<T>::WithOrientation(const QuaternionD& orientation)
	{
		m_transform.SetOrientation(orientation);
		return static_cast<T&>(*this);
	}

	template <typename T>
	T& SurfaceBuilderBase3<T>::WithTransform(const Transform3& transform)
	{
		m_transform = transform;
		return static_cast<T&>(*this);
	}
}

#endif