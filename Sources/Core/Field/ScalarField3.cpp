// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Field/ScalarField3.hpp>

namespace CubbyFlow
{
Vector3D ScalarField3::Gradient(const Vector3D&) const
{
    return Vector3D{ 0, 0, 0 };
}

double ScalarField3::Laplacian(const Vector3D&) const
{
    return 0.0;
}

std::function<double(const Vector3D&)> ScalarField3::Sampler() const
{
    const ScalarField3* self = this;
    return [self](const Vector3D& x) -> double { return self->Sample(x); };
}
}  // namespace CubbyFlow