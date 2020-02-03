// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/CollocatedVectorGrid2.h>
#include <Core/Grid/FaceCenteredGrid2.h>
#include <Core/LevelSet/LevelSetUtils.h>
#include <Core/Surface/SurfaceToImplicit2.h>
#include <Core/Utils/Macros.h>
#include <Core/Emitter/VolumeGridEmitter2.hpp>

namespace CubbyFlow
{
VolumeGridEmitter2::VolumeGridEmitter2(const ImplicitSurface2Ptr& sourceRegion,
                                       bool isOneShot)
    : m_sourceRegion(sourceRegion), m_isOneShot(isOneShot)
{
    // Do nothing
}

VolumeGridEmitter2::~VolumeGridEmitter2()
{
    // Do nothing
}

void VolumeGridEmitter2::AddSignedDistanceTarget(
    const ScalarGrid2Ptr& scalarGridTarget)
{
    auto mapper = [](double sdf, const Vector2D&, double oldVal) {
        return std::min(oldVal, sdf);
    };

    AddTarget(scalarGridTarget, mapper);
}

void VolumeGridEmitter2::AddStepFunctionTarget(
    const ScalarGrid2Ptr& scalarGridTarget, double minValue, double maxValue)
{
    double smoothingWidth = scalarGridTarget->GridSpacing().Min();
    auto mapper = [minValue, maxValue, smoothingWidth, scalarGridTarget](
                      double sdf, const Vector2D&, double oldVal) {
        double step = 1.0 - SmearedHeavisideSDF(sdf / smoothingWidth);
        return std::max(oldVal, (maxValue - minValue) * step + minValue);
    };

    AddTarget(scalarGridTarget, mapper);
}

void VolumeGridEmitter2::AddTarget(const ScalarGrid2Ptr& scalarGridTarget,
                                   const ScalarMapper& customMapper)
{
    m_customScalarTargets.emplace_back(scalarGridTarget, customMapper);
}

void VolumeGridEmitter2::AddTarget(const VectorGrid2Ptr& vectorGridTarget,
                                   const VectorMapper& customMapper)
{
    m_customVectorTargets.emplace_back(vectorGridTarget, customMapper);
}

const ImplicitSurface2Ptr& VolumeGridEmitter2::GetSourceRegion() const
{
    return m_sourceRegion;
}

bool VolumeGridEmitter2::GetIsOneShot() const
{
    return m_isOneShot;
}

void VolumeGridEmitter2::OnUpdate(double currentTimeInSeconds,
                                  double timeIntervalInSeconds)
{
    UNUSED_VARIABLE(currentTimeInSeconds);
    UNUSED_VARIABLE(timeIntervalInSeconds);

    if (m_hasEmitted && m_isOneShot)
    {
        return;
    }

    Emit();

    m_hasEmitted = true;
}

void VolumeGridEmitter2::Emit()
{
    if (m_sourceRegion == nullptr)
    {
        return;
    }

    m_sourceRegion->UpdateQueryEngine();

    for (const auto& target : m_customScalarTargets)
    {
        const auto& grid = std::get<0>(target);
        const auto& mapper = std::get<1>(target);

        auto pos = grid->GetDataPosition();
        grid->ParallelForEachDataPointIndex([&](size_t i, size_t j) {
            Vector2D gx = pos(i, j);
            double sdf = GetSourceRegion()->SignedDistance(gx);

            (*grid)(i, j) = mapper(sdf, gx, (*grid)(i, j));
        });
    }

    for (const auto& target : m_customVectorTargets)
    {
        const auto& grid = std::get<0>(target);
        const auto& mapper = std::get<1>(target);

        CollocatedVectorGrid2Ptr collocated =
            std::dynamic_pointer_cast<CollocatedVectorGrid2>(grid);
        if (collocated != nullptr)
        {
            auto pos = collocated->GetDataPosition();
            collocated->ParallelForEachDataPointIndex([&](size_t i, size_t j) {
                Vector2D gx = pos(i, j);
                double sdf = GetSourceRegion()->SignedDistance(gx);

                if (IsInsideSDF(sdf))
                {
                    (*collocated)(i, j) = mapper(sdf, gx, (*collocated)(i, j));
                }
            });

            continue;
        }

        FaceCenteredGrid2Ptr faceCentered =
            std::dynamic_pointer_cast<FaceCenteredGrid2>(grid);
        if (faceCentered != nullptr)
        {
            auto uPos = faceCentered->GetUPosition();
            auto vPos = faceCentered->GetVPosition();

            faceCentered->ParallelForEachUIndex([&](size_t i, size_t j) {
                Vector2D gx = uPos(i, j);
                double sdf = GetSourceRegion()->SignedDistance(gx);
                Vector2D oldVal = faceCentered->Sample(gx);
                Vector2D newVal = mapper(sdf, gx, oldVal);

                faceCentered->GetU(i, j) = newVal.x;
            });

            faceCentered->ParallelForEachVIndex([&](size_t i, size_t j) {
                Vector2D gx = vPos(i, j);
                double sdf = GetSourceRegion()->SignedDistance(gx);
                Vector2D oldVal = faceCentered->Sample(gx);
                Vector2D newVal = mapper(sdf, gx, oldVal);

                faceCentered->GetV(i, j) = newVal.y;
            });
        }
    }
}

VolumeGridEmitter2::Builder VolumeGridEmitter2::GetBuilder()
{
    return Builder();
}

VolumeGridEmitter2::Builder& VolumeGridEmitter2::Builder::WithSourceRegion(
    const Surface2Ptr& sourceRegion)
{
    auto implicit = std::dynamic_pointer_cast<ImplicitSurface2>(sourceRegion);
    if (implicit != nullptr)
    {
        m_sourceRegion = implicit;
    }
    else
    {
        m_sourceRegion = std::make_shared<SurfaceToImplicit2>(sourceRegion);
    }

    return *this;
}

VolumeGridEmitter2::Builder& VolumeGridEmitter2::Builder::WithIsOneShot(
    bool isOneShot)
{
    m_isOneShot = isOneShot;
    return *this;
}

VolumeGridEmitter2 VolumeGridEmitter2::Builder::Build() const
{
    return VolumeGridEmitter2(m_sourceRegion, m_isOneShot);
}

VolumeGridEmitter2Ptr VolumeGridEmitter2::Builder::MakeShared() const
{
    return std::shared_ptr<VolumeGridEmitter2>(
        new VolumeGridEmitter2(m_sourceRegion, m_isOneShot),
        [](VolumeGridEmitter2* obj) { delete obj; });
}
}  // namespace CubbyFlow
