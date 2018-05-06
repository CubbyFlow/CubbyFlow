/*************************************************************************
> File Name: ListQueryEngine3-Impl.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Ad-hoc list-based 3-D intersection/nearest-neighbor query engine.
> Created Time: 2017/10/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_LIST_QUERY_ENGINE3_IMPL_H
#define CUBBYFLOW_LIST_QUERY_ENGINE3_IMPL_H

namespace CubbyFlow
{
	template <typename T>
	void ListQueryEngine3<T>::Add(const T& item)
	{
		m_items.push_back(item);
	}

	template <typename T>
	void ListQueryEngine3<T>::Add(const std::vector<T>& items)
	{
		m_items.insert(m_items.end(), items.begin(), items.end());
	}

	template <typename T>
	bool ListQueryEngine3<T>::IsIntersects(
		const BoundingBox3D& box,
		const BoxIntersectionTestFunc3<T>& testFunc) const
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
	bool ListQueryEngine3<T>::IsIntersects(
		const Ray3D& ray, const RayIntersectionTestFunc3<T>& testFunc) const
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
	void ListQueryEngine3<T>::ForEachIntersectingItem(
		const BoundingBox3D& box, const BoxIntersectionTestFunc3<T>& testFunc,
		const IntersectionVisitorFunc3<T>& visitorFunc) const
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
	void ListQueryEngine3<T>::ForEachIntersectingItem(
		const Ray3D& ray, const RayIntersectionTestFunc3<T>& testFunc,
		const IntersectionVisitorFunc3<T>& visitorFunc) const
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
	ClosestIntersectionQueryResult3<T> ListQueryEngine3<T>::GetClosestIntersection(
		const Ray3D& ray, const GetRayIntersectionFunc3<T>& testFunc) const
	{
		ClosestIntersectionQueryResult3<T> best;
	
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
	NearestNeighborQueryResult3<T> ListQueryEngine3<T>::GetNearestNeighbor(
		const Vector3D& pt,
		const NearestNeighborDistanceFunc3<T>& distanceFunc) const
	{
		NearestNeighborQueryResult3<T> best;
		
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