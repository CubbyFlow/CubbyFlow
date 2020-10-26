// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/PointGenerator/PointGenerator3.hpp>

namespace CubbyFlow
{
void PointGenerator3::Generate(const BoundingBox3D& boundingBox, double spacing,
                               Array1<Vector3D>* points) const
{
    ForEachPoint(boundingBox, spacing, [&points](const Vector3D& point) {
        points->Append(point);
        return true;
    });
}
}  // namespace CubbyFlow