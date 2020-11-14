// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Field/VectorField3.hpp>

namespace CubbyFlow
{
double VectorField3::Divergence(const Vector3D&) const
{
    return 0.0;
}

Vector3D VectorField3::Curl(const Vector3D&) const
{
    return Vector3D{ 0, 0, 0 };
}

std::function<Vector3D(const Vector3D&)> VectorField3::Sampler() const
{
    const VectorField3* self = this;
    return [self](const Vector3D& x) -> Vector3D { return self->Sample(x); };
}
}  // namespace CubbyFlow