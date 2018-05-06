/*************************************************************************
> File Name: PointGenerator2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 2-D point generator.
> Created Time: 2017/06/02
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/PointGenerator/PointGenerator2.h>

namespace CubbyFlow
{
	PointGenerator2::PointGenerator2()
	{
		// Do nothing
	}

	PointGenerator2::~PointGenerator2()
	{
		// Do nothing
	}

	void PointGenerator2::Generate(const BoundingBox2D& boundingBox, double spacing, Array1<Vector2D>* points) const
	{
		ForEachPoint(boundingBox, spacing, [&points](const Vector2D& point)
		{
			points->Append(point);
			return true;
		});
	}
}