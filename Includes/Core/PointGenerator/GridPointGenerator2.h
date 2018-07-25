/*************************************************************************
> File Name: GridPointGenerator2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D regular-grid point generator.
> Created Time: 2017/08/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_POINT_GENERATOR2_H
#define CUBBYFLOW_GRID_POINT_GENERATOR2_H

#include <Core/PointGenerator/PointGenerator2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D regular-grid point generator.
	//!
	class GridPointGenerator2 final : public PointGenerator2
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
			const BoundingBox2D& boundingBox,
			double spacing,
			const std::function<bool(const Vector2D&)>& callback) const;
	};

	//! Shared pointer type for the GridPointGenerator2.
	using GridPointGenerator2Ptr = std::shared_ptr<GridPointGenerator2>;
}

#endif