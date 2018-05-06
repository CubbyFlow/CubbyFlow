/*************************************************************************
> File Name: GridPointGenerator3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D regular-grid point generator.
> Created Time: 2017/08/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/PointGenerator/GridPointGenerator3.h>

namespace CubbyFlow
{
	void GridPointGenerator3::ForEachPoint(
		const BoundingBox3D& boundingBox,
		double spacing,
		const std::function<bool(const Vector3D&)>& callback) const
	{
		Vector3D position;
		double boxWidth = boundingBox.GetWidth();
		double boxHeight = boundingBox.GetHeight();
		double boxDepth = boundingBox.GetDepth();
		bool shouldQuit = false;

		for (int k = 0; k * spacing <= boxDepth && !shouldQuit; ++k)
		{
			position.z = k * spacing + boundingBox.lowerCorner.z;

			for (int j = 0; j * spacing <= boxHeight && !shouldQuit; ++j)
			{
				position.y = j * spacing + boundingBox.lowerCorner.y;

				for (int i = 0; i * spacing <= boxWidth && !shouldQuit; ++i)
				{
					position.x = i * spacing + boundingBox.lowerCorner.x;
				
					if (!callback(position))
					{
						shouldQuit = true;
						break;
					}
				}
			}
		}
	}
}