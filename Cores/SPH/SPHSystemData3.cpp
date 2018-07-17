/*************************************************************************
> File Name: SPHSystemData3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D SPH particle system data.
> Created Time: 2017/06/03
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <BoundingBox/BoundingBox3.h>
#include <PointGenerator/BccLatticePointGenerator.h>
#include <SPH/SPHStdKernel3.h>
#include <SPH/SPHSystemData3.h>

#include <Flatbuffers/generated/SPHSystemData3_generated.h>

namespace CubbyFlow
{
	SPHSystemData3::SPHSystemData3() :
		SPHSystemData3(0)
	{
		// Do nothing
	}

	SPHSystemData3::SPHSystemData3(size_t numberOfParticles)
		: ParticleSystemData3(numberOfParticles)
	{
		m_densityIdx = AddScalarData();
		m_pressureIdx = AddScalarData();

		SetTargetSpacing(m_targetSpacing);
	}

	SPHSystemData3::SPHSystemData3(const SPHSystemData3& other)
	{
		Set(other);
	}

	SPHSystemData3::~SPHSystemData3()
	{
		// Do nothing
	}

	void SPHSystemData3::SetRadius(double newRadius)
	{
		// Interpret it as Setting target spacing
		SetTargetSpacing(newRadius);
	}

	void SPHSystemData3::SetMass(double newMass)
	{
		double incRatio = newMass / GetMass();
		m_targetDensity *= incRatio;
		ParticleSystemData3::SetMass(newMass);
	}

	ConstArrayAccessor1<double> SPHSystemData3::GetDensities() const
	{
		return ScalarDataAt(m_densityIdx);
	}

	ArrayAccessor1<double> SPHSystemData3::GetDensities()
	{
		return ScalarDataAt(m_densityIdx);
	}

	ConstArrayAccessor1<double> SPHSystemData3::GetPressures() const
	{
		return ScalarDataAt(m_pressureIdx);
	}

	ArrayAccessor1<double> SPHSystemData3::GetPressures()
	{
		return ScalarDataAt(m_pressureIdx);
	}

	void SPHSystemData3::UpdateDensities()
	{
		auto p = GetPositions();
		auto d = GetDensities();
		const double m = GetMass();

		ParallelFor(ZERO_SIZE, GetNumberOfParticles(), [&](size_t i)
		{
			double sum = SumOfKernelNearby(p[i]);
			d[i] = m * sum;
		});
	}

	void SPHSystemData3::SetTargetDensity(double targetDensity)
	{
		m_targetDensity = targetDensity;

		ComputeMass();
	}

	double SPHSystemData3::GetTargetDensity() const
	{
		return m_targetDensity;
	}

	void SPHSystemData3::SetTargetSpacing(double spacing)
	{
		ParticleSystemData3::SetRadius(spacing);

		m_targetSpacing = spacing;
		m_kernelRadius = m_kernelRadiusOverTargetSpacing * m_targetSpacing;

		ComputeMass();
	}

	double SPHSystemData3::GetTargetSpacing() const
	{
		return m_targetSpacing;
	}

	void SPHSystemData3::SetRelativeKernelRadius(double relativeRadius)
	{
		m_kernelRadiusOverTargetSpacing = relativeRadius;
		m_kernelRadius = m_kernelRadiusOverTargetSpacing * m_targetSpacing;

		ComputeMass();
	}

	double SPHSystemData3::GetRelativeKernelRadius() const
	{
		return m_kernelRadiusOverTargetSpacing;
	}

	void SPHSystemData3::SetKernelRadius(double kernelRadius)
	{
		m_kernelRadius = kernelRadius;
		m_targetSpacing = kernelRadius / m_kernelRadiusOverTargetSpacing;

		ComputeMass();
	}

	double SPHSystemData3::GetKernelRadius() const
	{
		return m_kernelRadius;
	}

	double SPHSystemData3::SumOfKernelNearby(const Vector3D& origin) const
	{
		double sum = 0.0;
		SPHStdKernel3 kernel(m_kernelRadius);

		GetNeighborSearcher()->ForEachNearbyPoint(origin, m_kernelRadius,
			[&](size_t, const Vector3D& neighborPosition)
		{
			double dist = origin.DistanceTo(neighborPosition);
			sum += kernel(dist);
		});

		return sum;
	}

	double SPHSystemData3::Interpolate(const Vector3D& origin, const ConstArrayAccessor1<double>& values) const
	{
		double sum = 0.0;
		auto d = GetDensities();
		SPHStdKernel3 kernel(m_kernelRadius);
		const double m = GetMass();

		GetNeighborSearcher()->ForEachNearbyPoint(origin, m_kernelRadius,
			[&](size_t i, const Vector3D& neighborPosition)
		{
			double dist = origin.DistanceTo(neighborPosition);
			double weight = m / d[i] * kernel(dist);

			sum += weight * values[i];
		});

		return sum;
	}

	Vector3D SPHSystemData3::Interpolate(const Vector3D& origin, const ConstArrayAccessor1<Vector3D>& values) const
	{
		Vector3D sum;
		auto d = GetDensities();
		SPHStdKernel3 kernel(m_kernelRadius);
		const double m = GetMass();

		GetNeighborSearcher()->ForEachNearbyPoint(origin, m_kernelRadius,
			[&](size_t i, const Vector3D& neighborPosition)
		{
			double dist = origin.DistanceTo(neighborPosition);
			double weight = m / d[i] * kernel(dist);

			sum += weight * values[i];
		});

		return sum;
	}

	Vector3D SPHSystemData3::GradientAt(size_t i, const ConstArrayAccessor1<double>& values) const
	{
		Vector3D sum;
		auto p = GetPositions();
		auto d = GetDensities();
		const auto& neighbors = GetNeighborLists()[i];
		Vector3D origin = p[i];
		SPHSpikyKernel3 kernel(m_kernelRadius);
		const double m = GetMass();

		for (size_t j : neighbors)
		{
			Vector3D neighborPosition = p[j];
			double dist = origin.DistanceTo(neighborPosition);

			if (dist > 0.0)
			{
				Vector3D dir = (neighborPosition - origin) / dist;
				sum += d[i] * m * (values[i] / Square(d[i]) + values[j] / Square(d[j])) * kernel.Gradient(dist, dir);
			}
		}

		return sum;
	}

	double SPHSystemData3::LaplacianAt(size_t i, const ConstArrayAccessor1<double>& values) const
	{
		double sum = 0.0;
		auto p = GetPositions();
		auto d = GetDensities();
		const auto& neighbors = GetNeighborLists()[i];
		Vector3D origin = p[i];
		SPHSpikyKernel3 kernel(m_kernelRadius);
		const double m = GetMass();

		for (size_t j : neighbors)
		{
			Vector3D neighborPosition = p[j];
			double dist = origin.DistanceTo(neighborPosition);
			sum += m * (values[j] - values[i]) / d[j] * kernel.SecondDerivative(dist);
		}

		return sum;
	}

	Vector3D SPHSystemData3::LaplacianAt(size_t i, const ConstArrayAccessor1<Vector3D>& values) const
	{
		Vector3D sum;
		auto p = GetPositions();
		auto d = GetDensities();
		const auto& neighbors = GetNeighborLists()[i];
		Vector3D origin = p[i];
		SPHSpikyKernel3 kernel(m_kernelRadius);
		const double m = GetMass();

		for (size_t j : neighbors)
		{
			Vector3D neighborPosition = p[j];
			double dist = origin.DistanceTo(neighborPosition);
			sum += m * (values[j] - values[i]) / d[j] * kernel.SecondDerivative(dist);
		}

		return sum;
	}

	void SPHSystemData3::BuildNeighborSearcher()
	{
		ParticleSystemData3::BuildNeighborSearcher(m_kernelRadius);
	}

	void SPHSystemData3::BuildNeighborLists()
	{
		ParticleSystemData3::BuildNeighborLists(m_kernelRadius);
	}

	void SPHSystemData3::ComputeMass()
	{
		Array1<Vector3D> points;
		BccLatticePointGenerator pointsGenerator;
		BoundingBox3D sampleBound(
			Vector3D(-1.5 * m_kernelRadius, -1.5 * m_kernelRadius, -1.5 * m_kernelRadius),
			Vector3D(1.5 * m_kernelRadius, 1.5 * m_kernelRadius, 1.5 * m_kernelRadius));

		pointsGenerator.Generate(sampleBound, m_targetSpacing, &points);

		double maxNumberDensity = 0.0;
		SPHStdKernel3 kernel(m_kernelRadius);

		for (size_t i = 0; i < points.size(); ++i)
		{
			const Vector3D& point = points[i];
			double sum = 0.0;

			for (size_t j = 0; j < points.size(); ++j)
			{
				const Vector3D& neighborPoint = points[j];
				sum += kernel(neighborPoint.DistanceTo(point));
			}

			maxNumberDensity = std::max(maxNumberDensity, sum);
		}

		assert(maxNumberDensity > 0);

		double newMass = m_targetDensity / maxNumberDensity;

		ParticleSystemData3::SetMass(newMass);
	}

	void SPHSystemData3::Serialize(std::vector<uint8_t>* buffer) const
	{
		flatbuffers::FlatBufferBuilder builder(1024);
		flatbuffers::Offset<fbs::ParticleSystemData3> fbsParticleSystemData;

		SerializeParticleSystemData(&builder, &fbsParticleSystemData);

		auto fbsSPHSystemData = fbs::CreateSPHSystemData3(
			builder,
			fbsParticleSystemData,
			m_targetDensity,
			m_targetSpacing,
			m_kernelRadiusOverTargetSpacing,
			m_kernelRadius,
			m_pressureIdx,
			m_densityIdx);

		builder.Finish(fbsSPHSystemData);

		uint8_t* buf = builder.GetBufferPointer();
		size_t size = builder.GetSize();

		buffer->resize(size);
		memcpy(buffer->data(), buf, size);
	}

	void SPHSystemData3::Deserialize(const std::vector<uint8_t>& buffer)
	{
		auto fbsSPHSystemData = fbs::GetSPHSystemData3(buffer.data());

		auto base = fbsSPHSystemData->base();
		DeserializeParticleSystemData(base);

		// SPH specific
		m_targetDensity = fbsSPHSystemData->targetDensity();
		m_targetSpacing = fbsSPHSystemData->targetSpacing();
		m_kernelRadiusOverTargetSpacing = fbsSPHSystemData->kernelRadiusOverTargetSpacing();
		m_kernelRadius = fbsSPHSystemData->kernelRadius();
		m_pressureIdx = static_cast<size_t>(fbsSPHSystemData->pressureIdx());
		m_densityIdx = static_cast<size_t>(fbsSPHSystemData->densityIdx());
	}

	void SPHSystemData3::Set(const SPHSystemData3& other)
	{
		ParticleSystemData3::Set(other);

		m_targetDensity = other.m_targetDensity;
		m_targetSpacing = other.m_targetSpacing;
		m_kernelRadiusOverTargetSpacing = other.m_kernelRadiusOverTargetSpacing;
		m_kernelRadius = other.m_kernelRadius;
		m_densityIdx = other.m_densityIdx;
		m_pressureIdx = other.m_pressureIdx;
	}

	SPHSystemData3& SPHSystemData3::operator=(const SPHSystemData3& other)
	{
		Set(other);
		return *this;
	}
}