/*************************************************************************
> File Name: PointsToImplicit2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D points-to-implicit converters.
> Created Time: 2017/11/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_POINTS_TO_IMPLICIT2_H
#define CUBBYFLOW_POINTS_TO_IMPLICIT2_H

#include <Array/ArrayAccessor1.h>
#include <Grid/ScalarGrid2.h>
#include <Vector/Vector2.h>

namespace CubbyFlow
{
	//! Abstract base class for 2-D points-to-implicit converters.
	class PointsToImplicit2
	{
	public:
		//! Default constructor.
		PointsToImplicit2();

		//! Default destructor.
		virtual ~PointsToImplicit2();

		//! Converts the given points to implicit surface scalar field.
		virtual void Convert(const ConstArrayAccessor1<Vector2D>& points, ScalarGrid2* output) const = 0;
	};

	//! Shared pointer for the PointsToImplicit2 type.
	using PointsToImplicit2Ptr = std::shared_ptr<PointsToImplicit2>;
}

#endif