// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/Hybrid/FLIP/FLIPSolver2.hpp>

namespace CubbyFlow
{
FLIPSolver2::FLIPSolver2() : FLIPSolver2{ { 1, 1 }, { 1, 1 }, { 0, 0 } }
{
    // Do nothing
}

FLIPSolver2::FLIPSolver2(const Vector2UZ& resolution,
                         const Vector2D& gridSpacing,
                         const Vector2D& gridOrigin)
    : PICSolver2(resolution, gridSpacing, gridOrigin)
{
    // Do nothing
}

double FLIPSolver2::GetPICBlendingFactor() const
{
    return m_picBlendingFactor;
}

void FLIPSolver2::SetPICBlendingFactor(double factor)
{
    m_picBlendingFactor = std::clamp(factor, 0.0, 1.0);
}

void FLIPSolver2::TransferFromParticlesToGrids()
{
    PICSolver2::TransferFromParticlesToGrids();

    // Store snapshot
    const FaceCenteredGrid2Ptr vel = GetGridSystemData()->GetVelocity();
    ConstArrayView2<double> u = GetGridSystemData()->GetVelocity()->UView();
    ConstArrayView2<double> v = GetGridSystemData()->GetVelocity()->VView();
    m_uDelta.Resize(u.Size());
    m_vDelta.Resize(v.Size());

    vel->ParallelForEachUIndex(
        [&](size_t i, size_t j) { m_uDelta(i, j) = u(i, j); });
    vel->ParallelForEachVIndex(
        [&](size_t i, size_t j) { m_vDelta(i, j) = v(i, j); });
}

void FLIPSolver2::TransferFromGridsToParticles()
{
    FaceCenteredGrid2Ptr flow = GetGridSystemData()->GetVelocity();
    ArrayView1<Vector2<double>> positions =
        GetParticleSystemData()->Positions();
    ArrayView1<Vector2<double>> velocities =
        GetParticleSystemData()->Velocities();
    const size_t numberOfParticles =
        GetParticleSystemData()->NumberOfParticles();

    // Compute delta
    flow->ParallelForEachUIndex([&](size_t i, size_t j) {
        m_uDelta(i, j) = flow->GetU(i, j) - m_uDelta(i, j);
    });

    flow->ParallelForEachVIndex([&](size_t i, size_t j) {
        m_vDelta(i, j) = flow->GetV(i, j) - m_vDelta(i, j);
    });

    LinearArraySampler2<double> uSampler{ m_uDelta.View(),
                                          flow->GridSpacing().CastTo<double>(),
                                          flow->UOrigin().CastTo<double>() };
    LinearArraySampler2<double> vSampler{ m_vDelta.View(),
                                          flow->GridSpacing().CastTo<double>(),
                                          flow->VOrigin().CastTo<double>() };

    auto sampler = [uSampler, vSampler](const Vector2D& x) {
        const Vector2<double> xf = x.CastTo<double>();
        const double u = uSampler(xf);
        const double v = vSampler(xf);
        return Vector2D{ u, v };
    };

    // Transfer delta to the particles
    ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
        Vector2D flipVel = velocities[i] + sampler(positions[i]);

        if (m_picBlendingFactor > 0.0)
        {
            const Vector2D picVel = flow->Sample(positions[i]);
            flipVel = Lerp(flipVel, picVel, m_picBlendingFactor);
        }

        velocities[i] = flipVel;
    });
}

FLIPSolver2::Builder FLIPSolver2::GetBuilder()
{
    return Builder{};
}

FLIPSolver2 FLIPSolver2::Builder::Build() const
{
    return FLIPSolver2{ m_resolution, GetGridSpacing(), m_gridOrigin };
}

FLIPSolver2Ptr FLIPSolver2::Builder::MakeShared() const
{
    return std::shared_ptr<FLIPSolver2>(
        new FLIPSolver2{ m_resolution, GetGridSpacing(), m_gridOrigin },
        [](FLIPSolver2* obj) { delete obj; });
}
}  // namespace CubbyFlow