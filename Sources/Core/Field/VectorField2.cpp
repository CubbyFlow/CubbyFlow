// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Field/VectorField2.hpp>

namespace CubbyFlow
{
double VectorField2::Divergence(const Vector2D&) const
{
    return 0.0;
}

double VectorField2::Curl(const Vector2D&) const
{
    return 0.0;
}

std::function<Vector2D(const Vector2D&)> VectorField2::Sampler() const
{
    const VectorField2* self = this;
    return [self](const Vector2D& x) -> Vector2D { return self->Sample(x); };
}
}  // namespace CubbyFlow