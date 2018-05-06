/*************************************************************************
> File Name: TrianglePointGenerator.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Right triangle point generator.
> Created Time: 2017/06/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/PointGenerator/TrianglePointGenerator.h>

namespace CubbyFlow
{
	void TrianglePointGenerator::ForEachPoint(const BoundingBox2D& boundingBox, double spacing,
		const std::function<bool(const Vector2D&)>& callback) const
	{
		const double halfSpacing = spacing / 2.0;
		const double ySpacing = spacing * std::sqrt(3.0) / 2.0;
		double boxWidth = boundingBox.GetWidth();
		double boxHeight = boundingBox.GetHeight();

		Vector2D position;
		bool hasOffset = false;
		bool shouldQuit = false;
		for (int j = 0; j * ySpacing <= boxHeight && !shouldQuit; ++j)
		{
			position.y = j * ySpacing + boundingBox.lowerCorner.y;

			double offset = (hasOffset) ? halfSpacing : 0.0;

			for (int i = 0; i * spacing + offset <= boxWidth && !shouldQuit; ++i)
			{
				position.x = i * spacing + offset + boundingBox.lowerCorner.x;
			
				if (!callback(position))
				{
					shouldQuit = true;
					break;
				}
			}

			hasOffset = !hasOffset;
		}
	}
	
}	