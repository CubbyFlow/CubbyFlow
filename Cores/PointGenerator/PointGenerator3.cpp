/*************************************************************************
> File Name: PointGenerator3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D point generator.
> Created Time: 2017/06/02
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <PointGenerator/PointGenerator3.h>

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