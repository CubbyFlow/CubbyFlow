/*************************************************************************
> File Name: BccLatticPointGenerator.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Body-centered lattice points generator.
> Created Time: 2017/06/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <PointGenerator/BccLatticePointGenerator.h>

namespace CubbyFlow
{
	void BccLatticePointGenerator::ForEachPoint(const BoundingBox3D& boundingBox, double spacing,
		const std::function<bool(const Vector3D&)>& callback) const
	{
		double halfSpacing = spacing / 2.0;
		double boxWidth = boundingBox.GetWidth();
		double boxHeight = boundingBox.GetHeight();
		double boxDepth = boundingBox.GetDepth();

		Vector3D position;
		bool hasOffset = false;
		bool shouldQuit = false;

		for (int k = 0; k * halfSpacing <= boxDepth && !shouldQuit; ++k)
		{
			position.z = k * halfSpacing + boundingBox.lowerCorner.z;

			double offset = (hasOffset) ? halfSpacing : 0.0;

			for (int j = 0; j * spacing + offset <= boxHeight && !shouldQuit; ++j)
			{
				position.y = j * spacing + offset + boundingBox.lowerCorner.y;

				for (int i = 0; i * spacing + offset <= boxWidth; ++i)
				{
					position.x = i * spacing + offset + boundingBox.lowerCorner.x;
					
					if (!callback(position))
					{
						shouldQuit = true;
						break;
					}
				}
			}

			hasOffset = !hasOffset;
		}
	}
}