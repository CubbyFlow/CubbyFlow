// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/Advection/CubicSemiLagrangian2.hpp>

namespace CubbyFlow
{
std::function<double(const Vector2D&)>
CubicSemiLagrangian2::GetScalarSamplerFunc(const ScalarGrid2& source) const
{
    const auto sourceSampler = MonotonicCatmullRomArraySampler2<double>{
        source.DataView(), source.GridSpacing(), source.DataOrigin()
    };
    return sourceSampler.Functor();
}

std::function<Vector2D(const Vector2D&)>
CubicSemiLagrangian2::GetVectorSamplerFunc(
    const CollocatedVectorGrid2& source) const
{
    const auto sourceSampler = MonotonicCatmullRomArraySampler2<Vector2D>{
        source.DataView(), source.GridSpacing(), source.DataOrigin()
    };
    return sourceSampler.Functor();
}

std::function<Vector2D(const Vector2D&)>
CubicSemiLagrangian2::GetVectorSamplerFunc(
    const FaceCenteredGrid2& source) const
{
    auto uSourceSampler = MonotonicCatmullRomArraySampler2<double>(
        source.UView(), source.GridSpacing(), source.UOrigin());
    auto vSourceSampler = MonotonicCatmullRomArraySampler2<double>(
        source.VView(), source.GridSpacing(), source.VOrigin());
    return [uSourceSampler, vSourceSampler](const Vector2D& x) {
        return Vector2D{ uSourceSampler(x), vSourceSampler(x) };
    };
}
}  // namespace CubbyFlow