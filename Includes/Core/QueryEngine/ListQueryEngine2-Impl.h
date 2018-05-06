/*************************************************************************
> File Name: ListQueryEngine2-Impl.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Ad-hoc list-based 2-D intersection/nearest-neighbor query engine.
> Created Time: 2017/10/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_LIST_QUERY_ENGINE2_IMPL_H
#define CUBBYFLOW_LIST_QUERY_ENGINE2_IMPL_H

namespace CubbyFlow
{
	template <typename T>
	void ListQueryEngine2<T>::Add(const T& item)
	{
		m_items.push_back(item);
	}

	template <typename T>
	void ListQueryEngine2<T>::Add(const std::vector<T>& items)
	{
		m_items.insert(m_items.end(), items.begin(), items.end());
	}

	template <typename T>
	bool ListQueryEngine2<T>::IsIntersects(
		const BoundingBox2D& box,
		const BoxIntersectionTestFunc2<T>& testFunc) const
	{
		for (const auto& item : m_items)
		{
			if (testFunc(item, box))
			{
				return true;
			}
		}

		return false;
	}

	template <typename T>
	bool ListQueryEngine2<T>::IsIntersects(
		const Ray2D& ray, const RayIntersectionTestFunc2<T>& testFunc) const
	{
		for (const auto& item : m_items)
		{
			if (testFunc(item, ray))
			{
				return true;
			}
		}

		return false;
	}

	template <typename T>
	void ListQueryEngine2<T>::ForEachIntersectingItem(
		const BoundingBox2D& box, const BoxIntersectionTestFunc2<T>& testFunc,
		const IntersectionVisitorFunc2<T>& visitorFunc) const
	{
		for (const auto& item : m_items)
		{
			if (testFunc(item, box))
			{
				visitorFunc(item);
			}
		}
	}

	template <typename T>
	void ListQueryEngine2<T>::ForEachIntersectingItem(
		const Ray2D& ray, const RayIntersectionTestFunc2<T>& testFunc,
		const IntersectionVisitorFunc2<T>& visitorFunc) const
	{
		for (const auto& item : m_items)
		{
			if (testFunc(item, ray))
			{
				visitorFunc(item);
			}
		}
	}

	template <typename T>
	ClosestIntersectionQueryResult2<T> ListQueryEngine2<T>::GetClosestIntersection(
		const Ray2D& ray, const GetRayIntersectionFunc2<T>& testFunc) const
	{
		ClosestIntersectionQueryResult2<T> best;

		for (const auto& item : m_items)
		{
			double dist = testFunc(item, ray);
			if (dist < best.distance)
			{
				best.distance = dist;
				best.item = &item;
			}
		}

		return best;
	}

	template <typename T>
	NearestNeighborQueryResult2<T> ListQueryEngine2<T>::GetNearestNeighbor(
		const Vector2D& pt,
		const NearestNeighborDistanceFunc2<T>& distanceFunc) const
	{
		NearestNeighborQueryResult2<T> best;
	
		for (const auto& item : m_items)
		{
			double dist = distanceFunc(item, pt);
			if (dist < best.distance)
			{
				best.item = &item;
				best.distance = dist;
			}
		}

		return best;
	}
}

#endif