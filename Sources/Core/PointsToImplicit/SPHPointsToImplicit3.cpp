/*************************************************************************
> File Name: SPHPointsToImplicit3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D points-to-implicit converter based on standard SPH kernel.
> Created Time: 2017/11/18
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/PointsToImplicit/SPHPointsToImplicit3.h>
#include <Core/Solver/LevelSet/FMMLevelSetSolver3.h>
#include <Core/SPH/SPHSystemData3.h>
#include <Core/Utils/Logging.h>

namespace CubbyFlow
{
	SPHPointsToImplicit3::SPHPointsToImplicit3(double kernelRadius, double cutOffDensity, bool isOutputSDF) :
		m_kernelRadius(kernelRadius), m_cutOffDensity(cutOffDensity), m_isOutputSDF(isOutputSDF)
	{
		// Do nothing
	}

	void SPHPointsToImplicit3::Convert(const ConstArrayAccessor1<Vector3D>& points, ScalarGrid3* output) const
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
		temp->Fill([&](const Vector3D& x)
		{
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
}