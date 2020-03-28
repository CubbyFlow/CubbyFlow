/*************************************************************************
> File Name: ZhuBridsonPointsToImplicit3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D points-to-implicit converter based on Zhu and Bridson's method.
> Created Time: 2017/11/18
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Particle/ParticleSystemData3.hpp>
#include <Core/PointsToImplicit/ZhuBridsonPointsToImplicit3.h>
#include <Core/Solver/LevelSet/FMMLevelSetSolver3.h>
#include <Core/Utils/Logging.h>

namespace CubbyFlow
{
	inline double k(double s)
	{
		return std::max(0.0, Cubic(1.0 - s * s));
	}

	ZhuBridsonPointsToImplicit3::ZhuBridsonPointsToImplicit3(double kernelRadius, double cutOffThreshold, bool isOutputSDF) :
		m_kernelRadius(kernelRadius), m_cutOffThreshold(cutOffThreshold), m_isOutputSDF(isOutputSDF)
	{
		// Do nothing
	}

	void ZhuBridsonPointsToImplicit3::Convert(const ConstArrayAccessor1<Vector3D>& points, ScalarGrid3* output) const
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
		particles.BuildNeighborSearcher(m_kernelRadius);

		const auto neighborSearcher = particles.GetNeighborSearcher();
		const double isoContValue = m_cutOffThreshold * m_kernelRadius;

		auto temp = output->Clone();
		temp->Fill([&](const Vector3D& x) -> double
		{
			Vector3D xAvg;
			double wSum = 0.0;
			const auto func = [&](size_t, const Vector3D& xi)
			{
				const double wi = k((x - xi).Length() / m_kernelRadius);
				wSum += wi;
				xAvg += wi * xi;
			};
			neighborSearcher->ForEachNearbyPoint(x, m_kernelRadius, func);

			if (wSum > 0.0) {
				xAvg /= wSum;
				return (x - xAvg).Length() - isoContValue;
			}
			else {
				return output->BoundingBox().DiagonalLength();
			}
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