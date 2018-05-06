/*************************************************************************
> File Name: SPHPointsToImplicit2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D points-to-implicit converter based on standard SPH kernel.
> Created Time: 2017/11/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/PointsToImplicit/SPHPointsToImplicit2.h>
#include <Core/Solver/LevelSet/FMMLevelSetSolver2.h>
#include <Core/SPH/SPHSystemData2.h>
#include <Core/Utils/Logging.h>

namespace CubbyFlow
{
	SPHPointsToImplicit2::SPHPointsToImplicit2(double kernelRadius, double cutOffDensity, bool isOutputSDF) :
		m_kernelRadius(kernelRadius), m_cutOffDensity(cutOffDensity), m_isOutputSDF(isOutputSDF)
	{
		// Do nothing
	}

	void SPHPointsToImplicit2::Convert(const ConstArrayAccessor1<Vector2D>& points, ScalarGrid2* output) const
	{
		if (output == nullptr)
		{
			CUBBYFLOW_WARN << "Null scalar grid output pointer provided.";
			return;
		}

		const auto res = output->Resolution();
		if (res.x * res.y == 0)
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

		SPHSystemData2 sphParticles;
		sphParticles.AddParticles(points);
		sphParticles.SetKernelRadius(m_kernelRadius);
		sphParticles.BuildNeighborSearcher();
		sphParticles.UpdateDensities();

		Array1<double> constData(sphParticles.GetNumberOfParticles(), 1.0);
		auto temp = output->Clone();
		temp->Fill([&](const Vector2D& x)
		{
			double d = sphParticles.Interpolate(x, constData);
			return m_cutOffDensity - d;
		});

		if (m_isOutputSDF)
		{
			FMMLevelSetSolver2 solver;
			solver.Reinitialize(*temp, std::numeric_limits<double>::max(), output);
		}
		else
		{
			temp->Swap(output);
		}
	}
}
