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
#include <Core/Particle/SPH/SPHStdKernel3.hpp>
#include <Core/Particle/SPH/SPHSystemData3.hpp>
#include <Core/PointGenerator/BccLatticePointGenerator.hpp>

#include <Flatbuffers/generated/SPHSystemData3_generated.h>

namespace CubbyFlow
{
SPHSystemData3::SPHSystemData3() : SPHSystemData3{ 0 }
{
    // Do nothing
}

SPHSystemData3::SPHSystemData3(size_t numberOfParticles)
    : ParticleSystemData3{ numberOfParticles }
{
    m_densityIdx = AddScalarData();
    m_pressureIdx = AddScalarData();

    SetTargetSpacing(m_targetSpacing);
}

SPHSystemData3::SPHSystemData3(const SPHSystemData3& other)
    : ParticleSystemData3{ other }
{
    Set(other);
}

SPHSystemData3& SPHSystemData3::operator=(const SPHSystemData3& other)
{
    Set(other);
    return *this;
}

void SPHSystemData3::SetRadius(double newRadius)
{
    // Interpret it as Setting target spacing
    SetTargetSpacing(newRadius);
}

void SPHSystemData3::SetMass(double newMass)
{
    const double incRatio = newMass / Mass();
    m_targetDensity *= incRatio;
    ParticleSystemData3::SetMass(newMass);
}

ConstArrayView1<double> SPHSystemData3::Densities() const
{
    return ScalarDataAt(m_densityIdx);
}

ArrayView1<double> SPHSystemData3::Densities()
{
    return ScalarDataAt(m_densityIdx);
}

ConstArrayView1<double> SPHSystemData3::Pressures() const
{
    return ScalarDataAt(m_pressureIdx);
}

ArrayView1<double> SPHSystemData3::Pressures()
{
    return ScalarDataAt(m_pressureIdx);
}

void SPHSystemData3::UpdateDensities()
{
    ArrayView<Vector3D, 1> p = Positions();
    ArrayView<double, 1> d = Densities();
    const double m = Mass();

    ParallelFor(ZERO_SIZE, NumberOfParticles(), [&](size_t i) {
        const double sum = SumOfKernelNearby(p[i]);
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
    SPHStdKernel3 kernel{ m_kernelRadius };

    NeighborSearcher()->ForEachNearbyPoint(
        origin, m_kernelRadius, [&](size_t, const Vector3D& neighborPosition) {
            const double dist = origin.DistanceTo(neighborPosition);
            sum += kernel(dist);
        });

    return sum;
}

double SPHSystemData3::Interpolate(const Vector3D& origin,
                                   const ConstArrayView1<double>& values) const
{
    double sum = 0.0;
    ConstArrayView1<double> d = Densities();
    SPHStdKernel3 kernel{ m_kernelRadius };
    const double m = Mass();

    NeighborSearcher()->ForEachNearbyPoint(
        origin, m_kernelRadius,
        [&](size_t i, const Vector3D& neighborPosition) {
            const double dist = origin.DistanceTo(neighborPosition);
            const double weight = m / d[i] * kernel(dist);

            sum += weight * values[i];
        });

    return sum;
}

Vector3D SPHSystemData3::Interpolate(
    const Vector3D& origin, const ConstArrayView1<Vector3D>& values) const
{
    Vector3D sum;
    ConstArrayView1<double> d = Densities();
    SPHStdKernel3 kernel{ m_kernelRadius };
    const double m = Mass();

    NeighborSearcher()->ForEachNearbyPoint(
        origin, m_kernelRadius,
        [&](size_t i, const Vector3D& neighborPosition) {
            const double dist = origin.DistanceTo(neighborPosition);
            const double weight = m / d[i] * kernel(dist);

            sum += weight * values[i];
        });

    return sum;
}

Vector3D SPHSystemData3::GradientAt(size_t i,
                                    const ConstArrayView1<double>& values) const
{
    Vector3D sum;
    const ConstArrayView1<Vector3D> p = Positions();
    const ConstArrayView1<double> d = Densities();
    const Array1<size_t>& neighbors = NeighborLists()[i];
    const Vector3D origin = p[i];
    const SPHSpikyKernel3 kernel{ m_kernelRadius };
    const double m = Mass();

    for (size_t j : neighbors)
    {
        Vector3D neighborPosition = p[j];
        const double dist = origin.DistanceTo(neighborPosition);

        if (dist > 0.0)
        {
            Vector3D dir = (neighborPosition - origin) / dist;
            sum += d[i] * m *
                   (values[i] / Square(d[i]) + values[j] / Square(d[j])) *
                   kernel.Gradient(dist, dir);
        }
    }

    return sum;
}

double SPHSystemData3::LaplacianAt(size_t i,
                                   const ConstArrayView1<double>& values) const
{
    double sum = 0.0;
    const ConstArrayView1<Vector3D> p = Positions();
    const ConstArrayView1<double> d = Densities();
    const Array1<size_t>& neighbors = NeighborLists()[i];
    const Vector3D origin = p[i];
    const SPHSpikyKernel3 kernel{ m_kernelRadius };
    const double m = Mass();

    for (size_t j : neighbors)
    {
        Vector3D neighborPosition = p[j];
        const double dist = origin.DistanceTo(neighborPosition);
        sum +=
            m * (values[j] - values[i]) / d[j] * kernel.SecondDerivative(dist);
    }

    return sum;
}

Vector3D SPHSystemData3::LaplacianAt(
    size_t i, const ConstArrayView1<Vector3D>& values) const
{
    Vector3D sum;
    const ConstArrayView1<Vector3D> p = Positions();
    const ConstArrayView1<double> d = Densities();
    const Array1<size_t>& neighbors = NeighborLists()[i];
    const Vector3D origin = p[i];
    const SPHSpikyKernel3 kernel{ m_kernelRadius };
    const double m = Mass();

    for (size_t j : neighbors)
    {
        Vector3D neighborPosition = p[j];
        const double dist = origin.DistanceTo(neighborPosition);
        sum +=
            m * (values[j] - values[i]) / d[j] * kernel.SecondDerivative(dist);
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
    const BccLatticePointGenerator pointsGenerator;
    const BoundingBox3D sampleBound{
        Vector3D{ -1.5 * m_kernelRadius, -1.5 * m_kernelRadius,
                  -1.5 * m_kernelRadius },
        Vector3D{ 1.5 * m_kernelRadius, 1.5 * m_kernelRadius,
                  1.5 * m_kernelRadius }
    };

    pointsGenerator.Generate(sampleBound, m_targetSpacing, &points);

    double maxNumberDensity = 0.0;
    const SPHStdKernel3 kernel{ m_kernelRadius };

    for (size_t i = 0; i < points.Length(); ++i)
    {
        const Vector3D& point1 = points[i];
        double sum = 0.0;

        for (const auto& point2 : points)
        {
            sum += kernel(point2.DistanceTo(point1));
        }

        maxNumberDensity = std::max(maxNumberDensity, sum);
    }

    assert(maxNumberDensity > 0);

    const double newMass = m_targetDensity / maxNumberDensity;

    ParticleSystemData3::SetMass(newMass);
}

void SPHSystemData3::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder{ 1024 };
    flatbuffers::Offset<fbs::ParticleSystemData3> fbsParticleSystemData;

    ParticleSystemData3::Serialize(*this, &builder, &fbsParticleSystemData);

    const flatbuffers::Offset<fbs::SPHSystemData3> fbsSPHSystemData =
        CreateSPHSystemData3(builder, fbsParticleSystemData, m_targetDensity,
                             m_targetSpacing, m_kernelRadiusOverTargetSpacing,
                             m_kernelRadius, m_pressureIdx, m_densityIdx);

    builder.Finish(fbsSPHSystemData);

    uint8_t* buf = builder.GetBufferPointer();
    const size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void SPHSystemData3::Deserialize(const std::vector<uint8_t>& buffer)
{
    const fbs::SPHSystemData3* fbsSPHSystemData =
        fbs::GetSPHSystemData3(buffer.data());

    const fbs::ParticleSystemData3* base = fbsSPHSystemData->base();
    ParticleSystemData3::Deserialize(base, *this);

    // SPH specific
    m_targetDensity = fbsSPHSystemData->targetDensity();
    m_targetSpacing = fbsSPHSystemData->targetSpacing();
    m_kernelRadiusOverTargetSpacing =
        fbsSPHSystemData->kernelRadiusOverTargetSpacing();
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
}  // namespace CubbyFlow