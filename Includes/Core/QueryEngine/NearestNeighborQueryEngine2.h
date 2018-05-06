/*************************************************************************
> File Name: NearestNeighborQueryEngine2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 2-D nearest neigbor query engine.
> Created Time: 2017/10/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_NEAREST_NEIGHBOR_QUERY_ENGINE2_H
#define CUBBYFLOW_NEAREST_NEIGHBOR_QUERY_ENGINE2_H

#include <Core/Vector/Vector2.h>

namespace CubbyFlow
{
	//! Nearest neighbor query result.
	template <typename T>
	struct NearestNeighborQueryResult2
	{
		const T* item = nullptr;
		double distance = std::numeric_limits<double>::max();
	};

	//! Nearest neighbor distance measure function.
	template <typename T>
	using NearestNeighborDistanceFunc2 = std::function<double(const T&, const Vector2D&)>;

	//! Abstract base class for 2-D nearest neighbor query engine.
	template <typename T>
	class NearestNeighborQueryEngine2
	{
	public:
		//! Returns the nearest neighbor for given point and distance measure function.
		virtual NearestNeighborQueryResult2<T> GetNearestNeighbor(
			const Vector2D& pt,
			const NearestNeighborDistanceFunc2<T>& distanceFunc) const = 0;
	};
}

#endif