/*************************************************************************
> File Name: NearestNeighborQueryEngine3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D nearest neigbor query engine.
> Created Time: 2017/10/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_NEAREST_NEIGHBOR_QUERY_ENGINE3_H
#define CUBBYFLOW_NEAREST_NEIGHBOR_QUERY_ENGINE3_H

#include <Core/Vector/Vector3.h>

namespace CubbyFlow
{
	//! Nearest neighbor query result.
	template <typename T>
	struct NearestNeighborQueryResult3
	{
		const T* item = nullptr;
		double distance = std::numeric_limits<double>::max();
	};

	//! Nearest neighbor distance measure function.
	template <typename T>
	using NearestNeighborDistanceFunc3 = std::function<double(const T&, const Vector3D&)>;

	//! Abstract base class for 3-D nearest neighbor query engine.
	template <typename T>
	class NearestNeighborQueryEngine3
	{
	public:
		//! Returns the nearest neighbor for given point and distance measure function.
		virtual NearestNeighborQueryResult3<T> GetNearestNeighbor(
			const Vector3D& pt,
			const NearestNeighborDistanceFunc3<T>& distanceFunc) const = 0;
	};
}

#endif