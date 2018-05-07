/*************************************************************************
> File Name: SphericalPointsToImplicit3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D points-to-implicit converter based on simple sphere model.
> Created Time: 2017/11/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Particle/ParticleSystemData3.h>
#include <Core/PointsToImplicit/SphericalPointsToImplicit3.h>
#include <Core/Solver/LevelSet/FMMLevelSetSolver3.h>
#include <Core/Utils/Logging.h>

namespace CubbyFlow
{
	SphericalPointsToImplicit3::SphericalPointsToImplicit3(double radius, bool isOutputSDF) :
		m_radius(radius), m_isOutputSDF(isOutputSDF)
	{
		// Do nothing
	}

	void SphericalPointsToImplicit3::Convert(const ConstArrayAccessor1<Vector3D>& points, ScalarGrid3* output) const
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

		ParticleSystemData3 particles;
		particles.AddParticles(points);
		particles.BuildNeighborSearcher(2.0 * m_radius);

		const auto neighborSearcher = particles.GetNeighborSearcher();

		auto temp = output->Clone();
		temp->Fill([&](const Vector3D& x)
		{
			double minDist = 2.0 * m_radius;
			neighborSearcher->ForEachNearbyPoint(x, 2.0 * m_radius,
				[&](size_t, const Vector3D& xj)
			{
				minDist = std::min(minDist, (x - xj).Length());
			});

			return minDist - m_radius;
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