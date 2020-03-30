// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/SPH/SPHSystemData3.h>
#include <Core/Utils/Logging.h>
#include <Core/PointsToImplicit/SPHPointsToImplicit3.hpp>
#include <Core/Solver/LevelSet/FMMLevelSetSolver3.hpp>

namespace CubbyFlow
{
SPHPointsToImplicit3::SPHPointsToImplicit3(double kernelRadius,
                                           double cutOffDensity,
                                           bool isOutputSDF)
    : m_kernelRadius(kernelRadius),
      m_cutOffDensity(cutOffDensity),
      m_isOutputSDF(isOutputSDF)
{
    // Do nothing
}

void SPHPointsToImplicit3::Convert(const ConstArrayAccessor1<Vector3D>& points,
                                   ScalarGrid3* output) const
{
    if (output == nullptr)
    {
        CUBBYFLOW_WARN << "Null scalar grid output pointer provided.";
        return;
    }

    const auto res = output->Resolution();
    if (res.x * res.y * res.z == 0)
    {
        CUBBYFLOW_WARN << "Empty grid is provided.";
        return;
    }

    const auto bbox = output->BoundingBox();
    if (bbox.IsEmpty())
    {
        CUBBYFLOW_WARN << "Empty domain is provided.";
        return;
    }

    SPHSystemData3 sphParticles;
    sphParticles.AddParticles(points);
    sphParticles.SetKernelRadius(m_kernelRadius);
    sphParticles.BuildNeighborSearcher();
    sphParticles.UpdateDensities();

    Array1<double> constData(sphParticles.GetNumberOfParticles(), 1.0);
    auto temp = output->Clone();
    temp->Fill([&](const Vector3D& x) {
        double d = sphParticles.Interpolate(x, constData);
        return m_cutOffDensity - d;
    });

    if (m_isOutputSDF)
    {
        FMMLevelSetSolver3 solver;
        solver.Reinitialize(*temp, std::numeric_limits<double>::max(), output);
    }
    else
    {
        temp->Swap(output);
    }
}
}  // namespace CubbyFlow