// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/PointGenerator/GridPointGenerator2.hpp>

namespace CubbyFlow
{
void GridPointGenerator2::ForEachPoint(
    const BoundingBox2D& boundingBox, double spacing,
    const std::function<bool(const Vector2D&)>& callback) const
{
    Vector2D position;
    const double boxWidth = boundingBox.Width();
    const double boxHeight = boundingBox.Height();
    bool shouldQuit = false;

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
}  // namespace CubbyFlow