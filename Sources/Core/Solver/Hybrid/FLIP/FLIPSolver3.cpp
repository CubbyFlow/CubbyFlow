// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/Hybrid/FLIP/FLIPSolver3.hpp>

namespace CubbyFlow
{
FLIPSolver3::FLIPSolver3()
    : FLIPSolver3{ { 1, 1, 1 }, { 1, 1, 1 }, { 0, 0, 0 } }
{
    // Do nothing
}

FLIPSolver3::FLIPSolver3(const Size3& resolution, const Vector3D& gridSpacing,
                         const Vector3D& gridOrigin)
    : PICSolver3{ resolution, gridSpacing, gridOrigin }
{
    // Do nothing
}

double FLIPSolver3::GetPICBlendingFactor() const
{
    return m_picBlendingFactor;
}

void FLIPSolver3::SetPICBlendingFactor(double factor)
{
    m_picBlendingFactor = std::clamp(factor, 0.0, 1.0);
}

void FLIPSolver3::TransferFromParticlesToGrids()
{
    PICSolver3::TransferFromParticlesToGrids();

    // Store snapshot
    const FaceCenteredGrid3Ptr vel = GetGridSystemData()->GetVelocity();
    ConstArrayAccessor3<double> u =
        GetGridSystemData()->GetVelocity()->GetUConstAccessor();
    ConstArrayAccessor3<double> v =
        GetGridSystemData()->GetVelocity()->GetVConstAccessor();
    ConstArrayAccessor3<double> w =
        GetGridSystemData()->GetVelocity()->GetWConstAccessor();
    m_uDelta.Resize(u.size());
    m_vDelta.Resize(v.size());
    m_wDelta.Resize(w.size());

    vel->ParallelForEachUIndex(
        [&](size_t i, size_t j, size_t k) { m_uDelta(i, j, k) = u(i, j, k); });
    vel->ParallelForEachVIndex(
        [&](size_t i, size_t j, size_t k) { m_vDelta(i, j, k) = v(i, j, k); });
    vel->ParallelForEachWIndex(
        [&](size_t i, size_t j, size_t k) { m_wDelta(i, j, k) = w(i, j, k); });
}

void FLIPSolver3::TransferFromGridsToParticles()
{
    FaceCenteredGrid3Ptr flow = GetGridSystemData()->GetVelocity();
    ArrayAccessor1<Vector3<double>> positions =
        GetParticleSystemData()->GetPositions();
    ArrayAccessor1<Vector3<double>> velocities =
        GetParticleSystemData()->GetVelocities();
    const size_t numberOfParticles =
        GetParticleSystemData()->GetNumberOfParticles();

    // Compute delta
    flow->ParallelForEachUIndex([&](size_t i, size_t j, size_t k) {
        m_uDelta(i, j, k) = flow->GetU(i, j, k) - m_uDelta(i, j, k);
    });

    flow->ParallelForEachVIndex([&](size_t i, size_t j, size_t k) {
        m_vDelta(i, j, k) = flow->GetV(i, j, k) - m_vDelta(i, j, k);
    });

    flow->ParallelForEachWIndex([&](size_t i, size_t j, size_t k) {
        m_wDelta(i, j, k) = flow->GetW(i, j, k) - m_wDelta(i, j, k);
    });

    LinearArraySampler3<double, double> uSampler{
        m_uDelta.ConstAccessor(), flow->GridSpacing().CastTo<double>(),
        flow->GetUOrigin().CastTo<double>()
    };
    LinearArraySampler3<double, double> vSampler{
        m_vDelta.ConstAccessor(), flow->GridSpacing().CastTo<double>(),
        flow->GetVOrigin().CastTo<double>()
    };
    LinearArraySampler3<double, double> wSampler{
        m_wDelta.ConstAccessor(), flow->GridSpacing().CastTo<double>(),
        flow->GetWOrigin().CastTo<double>()
    };

    auto sampler = [uSampler, vSampler, wSampler](const Vector3D& x) {
        const Vector3<double> xf = x.CastTo<double>();
        const double u = uSampler(xf);
        const double v = vSampler(xf);
        const double w = wSampler(xf);
        return Vector3D{ u, v, w };
    };

    // Transfer delta to the particles
    ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
        Vector3D flipVel = velocities[i] + sampler(positions[i]);

        if (m_picBlendingFactor > 0.0)
        {
            const Vector3D picVel = flow->Sample(positions[i]);
            flipVel = Lerp(flipVel, picVel, m_picBlendingFactor);
        }

        velocities[i] = flipVel;
    });
}

FLIPSolver3::Builder FLIPSolver3::GetBuilder()
{
    return Builder{};
}

FLIPSolver3 FLIPSolver3::Builder::Build() const
{
    return FLIPSolver3{ m_resolution, GetGridSpacing(), m_gridOrigin };
}

FLIPSolver3Ptr FLIPSolver3::Builder::MakeShared() const
{
    return std::shared_ptr<FLIPSolver3>(
        new FLIPSolver3{ m_resolution, GetGridSpacing(), m_gridOrigin },
        [](FLIPSolver3* obj) { delete obj; });
}
}  // namespace CubbyFlow