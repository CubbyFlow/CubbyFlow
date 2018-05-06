/*************************************************************************
> File Name: PointGenerator3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 3-D point generator.
> Created Time: 2017/06/02
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/PointGenerator/PointGenerator3.h>

namespace CubbyFlow
{
	PointGenerator3::PointGenerator3()
	{
		// Do nothing
	}

	PointGenerator3::~PointGenerator3()
	{
		// Do nothing
	}

	void PointGenerator3::Generate(const BoundingBox3D& boundingBox, double spacing, Array1<Vector3D>* points) const
	{
		ForEachPoint(boundingBox, spacing, [&points](const Vector3D& point)
		{
			points->Append(point);
			return true;
		});
	}
}