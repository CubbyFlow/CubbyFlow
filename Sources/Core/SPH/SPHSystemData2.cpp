/*************************************************************************
> File Name: SPHSystemData2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D SPH particle system data.
> Created Time: 2017/05/31
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/BoundingBox/BoundingBox2.h>
#include <Core/PointGenerator/TrianglePointGenerator.h>
#include <Core/SPH/SPHStdKernel2.h>
#include <Core/SPH/SPHSystemData2.h>

#include <Flatbuffers/generated/SPHSystemData2_generated.h>

namespace CubbyFlow
{
	SPHSystemData2::SPHSystemData2() :
		SPHSystemData2(0)
	{
		// Do nothing
	}

	SPHSystemData2::SPHSystemData2(size_t numberOfParticles)
		: ParticleSystemData2(numberOfParticles)
	{
		m_densityIdx = AddScalarData();
		m_pressureIdx = AddScalarData();

		SetTargetSpacing(m_targetSpacing);
	}

	SPHSystemData2::SPHSystemData2(const SPHSystemData2& other)
	{
		Set(other);
	}

	SPHSystemData2::~SPHSystemData2()
	{
		// Do nothing
	}

	void SPHSystemData2::SetRadius(double newRadius)
	{
		// Interpret it as Setting target spacing
		SetTargetSpacing(newRadius);
	}

	void SPHSystemData2::SetMass(double newMass)
	{
		double incRatio = newMass / GetMass();
		m_targetDensity *= incRatio;
		ParticleSystemData2::SetMass(newMass);
	}

	ConstArrayAccessor1<double> SPHSystemData2::GetDensities() const
	{
		return ScalarDataAt(m_densityIdx);
	}

	ArrayAccessor1<double> SPHSystemData2::GetDensities()
	{
		return ScalarDataAt(m_densityIdx);
	}

	ConstArrayAccessor1<double> SPHSystemData2::GetPressures() const
	{
		return ScalarDataAt(m_pressureIdx);
	}

	ArrayAccessor1<double> SPHSystemData2::GetPressures()
	{
		return ScalarDataAt(m_pressureIdx);
	}

	void SPHSystemData2::UpdateDensities()
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

	void SPHSystemData2::SetTargetDensity(double targetDensity)
	{
		m_targetDensity = targetDensity;

		ComputeMass();
	}

	double SPHSystemData2::GetTargetDensity() const
	{
		return m_targetDensity;
	}

	void SPHSystemData2::SetTargetSpacing(double spacing)
	{
		ParticleSystemData2::SetRadius(spacing);

		m_targetSpacing = spacing;
		m_kernelRadius = m_kernelRadiusOverTargetSpacing * m_targetSpacing;

		ComputeMass();
	}

	double SPHSystemData2::GetTargetSpacing() const
	{
		return m_targetSpacing;
	}

	void SPHSystemData2::SetRelativeKernelRadius(double relativeRadius)
	{
		m_kernelRadiusOverTargetSpacing = relativeRadius;
		m_kernelRadius = m_kernelRadiusOverTargetSpacing * m_targetSpacing;

		ComputeMass();
	}

	double SPHSystemData2::GetRelativeKernelRadius() const
	{
		return m_kernelRadiusOverTargetSpacing;
	}

	void SPHSystemData2::SetKernelRadius(double kernelRadius)
	{
		m_kernelRadius = kernelRadius;
		m_targetSpacing = kernelRadius / m_kernelRadiusOverTargetSpacing;

		ComputeMass();
	}

	double SPHSystemData2::GetKernelRadius() const
	{
		return m_kernelRadius;
	}

	double SPHSystemData2::SumOfKernelNearby(const Vector2D& origin) const
	{
		double sum = 0.0;
		SPHStdKernel2 kernel(m_kernelRadius);

		GetNeighborSearcher()->ForEachNearbyPoint(origin, m_kernelRadius,
			[&](size_t, const Vector2D& neighborPosition)
		{
			double dist = origin.DistanceTo(neighborPosition);
			sum += kernel(dist);
		});

		return sum;
	}

	double SPHSystemData2::Interpolate(const Vector2D& origin, const ConstArrayAccessor1<double>& values) const
	{
		double sum = 0.0;
		auto d = GetDensities();
		SPHStdKernel2 kernel(m_kernelRadius);
		const double m = GetMass();

		GetNeighborSearcher()->ForEachNearbyPoint(origin, m_kernelRadius,
			[&](size_t i, const Vector2D& neighborPosition)
		{
			double dist = origin.DistanceTo(neighborPosition);
			double weight = m / d[i] * kernel(dist);

			sum += weight * values[i];
		});

		return sum;
	}

	Vector2D SPHSystemData2::Interpolate(const Vector2D& origin, const ConstArrayAccessor1<Vector2D>& values) const
	{
		Vector2D sum;
		auto d = GetDensities();
		SPHStdKernel2 kernel(m_kernelRadius);
		const double m = GetMass();

		GetNeighborSearcher()->ForEachNearbyPoint(origin, m_kernelRadius,
			[&](size_t i, const Vector2D& neighborPosition)
		{
			double dist = origin.DistanceTo(neighborPosition);
			double weight = m / d[i] * kernel(dist);

			sum += weight * values[i];
		});

		return sum;
	}

	Vector2D SPHSystemData2::GradientAt(size_t i, const ConstArrayAccessor1<double>& values) const
	{
		Vector2D sum;
		auto p = GetPositions();
		auto d = GetDensities();
		const auto& neighbors = GetNeighborLists()[i];
		Vector2D origin = p[i];
		SPHSpikyKernel2 kernel(m_kernelRadius);
		const double m = GetMass();

		for (size_t j : neighbors)
		{
			Vector2D neighborPosition = p[j];
			double dist = origin.DistanceTo(neighborPosition);

			if (dist > 0.0)
			{
				Vector2D dir = (neighborPosition - origin) / dist;
				sum += d[i] * m * (values[i] / Square(d[i]) + values[j] / Square(d[j])) * kernel.Gradient(dist, dir);
			}
		}

		return sum;
	}

	double SPHSystemData2::LaplacianAt(size_t i, const ConstArrayAccessor1<double>& values) const
	{
		double sum = 0.0;
		auto p = GetPositions();
		auto d = GetDensities();
		const auto& neighbors = GetNeighborLists()[i];
		Vector2D origin = p[i];
		SPHSpikyKernel2 kernel(m_kernelRadius);
		const double m = GetMass();

		for (size_t j : neighbors)
		{
			Vector2D neighborPosition = p[j];
			double dist = origin.DistanceTo(neighborPosition);
			sum += m * (values[j] - values[i]) / d[j] * kernel.SecondDerivative(dist);
		}

		return sum;
	}

	Vector2D SPHSystemData2::LaplacianAt(size_t i, const ConstArrayAccessor1<Vector2D>& values) const
	{
		Vector2D sum;
		auto p = GetPositions();
		auto d = GetDensities();
		const auto& neighbors = GetNeighborLists()[i];
		Vector2D origin = p[i];
		SPHSpikyKernel2 kernel(m_kernelRadius);
		const double m = GetMass();

		for (size_t j : neighbors)
		{
			Vector2D neighborPosition = p[j];
			double dist = origin.DistanceTo(neighborPosition);
			sum += m * (values[j] - values[i]) / d[j] * kernel.SecondDerivative(dist);
		}

		return sum;
	}

	void SPHSystemData2::BuildNeighborSearcher()
	{
		ParticleSystemData2::BuildNeighborSearcher(m_kernelRadius);
	}

	void SPHSystemData2::BuildNeighborLists()
	{
		ParticleSystemData2::BuildNeighborLists(m_kernelRadius);
	}

	void SPHSystemData2::ComputeMass()
	{
		Array1<Vector2D> points;
		TrianglePointGenerator pointsGenerator;
		BoundingBox2D sampleBound(
			Vector2D(-1.5 * m_kernelRadius, -1.5 * m_kernelRadius),
			Vector2D(1.5 * m_kernelRadius, 1.5 * m_kernelRadius));

		pointsGenerator.Generate(sampleBound, m_targetSpacing, &points);

		double maxNumberDensity = 0.0;
		SPHStdKernel2 kernel(m_kernelRadius);

		for (size_t i = 0; i < points.size(); ++i)
		{
			const Vector2D& point = points[i];
			double sum = 0.0;

			for (size_t j = 0; j < points.size(); ++j)
			{
				const Vector2D& neighborPoint = points[j];
				sum += kernel(neighborPoint.DistanceTo(point));
			}

			maxNumberDensity = std::max(maxNumberDensity, sum);
		}

		assert(maxNumberDensity > 0);

		double newMass = m_targetDensity / maxNumberDensity;

		ParticleSystemData2::SetMass(newMass);
	}

	void SPHSystemData2::Serialize(std::vector<uint8_t>* buffer) const
	{
		flatbuffers::FlatBufferBuilder builder(1024);
		flatbuffers::Offset<fbs::ParticleSystemData2> fbsParticleSystemData;

		SerializeParticleSystemData(&builder, &fbsParticleSystemData);

		auto fbsSPHSystemData = fbs::CreateSPHSystemData2(
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

	void SPHSystemData2::Deserialize(const std::vector<uint8_t>& buffer)
	{
		auto fbsSPHSystemData = fbs::GetSPHSystemData2(buffer.data());

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

	void SPHSystemData2::Set(const SPHSystemData2& other)
	{
		ParticleSystemData2::Set(other);

		m_targetDensity = other.m_targetDensity;
		m_targetSpacing = other.m_targetSpacing;
		m_kernelRadiusOverTargetSpacing = other.m_kernelRadiusOverTargetSpacing;
		m_kernelRadius = other.m_kernelRadius;
		m_densityIdx = other.m_densityIdx;
		m_pressureIdx = other.m_pressureIdx;
	}

	SPHSystemData2& SPHSystemData2::operator=(const SPHSystemData2& other)
	{
		Set(other);
		return *this;
	}
}