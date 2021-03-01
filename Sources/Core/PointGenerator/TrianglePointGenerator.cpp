// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/PointGenerator/TrianglePointGenerator.hpp>

namespace CubbyFlow
{
void TrianglePointGenerator::ForEachPoint(
    const BoundingBox2D& boundingBox, double spacing,
    const std::function<bool(const Vector2D&)>& callback) const
{
    const double halfSpacing = spacing / 2.0;
    const double ySpacing = spacing * std::sqrt(3.0) / 2.0;
    const double boxWidth = boundingBox.Width();
    const double boxHeight = boundingBox.Height();

    Vector2D position;
    bool hasOffset = false;
    bool shouldQuit = false;
    for (int j = 0; j * ySpacing <= boxHeight && !shouldQuit; ++j)
    {
        position.y = j * ySpacing + boundingBox.lowerCorner.y;

        const double offset = (hasOffset) ? halfSpacing : 0.0;

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

}  // namespace CubbyFlow