/*************************************************************************
> File Name: PCISPHSolver3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D PCISPH solver.
> Created Time: 2017/06/10
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/PointGenerator/BccLatticePointGenerator.hpp>
#include <Core/Solver/Particle/PCISPH/PCISPHSolver3.h>
#include <Core/SPH/SPHStdKernel3.h>
#include <Core/Utils/Logging.h>

namespace CubbyFlow
{
	// Heuristically chosen
	const double DEFAULT_TIME_STEP_LIMIT_SCALE = 5.0;

	PCISPHSolver3::PCISPHSolver3()
	{
		SetTimeStepLimitScale(DEFAULT_TIME_STEP_LIMIT_SCALE);
	}

	PCISPHSolver3::PCISPHSolver3(double targetDensity, double targetSpacing, double relativeKernelRadius) :
		SPHSolver3(targetDensity, targetSpacing, relativeKernelRadius)
	{
		SetTimeStepLimitScale(DEFAULT_TIME_STEP_LIMIT_SCALE);
	}

	PCISPHSolver3::~PCISPHSolver3()
	{
		// Do nothing
	}

	double PCISPHSolver3::GetMaxDensityErrorRatio() const
	{
		return m_maxDensityErrorRatio;
	}

	void PCISPHSolver3::SetMaxDensityErrorRatio(double ratio)
	{
		m_maxDensityErrorRatio = std::max(ratio, 0.0);
	}

	unsigned int PCISPHSolver3::GetMaxNumberOfIterations() const
	{
		return m_maxNumberOfIterations;
	}

	void PCISPHSolver3::SetMaxNumberOfIterations(unsigned int n)
	{
		m_maxNumberOfIterations = n;
	}

	void PCISPHSolver3::AccumulatePressureForce(double timeIntervalInSeconds)
	{
		auto particles = GetSPHSystemData();
		const size_t numberOfParticles = particles->GetNumberOfParticles();
		const double delta = ComputeDelta(timeIntervalInSeconds);
		const double targetDensity = particles->GetTargetDensity();
		const double mass = particles->GetMass();

		auto p = particles->GetPressures();
		auto d = particles->GetDensities();
		auto x = particles->GetPositions();
		auto v = particles->GetVelocities();
		auto f = particles->GetForces();

		// Predicted density ds
		Array1<double> ds(numberOfParticles, 0.0);

		SPHStdKernel3 kernel(particles->GetKernelRadius());

		// Initialize buffers
		ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i)
		{
			p[i] = 0.0;
			m_pressureForces[i] = Vector3D();
			m_densityErrors[i] = 0.0;
			ds[i] = d[i];
		});

		unsigned int maxNumIter = 0;
		double maxDensityError;
		double densityErrorRatio = 0.0;

		for (unsigned int k = 0; k < m_maxNumberOfIterations; ++k)
		{
			// Predict velocity and position
			ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i)
			{
				m_tempVelocities[i] = v[i] + timeIntervalInSeconds / mass * (f[i] + m_pressureForces[i]);
				m_tempPositions[i] = x[i] + timeIntervalInSeconds * m_tempVelocities[i];
			});

			// Resolve collisions
			ResolveCollision(m_tempPositions, m_tempVelocities);

			// Compute pressure from density error
			ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i)
			{
				double weightSum = 0.0;
				const auto& neighbors = particles->GetNeighborLists()[i];

				for (size_t j : neighbors)
				{
					double dist = m_tempPositions[j].DistanceTo(m_tempPositions[i]);
					weightSum += kernel(dist);
				}
				weightSum += kernel(0);

				double density = mass * weightSum;
				double densityError = (density - targetDensity);
				double pressure = delta * densityError;

				if (pressure < 0.0)
				{
					pressure *= GetNegativePressureScale();
					densityError *= GetNegativePressureScale();
				}

				p[i] += pressure;
				ds[i] = density;
				m_densityErrors[i] = densityError;
			});

			// Compute pressure gradient force
			m_pressureForces.Set(Vector3D());
			SPHSolver3::AccumulatePressureForce(x, ds.ConstAccessor(), p, m_pressureForces.Accessor());

			// Compute max density error
			maxDensityError = 0.0;
			for (size_t i = 0; i < numberOfParticles; ++i)
			{
				maxDensityError = AbsMax(maxDensityError, m_densityErrors[i]);
			}

			densityErrorRatio = maxDensityError / targetDensity;
			maxNumIter = k + 1;

			if (std::fabs(densityErrorRatio) < m_maxDensityErrorRatio)
			{
				break;
			}
		}

		CUBBYFLOW_INFO << "Number of PCI iterations: " << maxNumIter;
		CUBBYFLOW_INFO << "Max density error after PCI iteration: " << maxDensityError;

		if (std::fabs(densityErrorRatio) > m_maxDensityErrorRatio)
		{
			CUBBYFLOW_WARN << "Max density error ratio is greater than the threshold!";
			CUBBYFLOW_WARN << "Ratio: " << densityErrorRatio
				<< " Threshold: " << m_maxDensityErrorRatio;
		}

		// Accumulate pressure force
		ParallelFor(ZERO_SIZE, numberOfParticles, [this, &f](size_t i)
		{
			f[i] += m_pressureForces[i];
		});
	}

	void PCISPHSolver3::OnBeginAdvanceTimeStep(double timeStepInSeconds)
	{
		SPHSolver3::OnBeginAdvanceTimeStep(timeStepInSeconds);

		// Allocate temp buffers
		size_t numberOfParticles = GetParticleSystemData()->GetNumberOfParticles();
		m_tempPositions.Resize(numberOfParticles);
		m_tempVelocities.Resize(numberOfParticles);
		m_pressureForces.Resize(numberOfParticles);
		m_densityErrors.Resize(numberOfParticles);
	}

	double PCISPHSolver3::ComputeDelta(double timeStepInSeconds) const
	{
		auto particles = GetSPHSystemData();
		const double kernelRadius = particles->GetKernelRadius();

		Array1<Vector3D> points;
		BccLatticePointGenerator pointsGenerator;
		Vector3D origin;
		BoundingBox3D sampleBound(origin, origin);
		sampleBound.Expand(1.5 * kernelRadius);

		pointsGenerator.Generate(sampleBound, particles->GetTargetSpacing(), &points);

		SPHSpikyKernel3 kernel(kernelRadius);

		double denom = 0;
		Vector3D denom1;
		double denom2 = 0;

		for (size_t i = 0; i < points.size(); ++i)
		{
			const Vector3D& point = points[i];
			double distanceSquared = point.LengthSquared();

			if (distanceSquared < kernelRadius * kernelRadius)
			{
				double distance = std::sqrt(distanceSquared);
				Vector3D direction = (distance > 0.0) ? point / distance : Vector3D();

				// grad(Wij)
				Vector3D gradWij = kernel.Gradient(distance, direction);
				denom1 += gradWij;
				denom2 += gradWij.Dot(gradWij);
			}
		}

		denom += -denom1.Dot(denom1) - denom2;

		return (std::fabs(denom) > 0.0) ? -1 / (ComputeBeta(timeStepInSeconds) * denom) : 0;
	}

	double PCISPHSolver3::ComputeBeta(double timeStepInSeconds) const
	{
		auto particles = GetSPHSystemData();
		return 2.0 * Square(particles->GetMass() * timeStepInSeconds / particles->GetTargetDensity());
	}

	PCISPHSolver3::Builder PCISPHSolver3::GetBuilder()
	{
		return Builder();
	}

	PCISPHSolver3 PCISPHSolver3::Builder::Build() const
	{
		return PCISPHSolver3(m_targetDensity, m_targetSpacing, m_relativeKernelRadius);
	}

	PCISPHSolver3Ptr PCISPHSolver3::Builder::MakeShared() const
	{
		return std::shared_ptr<PCISPHSolver3>(
			new PCISPHSolver3(m_targetDensity, m_targetSpacing, m_relativeKernelRadius),
			[](PCISPHSolver3* obj)
		{
			delete obj;
		});
	}
}