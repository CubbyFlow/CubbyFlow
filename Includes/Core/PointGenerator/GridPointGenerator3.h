/*************************************************************************
> File Name: GridPointGenerator3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D regular-grid point generator.
> Created Time: 2017/08/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_POINT_GENERATOR3_H
#define CUBBYFLOW_GRID_POINT_GENERATOR3_H

#include <Core/PointGenerator/PointGenerator3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D regular-grid point generator.
	//!
	class GridPointGenerator3 final : public PointGenerator3
	{
	public:
		//!
		//! \brief Invokes \p callback function for each regular grid points inside
		//! \p boundingBox.
		//!
		//! This function iterates every regular grid points inside \p boundingBox
		//! where \p spacing is the size of the unit cell of regular grid structure.
		//!
		void ForEachPoint(
			const BoundingBox3D& boundingBox,
			double spacing,
			const std::function<bool(const Vector3D&)>& callback) const;
	};

	//! Shared pointer type for the GridPointGenerator3.
	using GridPointGenerator3Ptr = std::shared_ptr<GridPointGenerator3>;
}

#endif