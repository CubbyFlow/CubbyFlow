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
    const auto sourceSampler =
        CubicArraySampler3<double, double>{ source.GetConstDataAccessor(),
                                            source.GridSpacing(),
                                            source.GetDataOrigin() };
    return sourceSampler.Functor();
}

std::function<Vector3D(const Vector3D&)>
CubicSemiLagrangian3::GetVectorSamplerFunc(
    const CollocatedVectorGrid3& source) const
{
    const auto sourceSampler =
        CubicArraySampler3<Vector3D, double>{ source.GetConstDataAccessor(),
                                              source.GridSpacing(),
                                              source.GetDataOrigin() };
    return sourceSampler.Functor();
}

std::function<Vector3D(const Vector3D&)>
CubicSemiLagrangian3::GetVectorSamplerFunc(
    const FaceCenteredGrid3& source) const
{
    auto uSourceSampler = CubicArraySampler3<double, double>(
        source.GetUConstAccessor(), source.GridSpacing(), source.GetUOrigin());
    auto vSourceSampler = CubicArraySampler3<double, double>(
        source.GetVConstAccessor(), source.GridSpacing(), source.GetVOrigin());
    auto wSourceSampler = CubicArraySampler3<double, double>(
        source.GetWConstAccessor(), source.GridSpacing(), source.GetWOrigin());
    return [uSourceSampler, vSourceSampler, wSourceSampler](const Vector3D& x) {
        return Vector3D{ uSourceSampler(x), vSourceSampler(x),
                         wSourceSampler(x) };
    };
}
}  // namespace CubbyFlow