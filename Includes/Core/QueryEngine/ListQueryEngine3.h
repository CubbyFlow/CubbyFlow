/*************************************************************************
> File Name: ListQueryEngine3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Ad-hoc list-based 3-D intersection/nearest-neighbor query engine.
> Created Time: 2017/10/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_LIST_QUERY_ENGINE3_H
#define CUBBYFLOW_LIST_QUERY_ENGINE3_H

#include <Core/QueryEngine/IntersectionQueryEngine3.h>
#include <Core/QueryEngine/NearestNeighborQueryEngine3.h>

namespace CubbyFlow
{
	//! Ad-hoc list-based 3-D intersection/nearest-neighbor query engine.
	template <typename T>
	class ListQueryEngine3 final : public IntersectionQueryEngine3<T>, public NearestNeighborQueryEngine3<T>
	{
	public:
		//! Adds an item to the container.
		void Add(const T& item);

		//! Adds items to the container.
		void Add(const std::vector<T>& items);

		//! Returns true if given \p box intersects with any of the stored items.
		bool IsIntersects(const BoundingBox3D& box,
			const BoxIntersectionTestFunc3<T>& testFunc) const override;

		//! Returns true if given \p ray intersects with any of the stored items.
		bool IsIntersects(const Ray3D& ray,
			const RayIntersectionTestFunc3<T>& testFunc) const override;

		//! Invokes \p visitorFunc for every intersecting items.
		void ForEachIntersectingItem(
			const BoundingBox3D& box, const BoxIntersectionTestFunc3<T>& testFunc,
			const IntersectionVisitorFunc3<T>& visitorFunc) const override;

		//! Invokes \p visitorFunc for every intersecting items.
		void ForEachIntersectingItem(
			const Ray3D& ray, const RayIntersectionTestFunc3<T>& testFunc,
			const IntersectionVisitorFunc3<T>& visitorFunc) const override;

		//! Returns the closest intersection for given \p ray.
		ClosestIntersectionQueryResult3<T> GetClosestIntersection(
			const Ray3D& ray,
			const GetRayIntersectionFunc3<T>& testFunc) const override;

		//! Returns the nearest neighbor for given point and distance measure function.
		NearestNeighborQueryResult3<T> GetNearestNeighbor(
			const Vector3D& pt,
			const NearestNeighborDistanceFunc3<T>& distanceFunc) const override;

	private:
		std::vector<T> m_items;
	};
}

#include <Core/QueryEngine/ListQueryEngine3-Impl.h>

#endif