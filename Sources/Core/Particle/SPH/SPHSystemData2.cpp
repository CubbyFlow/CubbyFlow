// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Geometry/BoundingBox.hpp>
#include <Core/Particle/SPH/SPHStdKernel2.hpp>
#include <Core/Particle/SPH/SPHSystemData2.hpp>
#include <Core/PointGenerator/TrianglePointGenerator.hpp>

#include <Flatbuffers/generated/SPHSystemData2_generated.h>

namespace CubbyFlow
{
SPHSystemData2::SPHSystemData2() : SPHSystemData2{ 0 }
{
    // Do nothing
}

SPHSystemData2::SPHSystemData2(size_t numberOfParticles)
    : ParticleSystemData2{ numberOfParticles }
{
    m_densityIdx = AddScalarData();
    m_pressureIdx = AddScalarData();

    SetTargetSpacing(m_targetSpacing);
}

SPHSystemData2::SPHSystemData2(const SPHSystemData2& other)
    : ParticleSystemData2{ other }
{
    Set(other);
}

SPHSystemData2& SPHSystemData2::operator=(const SPHSystemData2& other)
{
    Set(other);
    return *this;
}

void SPHSystemData2::SetRadius(double newRadius)
{
    // Interpret it as Setting target spacing
    SetTargetSpacing(newRadius);
}

void SPHSystemData2::SetMass(double newMass)
{
    const double incRatio = newMass / GetMass();
    m_targetDensity *= incRatio;
    ParticleSystemData2::SetMass(newMass);
}

ConstArrayView1<double> SPHSystemData2::Densities() const
{
    return ScalarDataAt(m_densityIdx);
}

ArrayView1<double> SPHSystemData2::Densities()
{
    return ScalarDataAt(m_densityIdx);
}

ConstArrayView1<double> SPHSystemData2::Pressures() const
{
    return ScalarDataAt(m_pressureIdx);
}

ArrayView1<double> SPHSystemData2::Pressures()
{
    return ScalarDataAt(m_pressureIdx);
}

void SPHSystemData2::UpdateDensities()
{
    ArrayView<Vector2D, 1> p = Positions();
    ArrayView<double, 1> d = Densities();
    const double m = GetMass();

    ParallelFor(ZERO_SIZE, GetNumberOfParticles(), [&](size_t i) {
        const double sum = SumOfKernelNearby(p[i]);
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
    SPHStdKernel2 kernel{ m_kernelRadius };

    GetNeighborSearcher()->ForEachNearbyPoint(
        origin, m_kernelRadius, [&](size_t, const Vector2D& neighborPosition) {
            const double dist = origin.DistanceTo(neighborPosition);
            sum += kernel(dist);
        });

    return sum;
}

double SPHSystemData2::Interpolate(const Vector2D& origin,
                                   const ConstArrayView1<double>& values) const
{
    double sum = 0.0;
    ConstArrayView1<double> d = Densities();
    SPHStdKernel2 kernel{ m_kernelRadius };
    const double m = GetMass();

    GetNeighborSearcher()->ForEachNearbyPoint(
        origin, m_kernelRadius,
        [&](size_t i, const Vector2D& neighborPosition) {
            const double dist = origin.DistanceTo(neighborPosition);
            const double weight = m / d[i] * kernel(dist);

            sum += weight * values[i];
        });

    return sum;
}

Vector2D SPHSystemData2::Interpolate(
    const Vector2D& origin, const ConstArrayView1<Vector2D>& values) const
{
    Vector2D sum;
    ConstArrayView1<double> d = Densities();
    SPHStdKernel2 kernel{ m_kernelRadius };
    const double m = GetMass();

    GetNeighborSearcher()->ForEachNearbyPoint(
        origin, m_kernelRadius,
        [&](size_t i, const Vector2D& neighborPosition) {
            const double dist = origin.DistanceTo(neighborPosition);
            const double weight = m / d[i] * kernel(dist);

            sum += weight * values[i];
        });

    return sum;
}

Vector2D SPHSystemData2::GradientAt(size_t i,
                                    const ConstArrayView1<double>& values) const
{
    Vector2D sum;
    const ConstArrayView1<Vector2D> p = Positions();
    const ConstArrayView1<double> d = Densities();
    const std::vector<size_t>& neighbors = GetNeighborLists()[i];
    const Vector2D origin = p[i];
    const SPHSpikyKernel2 kernel{ m_kernelRadius };
    const double m = GetMass();

    for (size_t j : neighbors)
    {
        Vector2D neighborPosition = p[j];
        const double dist = origin.DistanceTo(neighborPosition);

        if (dist > 0.0)
        {
            Vector2D dir = (neighborPosition - origin) / dist;
            sum += d[i] * m *
                   (values[i] / Square(d[i]) + values[j] / Square(d[j])) *
                   kernel.Gradient(dist, dir);
        }
    }

    return sum;
}

double SPHSystemData2::LaplacianAt(size_t i,
                                   const ConstArrayView1<double>& values) const
{
    double sum = 0.0;
    const ConstArrayView1<Vector2D> p = Positions();
    const ConstArrayView1<double> d = Densities();
    const std::vector<size_t>& neighbors = GetNeighborLists()[i];
    const Vector2D origin = p[i];
    const SPHSpikyKernel2 kernel{ m_kernelRadius };
    const double m = GetMass();

    for (size_t j : neighbors)
    {
        Vector2D neighborPosition = p[j];
        const double dist = origin.DistanceTo(neighborPosition);
        sum +=
            m * (values[j] - values[i]) / d[j] * kernel.SecondDerivative(dist);
    }

    return sum;
}

Vector2D SPHSystemData2::LaplacianAt(
    size_t i, const ConstArrayView1<Vector2D>& values) const
{
    Vector2D sum;
    const ConstArrayView1<Vector2D> p = Positions();
    const ConstArrayView1<double> d = Densities();
    const std::vector<size_t>& neighbors = GetNeighborLists()[i];
    const Vector2D origin = p[i];
    const SPHSpikyKernel2 kernel{ m_kernelRadius };
    const double m = GetMass();

    for (size_t j : neighbors)
    {
        Vector2D neighborPosition = p[j];
        const double dist = origin.DistanceTo(neighborPosition);
        sum +=
            m * (values[j] - values[i]) / d[j] * kernel.SecondDerivative(dist);
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
    const TrianglePointGenerator pointsGenerator;
    const BoundingBox2D sampleBound{
        Vector2D{ -1.5 * m_kernelRadius, -1.5 * m_kernelRadius },
        Vector2D{ 1.5 * m_kernelRadius, 1.5 * m_kernelRadius }
    };

    pointsGenerator.Generate(sampleBound, m_targetSpacing, &points);

    double maxNumberDensity = 0.0;
    const SPHStdKernel2 kernel{ m_kernelRadius };

    for (size_t i = 0; i < points.Length(); ++i)
    {
        const Vector2D& point1 = points[i];
        double sum = 0.0;

        for (const auto& point2 : points)
        {
            sum += kernel(point2.DistanceTo(point1));
        }

        maxNumberDensity = std::max(maxNumberDensity, sum);
    }

    assert(maxNumberDensity > 0);

    const double newMass = m_targetDensity / maxNumberDensity;

    ParticleSystemData2::SetMass(newMass);
}

void SPHSystemData2::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder{ 1024 };
    flatbuffers::Offset<fbs::ParticleSystemData2> fbsParticleSystemData;

    SerializeParticleSystemData(&builder, &fbsParticleSystemData);

    const flatbuffers::Offset<fbs::SPHSystemData2> fbsSPHSystemData =
        CreateSPHSystemData2(builder, fbsParticleSystemData, m_targetDensity,
                             m_targetSpacing, m_kernelRadiusOverTargetSpacing,
                             m_kernelRadius, m_pressureIdx, m_densityIdx);

    builder.Finish(fbsSPHSystemData);

    uint8_t* buf = builder.GetBufferPointer();
    const size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void SPHSystemData2::Deserialize(const std::vector<uint8_t>& buffer)
{
    const fbs::SPHSystemData2* fbsSPHSystemData =
        fbs::GetSPHSystemData2(buffer.data());

    const fbs::ParticleSystemData2* base = fbsSPHSystemData->base();
    DeserializeParticleSystemData(base);

    // SPH specific
    m_targetDensity = fbsSPHSystemData->targetDensity();
    m_targetSpacing = fbsSPHSystemData->targetSpacing();
    m_kernelRadiusOverTargetSpacing =
        fbsSPHSystemData->kernelRadiusOverTargetSpacing();
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
}  // namespace CubbyFlow