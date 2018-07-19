/*************************************************************************
> File Name: IntersectionQueryEngine2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D intersection test query engine.
> Created Time: 2017/10/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_INTERSECTION_QUERY_ENGINE2_H
#define CUBBYFLOW_INTERSECTION_QUERY_ENGINE2_H

#include <BoundingBox/BoundingBox2.h>
#include <Vector/Vector2.h>

namespace CubbyFlow
{
	//! Closest intersection query result.
	template <typename T>
	struct ClosestIntersectionQueryResult2
	{
		const T* item = nullptr;
		double distance = std::numeric_limits<double>::max();
	};

	//! Closest intersection distance measure function.
	template <typename T>
	using ClosestIntersectionDistanceFunc2 = std::function<double(const T&, const Vector2D&)>;

	//! Box-item intersection test function.
	template <typename T>
	using BoxIntersectionTestFunc2 = std::function<bool(const T&, const BoundingBox2D&)>;

	//! Ray-item intersection test function.
	template <typename T>
	using RayIntersectionTestFunc2 = std::function<bool(const T&, const Ray2D&)>;

	//! Ray-item closest intersection evaluation function.
	template <typename T>
	using GetRayIntersectionFunc2 = std::function<double(const T&, const Ray2D&)>;

	//! Visitor function which is invoked for each intersecting item.
	template <typename T>
	using IntersectionVisitorFunc2 = std::function<void(const T&)>;

	//! Abstract base class for 2-D intersection test query engine.
	template <typename T>
	class IntersectionQueryEngine2
	{
	public:
		//! Returns true if given \p box intersects with any of the stored items.
		virtual bool IsIntersects(
			const BoundingBox2D& box,
			const BoxIntersectionTestFunc2<T>& testFunc) const = 0;

		//! Returns true if given \p ray intersects with any of the stored items.
		virtual bool IsIntersects(
			const Ray2D& ray,
			const RayIntersectionTestFunc2<T>& testFunc) const = 0;

		//! Invokes \p visitorFunc for every intersecting items.
		virtual void ForEachIntersectingItem(
			const BoundingBox2D& box, const BoxIntersectionTestFunc2<T>& testFunc,
			const IntersectionVisitorFunc2<T>& visitorFunc) const = 0;

		//! Invokes \p visitorFunc for every intersecting items.
		virtual void ForEachIntersectingItem(
			const Ray2D& ray, const RayIntersectionTestFunc2<T>& testFunc,
			const IntersectionVisitorFunc2<T>& visitorFunc) const = 0;

		//! Returns the closest intersection for given \p ray.
		virtual ClosestIntersectionQueryResult2<T> GetClosestIntersection(
			const Ray2D& ray, const GetRayIntersectionFunc2<T>& testFunc) const = 0;
	};
}

#endif