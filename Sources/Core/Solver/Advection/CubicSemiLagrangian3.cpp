// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/Advection/CubicSemiLagrangian3.hpp>

namespace CubbyFlow
{
std::function<double(const Vector3D&)>
CubicSemiLagrangian3::GetScalarSamplerFunc(const ScalarGrid3& source) const
{
    const auto sourceSampler = MonotonicCatmullRomArraySampler3<double>{
        source.DataView(), source.GridSpacing(), source.DataOrigin()
    };
    return sourceSampler.Functor();
}

std::function<Vector3D(const Vector3D&)>
CubicSemiLagrangian3::GetVectorSamplerFunc(
    const CollocatedVectorGrid3& source) const
{
    const auto sourceSampler = MonotonicCatmullRomArraySampler3<Vector3D>{
        source.DataView(), source.GridSpacing(), source.DataOrigin()
    };
    return sourceSampler.Functor();
}

std::function<Vector3D(const Vector3D&)>
CubicSemiLagrangian3::GetVectorSamplerFunc(
    const FaceCenteredGrid3& source) const
{
    auto uSourceSampler = MonotonicCatmullRomArraySampler3<double>(
        source.UView(), source.GridSpacing(), source.UOrigin());
    auto vSourceSampler = MonotonicCatmullRomArraySampler3<double>(
        source.VView(), source.GridSpacing(), source.VOrigin());
    auto wSourceSampler = MonotonicCatmullRomArraySampler3<double>(
        source.WView(), source.GridSpacing(), source.WOrigin());
    return [uSourceSampler, vSourceSampler, wSourceSampler](const Vector3D& x) {
        return Vector3D{ uSourceSampler(x), vSourceSampler(x),
                         wSourceSampler(x) };
    };
}
}  // namespace CubbyFlow