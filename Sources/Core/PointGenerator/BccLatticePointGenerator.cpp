// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/PointGenerator/BccLatticePointGenerator.hpp>

namespace CubbyFlow
{
void BccLatticePointGenerator::ForEachPoint(
    const BoundingBox3D& boundingBox, double spacing,
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
}  // namespace CubbyFlow