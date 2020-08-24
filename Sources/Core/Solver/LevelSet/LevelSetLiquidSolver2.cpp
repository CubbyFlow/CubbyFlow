// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/CellCenteredScalarGrid2.hpp>
#include <Core/LevelSet/LevelSetUtils.hpp>
#include <Core/Solver/LevelSet/ENOLevelSetSolver2.hpp>
#include <Core/Solver/LevelSet/FMMLevelSetSolver2.hpp>
#include <Core/Solver/LevelSet/LevelSetLiquidSolver2.hpp>
#include <Core/Utils/Logging.hpp>
#include <Core/Utils/Timer.hpp>

namespace CubbyFlow
{
LevelSetLiquidSolver2::LevelSetLiquidSolver2()
    : LevelSetLiquidSolver2({ 1, 1 }, { 1, 1 }, { 0, 0 })
{
    // Do nothing
}

LevelSetLiquidSolver2::LevelSetLiquidSolver2(const Size2& resolution,
                                             const Vector2D& gridSpacing,
                                             const Vector2D& gridOrigin)
    : GridFluidSolver2(resolution, gridSpacing, gridOrigin)
{
    auto grids = GetGridSystemData();
    m_signedDistanceFieldId = grids->AddAdvectableScalarData(
        std::make_shared<CellCenteredScalarGrid2::Builder>(),
        std::numeric_limits<double>::max());
    m_levelSetSolver = std::make_shared<ENOLevelSetSolver2>();
}

LevelSetLiquidSolver2::~LevelSetLiquidSolver2()
{
    // Do nothing
}

ScalarGrid2Ptr LevelSetLiquidSolver2::GetSignedDistanceField() const
{
    return GetGridSystemData()->GetAdvectableScalarDataAt(
        m_signedDistanceFieldId);
}

LevelSetSolver2Ptr LevelSetLiquidSolver2::GetLevelSetSolver() const
{
    return m_levelSetSolver;
}

void LevelSetLiquidSolver2::SetLevelSetSolver(
    const LevelSetSolver2Ptr& newSolver)
{
    m_levelSetSolver = newSolver;
}

void LevelSetLiquidSolver2::SetMinReinitializeDistance(double distance)
{
    m_minReinitializeDistance = distance;
}

void LevelSetLiquidSolver2::SetIsGlobalCompensationEnabled(bool isEnabled)
{
    m_isGlobalCompensationEnabled = isEnabled;
}

double LevelSetLiquidSolver2::ComputeVolume() const
{
    auto sdf = GetSignedDistanceField();
    const Vector2D gridSpacing = sdf->GridSpacing();
    const double cellVolume = gridSpacing.x * gridSpacing.y;
    const double h = std::max(gridSpacing.x, gridSpacing.y);

    double volume = 0.0;
    sdf->ForEachDataPointIndex([&](size_t i, size_t j) {
        volume += 1.0 - SmearedHeavisideSDF((*sdf)(i, j) / h);
    });
    volume *= cellVolume;

    return volume;
}

void LevelSetLiquidSolver2::OnBeginAdvanceTimeStep(double timeIntervalInSeconds)
{
    UNUSED_VARIABLE(timeIntervalInSeconds);

    // Measure current volume
    m_lastKnownVolume = ComputeVolume();

    CUBBYFLOW_INFO << "Current volume: " << m_lastKnownVolume;
}

void LevelSetLiquidSolver2::OnEndAdvanceTimeStep(double timeIntervalInSeconds)
{
    double currentCfl = GetCFL(timeIntervalInSeconds);

    Timer timer;
    Reinitialize(currentCfl);
    CUBBYFLOW_INFO << "reinitializing level set field took "
                   << timer.DurationInSeconds() << " seconds";

    // Measure current volume
    double currentVol = ComputeVolume();
    double volDiff = currentVol - m_lastKnownVolume;

    CUBBYFLOW_INFO << "Current volume: " << currentVol << " "
                   << "Volume diff: " << volDiff;

    if (m_isGlobalCompensationEnabled)
    {
        AddVolume(-volDiff);

        currentVol = ComputeVolume();
        CUBBYFLOW_INFO << "Volume after global compensation: " << currentVol;
    }
}

void LevelSetLiquidSolver2::ComputeAdvection(double timeIntervalInSeconds)
{
    double currentCFL = GetCFL(timeIntervalInSeconds);

    Timer timer;
    ExtrapolateVelocityToAir(currentCFL);
    CUBBYFLOW_INFO << "velocity extrapolation took "
                   << timer.DurationInSeconds() << " seconds";

    GridFluidSolver2::ComputeAdvection(timeIntervalInSeconds);
}

ScalarField2Ptr LevelSetLiquidSolver2::GetFluidSDF() const
{
    return GetSignedDistanceField();
}

void LevelSetLiquidSolver2::Reinitialize(double currentCfl)
{
    if (m_levelSetSolver != nullptr)
    {
        auto sdf = GetSignedDistanceField();
        auto sdf0 = sdf->Clone();

        const Vector2D gridSpacing = sdf->GridSpacing();
        const double h = std::max(gridSpacing.x, gridSpacing.y);
        const double maxReinitDist =
            std::max(2.0 * currentCfl, m_minReinitializeDistance) * h;

        CUBBYFLOW_INFO << "Max reinitialize distance: " << maxReinitDist;

        m_levelSetSolver->Reinitialize(*sdf0, maxReinitDist, sdf.get());
        ExtrapolateIntoCollider(sdf.get());
    }
}

void LevelSetLiquidSolver2::ExtrapolateVelocityToAir(double currentCFL)
{
    auto sdf = GetSignedDistanceField();
    auto vel = GetGridSystemData()->GetVelocity();

    auto u = vel->GetUAccessor();
    auto v = vel->GetVAccessor();
    auto uPos = vel->GetUPosition();
    auto vPos = vel->GetVPosition();

    Array2<char> uMarker(u.size());
    Array2<char> vMarker(v.size());

    uMarker.ParallelForEachIndex([&](size_t i, size_t j) {
        if (IsInsideSDF(sdf->Sample(uPos(i, j))))
        {
            uMarker(i, j) = 1;
        }
        else
        {
            uMarker(i, j) = 0;
            u(i, j) = 0.0;
        }
    });

    vMarker.ParallelForEachIndex([&](size_t i, size_t j) {
        if (IsInsideSDF(sdf->Sample(vPos(i, j))))
        {
            vMarker(i, j) = 1;
        }
        else
        {
            vMarker(i, j) = 0;
            v(i, j) = 0.0;
        }
    });

    const Vector2D gridSpacing = sdf->GridSpacing();
    const double h = std::max(gridSpacing.x, gridSpacing.y);
    const double maxDist =
        std::max(2.0 * currentCFL, m_minReinitializeDistance) * h;

    CUBBYFLOW_INFO << "Max velocity extrapolation distance: " << maxDist;

    FMMLevelSetSolver2 fmmSolver;
    fmmSolver.Extrapolate(*vel, *sdf, maxDist, vel.get());

    ApplyBoundaryCondition();
}

void LevelSetLiquidSolver2::AddVolume(double volDiff)
{
    auto sdf = GetSignedDistanceField();
    const Vector2D gridSpacing = sdf->GridSpacing();
    const double cellVolume = gridSpacing.x * gridSpacing.y;
    const double h = std::max(gridSpacing.x, gridSpacing.y);

    double volume0 = 0.0;
    double volume1 = 0.0;
    sdf->ForEachDataPointIndex([&](size_t i, size_t j) {
        volume0 += 1.0 - SmearedHeavisideSDF((*sdf)(i, j) / h);
        volume1 += 1.0 - SmearedHeavisideSDF((*sdf)(i, j) / h + 1.0);
    });
    volume0 *= cellVolume;
    volume1 *= cellVolume;

    const double dVdh = (volume1 - volume0) / h;

    if (std::abs(dVdh) > 0.0)
    {
        double dist = volDiff / dVdh;

        sdf->ParallelForEachDataPointIndex(
            [&](size_t i, size_t j) { (*sdf)(i, j) += dist; });
    }
}

LevelSetLiquidSolver2::Builder LevelSetLiquidSolver2::GetBuilder()
{
    return Builder();
}

LevelSetLiquidSolver2 LevelSetLiquidSolver2::Builder::Build() const
{
    return LevelSetLiquidSolver2(m_resolution, GetGridSpacing(), m_gridOrigin);
}

LevelSetLiquidSolver2Ptr LevelSetLiquidSolver2::Builder::MakeShared() const
{
    return std::shared_ptr<LevelSetLiquidSolver2>(
        new LevelSetLiquidSolver2(m_resolution, GetGridSpacing(), m_gridOrigin),
        [](LevelSetLiquidSolver2* obj) { delete obj; });
}
}  // namespace CubbyFlow