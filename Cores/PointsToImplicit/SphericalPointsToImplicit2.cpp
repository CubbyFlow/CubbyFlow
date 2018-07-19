/*************************************************************************
> File Name: SphericalPointsToImplicit2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D points-to-implicit converter based on simple sphere model.
> Created Time: 2017/11/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Particle/ParticleSystemData2.h>
#include <PointsToImplicit/SphericalPointsToImplicit2.h>
#include <Solver/LevelSet/FMMLevelSetSolver2.h>
#include <Utils/Logging.h>

namespace CubbyFlow
{
	SphericalPointsToImplicit2::SphericalPointsToImplicit2(double radius, bool isOutputSDF) :
		m_radius(radius), m_isOutputSDF(isOutputSDF)
	{
		// Do nothing
	}

	void SphericalPointsToImplicit2::Convert(const ConstArrayAccessor1<Vector2D>& points, ScalarGrid2* output) const
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

		ParticleSystemData2 particles;
		particles.AddParticles(points);
		particles.BuildNeighborSearcher(2.0 * m_radius);

		const auto neighborSearcher = particles.GetNeighborSearcher();

		auto temp = output->Clone();
		temp->Fill([&](const Vector2D& x)
		{
			double minDist = 2.0 * m_radius;
			neighborSearcher->ForEachNearbyPoint(x, 2.0 * m_radius,
				[&](size_t, const Vector2D& xj)
			{
				minDist = std::min(minDist, (x - xj).Length());
			});

			return minDist - m_radius;
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
