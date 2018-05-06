/*************************************************************************
> File Name: PointsToImplicit3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 3-D points-to-implicit converters.
> Created Time: 2017/11/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_POINTS_TO_IMPLICIT3_H
#define CUBBYFLOW_POINTS_TO_IMPLICIT3_H

#include <Core/Array/ArrayAccessor1.h>
#include <Core/Grid/ScalarGrid3.h>
#include <Core/Vector/Vector3.h>

namespace CubbyFlow
{
	//! Abstract base class for 3-D points-to-implicit converters.
	class PointsToImplicit3
	{
	public:
		//! Default constructor.
		PointsToImplicit3();

		//! Default destructor.
		virtual ~PointsToImplicit3();

		//! Converts the given points to implicit surface scalar field.
		virtual void Convert(const ConstArrayAccessor1<Vector3D>& points, ScalarGrid3* output) const = 0;
	};

	//! Shared pointer for the PointsToImplicit3 type.
	using PointsToImplicit3Ptr = std::shared_ptr<PointsToImplicit3>;
}

#endif