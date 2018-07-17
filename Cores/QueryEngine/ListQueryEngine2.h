/*************************************************************************
> File Name: ListQueryEngine2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Ad-hoc list-based 2-D intersection/nearest-neighbor query engine.
> Created Time: 2017/10/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_LIST_QUERY_ENGINE2_H
#define CUBBYFLOW_LIST_QUERY_ENGINE2_H

#include <QueryEngine/IntersectionQueryEngine2.h>
#include <QueryEngine/NearestNeighborQueryEngine2.h>

namespace CubbyFlow
{
	//! Ad-hoc list-based 2-D intersection/nearest-neighbor query engine.
	template <typename T>
	class ListQueryEngine2 final : public IntersectionQueryEngine2<T>, public NearestNeighborQueryEngine2<T>
	{
	public:
		//! Adds an item to the container.
		void Add(const T& item);

		//! Adds items to the container.
		void Add(const std::vector<T>& items);

		//! Returns true if given \p box intersects with any of the stored items.
		bool IsIntersects(const BoundingBox2D& box,
			const BoxIntersectionTestFunc2<T>& testFunc) const override;

		//! Returns true if given \p ray intersects with any of the stored items.
		bool IsIntersects(const Ray2D& ray,
			const RayIntersectionTestFunc2<T>& testFunc) const override;

		//! Invokes \p visitorFunc for every intersecting items.
		void ForEachIntersectingItem(
			const BoundingBox2D& box, const BoxIntersectionTestFunc2<T>& testFunc,
			const IntersectionVisitorFunc2<T>& visitorFunc) const override;

		//! Invokes \p visitorFunc for every intersecting items.
		void ForEachIntersectingItem(
			const Ray2D& ray, const RayIntersectionTestFunc2<T>& testFunc,
			const IntersectionVisitorFunc2<T>& visitorFunc) const override;

		//! Returns the closest intersection for given \p ray.
		ClosestIntersectionQueryResult2<T> GetClosestIntersection(
			const Ray2D& ray,
			const GetRayIntersectionFunc2<T>& testFunc) const override;

		//! Returns the nearest neighbor for given point and distance measure function.
		NearestNeighborQueryResult2<T> GetNearestNeighbor(
			const Vector2D& pt,
			const NearestNeighborDistanceFunc2<T>& distanceFunc) const override;

	private:
		std::vector<T> m_items;
	};
}

#include <QueryEngine/ListQueryEngine2-Impl.h>

#endif