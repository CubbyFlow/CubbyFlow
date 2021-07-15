// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#if CUBBYFLOW_USE_CUDA

#include <Core/CUDA/CUDASPHSystemData3.hpp>
#include <Core/Particle/SPHKernels.hpp>
#include <Core/PointGenerator/BccLatticePointGenerator.hpp>

namespace CubbyFlow
{
CUDASPHSystemData3::CUDASPHSystemData3() : CUDASPHSystemData3{ 0 }
{
    // Do nothing
}

CUDASPHSystemData3::CUDASPHSystemData3(size_t numberOfParticles)
    : CUDAParticleSystemData3{ numberOfParticles }
{
    m_densityIdx = AddFloatData();
    m_pressureIdx = AddFloatData();

    SetTargetSpacing(m_targetSpacing);
}

CUDASPHSystemData3::CUDASPHSystemData3(const CUDASPHSystemData3& other)
    : CUDAParticleSystemData3{ other },
      m_targetDensity(other.m_targetDensity),
      m_targetSpacing(other.m_targetSpacing),
      m_kernelRadiusOverTargetSpacing(other.m_kernelRadiusOverTargetSpacing),
      m_kernelRadius(other.m_kernelRadius),
      m_pressureIdx(other.m_pressureIdx),
      m_densityIdx(other.m_densityIdx)
{
    // Do nothing
}

CUDASPHSystemData3::CUDASPHSystemData3(CUDASPHSystemData3&& other) noexcept
    : CUDAParticleSystemData3{ std::move(other) },
      m_targetDensity(
          std::exchange(other.m_targetDensity, WATER_DENSITY_FLOAT)),
      m_targetSpacing(std::exchange(other.m_targetSpacing, 0.1f)),
      m_kernelRadiusOverTargetSpacing(
          std::exchange(other.m_kernelRadiusOverTargetSpacing, 1.8f)),
      m_kernelRadius(std::exchange(other.m_kernelRadius, 0.1f)),
      m_pressureIdx(std::exchange(other.m_pressureIdx, 0)),
      m_densityIdx(std::exchange(other.m_densityIdx, 0))
{
    // Do nothing
}

CUDASPHSystemData3& CUDASPHSystemData3::operator=(
    const CUDASPHSystemData3& other)
{
    m_targetDensity = other.m_targetDensity;
    m_targetSpacing = other.m_targetSpacing;
    m_kernelRadiusOverTargetSpacing = other.m_kernelRadiusOverTargetSpacing;
    m_kernelRadius = other.m_kernelRadius;
    m_densityIdx = other.m_densityIdx;
    m_pressureIdx = other.m_pressureIdx;
    CUDAParticleSystemData3::operator=(other);
    return *this;
}

CUDASPHSystemData3& CUDASPHSystemData3::operator=(
    CUDASPHSystemData3&& other) noexcept
{
    m_targetDensity = std::exchange(other.m_targetDensity, WATER_DENSITY_FLOAT);
    m_targetSpacing = std::exchange(other.m_targetSpacing, 0.1f);
    m_kernelRadiusOverTargetSpacing =
        std::exchange(other.m_kernelRadiusOverTargetSpacing, 1.8f);
    m_kernelRadius = std::exchange(other.m_kernelRadius, 0.1f);
    m_pressureIdx = std::exchange(other.m_pressureIdx, 0);
    m_densityIdx = std::exchange(other.m_densityIdx, 0);
    CUDAParticleSystemData3::operator=(std::move(other));
    return *this;
}

ConstCUDAArrayView1<float> CUDASPHSystemData3::Densities() const
{
    return FloatDataAt(m_densityIdx);
}

CUDAArrayView1<float> CUDASPHSystemData3::Densities()
{
    return FloatDataAt(m_densityIdx);
}

ConstCUDAArrayView1<float> CUDASPHSystemData3::Pressures() const
{
    return FloatDataAt(m_pressureIdx);
}

CUDAArrayView1<float> CUDASPHSystemData3::Pressures()
{
    return FloatDataAt(m_pressureIdx);
}

float CUDASPHSystemData3::TargetDensity() const
{
    return m_targetDensity;
}

void CUDASPHSystemData3::SetTargetDensity(float targetDensity)
{
    m_targetDensity = targetDensity;

    ComputeMass();
}

float CUDASPHSystemData3::TargetSpacing() const
{
    return m_targetSpacing;
}

void CUDASPHSystemData3::SetTargetSpacing(float spacing)
{
    m_targetSpacing = spacing;
    m_kernelRadius = m_kernelRadiusOverTargetSpacing * m_targetSpacing;

    ComputeMass();
}

float CUDASPHSystemData3::RelativeKernelRadius() const
{
    return m_kernelRadiusOverTargetSpacing;
}

void CUDASPHSystemData3::SetRelativeKernelRadius(float relativeRadius)
{
    m_kernelRadiusOverTargetSpacing = relativeRadius;
    m_kernelRadius = m_kernelRadiusOverTargetSpacing * m_targetSpacing;

    ComputeMass();
}

float CUDASPHSystemData3::KernelRadius() const
{
    return m_kernelRadius;
}

void CUDASPHSystemData3::SetKernelRadius(float kernelRadius)
{
    m_kernelRadius = kernelRadius;
    m_targetSpacing = kernelRadius / m_kernelRadiusOverTargetSpacing;

    ComputeMass();
}

float CUDASPHSystemData3::Mass() const
{
    return m_mass;
}

void CUDASPHSystemData3::BuildNeighborSearcher()
{
    CUDAParticleSystemData3::BuildNeighborSearcher(m_kernelRadius);
}

void CUDASPHSystemData3::Set(const CUDASPHSystemData3& other)
{
    CUDAParticleSystemData3::Set(other);

    m_targetDensity = other.m_targetDensity;
    m_targetSpacing = other.m_targetSpacing;
    m_kernelRadiusOverTargetSpacing = other.m_kernelRadiusOverTargetSpacing;
    m_kernelRadius = other.m_kernelRadius;
    m_densityIdx = other.m_densityIdx;
    m_pressureIdx = other.m_pressureIdx;
}

void CUDASPHSystemData3::ComputeMass()
{
    Array1<Vector3D> points;
    BccLatticePointGenerator pointsGenerator;
    BoundingBox3D sampleBound(
        Vector3D{ -1.5 * m_kernelRadius, -1.5 * m_kernelRadius,
                  -1.5 * m_kernelRadius },
        Vector3D{ 1.5 * m_kernelRadius, 1.5 * m_kernelRadius,
                  1.5 * m_kernelRadius });

    pointsGenerator.Generate(sampleBound, m_targetSpacing, &points);

    double maxNumberDensity = 0.0;
    SPHStdKernel3 kernel(m_kernelRadius);

    for (size_t i = 0; i < points.Length(); ++i)
    {
        const Vector3D& point = points[i];
        double sum = 0.0;

        for (size_t j = 0; j < points.Length(); ++j)
        {
            const Vector3D& neighborPoint = points[j];
            sum += kernel(neighborPoint.DistanceTo(point));
        }

        maxNumberDensity = std::max(maxNumberDensity, sum);
    }

    assert(maxNumberDensity > 0);

    m_mass = static_cast<float>(m_targetDensity / maxNumberDensity);
}
}  // namespace CubbyFlow

#endif
