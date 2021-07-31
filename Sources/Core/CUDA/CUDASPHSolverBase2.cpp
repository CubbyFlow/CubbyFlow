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

#include <Core/CUDA/CUDASPHSolverBase2.hpp>

using namespace CubbyFlow;

static double TIME_STEP_LIMIT_BY_SPEED_FACTOR = 0.4;
static double TIME_STEP_LIMIT_BY_FORCE_FACTOR = 0.25;

CUDASPHSolverBase2::CUDASPHSolverBase2()
{
    m_sphSystemData = std::make_shared<CUDASPHSystemData2>();
    m_forcesIdx = m_sphSystemData->AddVectorData();
    m_smoothedVelIdx = m_sphSystemData->AddVectorData();

    SetIsUsingFixedSubTimeSteps(false);
}

float CUDASPHSolverBase2::NegativePressureScale() const
{
    return m_negativePressureScale;
}

void CUDASPHSolverBase2::SetNegativePressureScale(
    float newNegativePressureScale)
{
    m_negativePressureScale = newNegativePressureScale;
}

float CUDASPHSolverBase2::ViscosityCoefficient() const
{
    return m_viscosityCoefficient;
}

void CUDASPHSolverBase2::SetViscosityCoefficient(float newViscosityCoefficient)
{
    m_viscosityCoefficient = newViscosityCoefficient;
}

float CUDASPHSolverBase2::PseudoViscosityCoefficient() const
{
    return m_pseudoViscosityCoefficient;
}

void CUDASPHSolverBase2::SetPseudoViscosityCoefficient(
    float newPseudoViscosityCoefficient)
{
    m_pseudoViscosityCoefficient = newPseudoViscosityCoefficient;
}

float CUDASPHSolverBase2::SpeedOfSound() const
{
    return m_speedOfSound;
}

void CUDASPHSolverBase2::SetSpeedOfSound(float newSpeedOfSound)
{
    m_speedOfSound =
        std::max(newSpeedOfSound, std::numeric_limits<float>::epsilon());
}

float CUDASPHSolverBase2::TimeStepLimitScale() const
{
    return m_timeStepLimitScale;
}

void CUDASPHSolverBase2::SetTimeStepLimitScale(float newScale)
{
    m_timeStepLimitScale = std::max(newScale, 0.0f);
}

const BoundingBox2F& CUDASPHSolverBase2::Container() const
{
    return m_container;
}

void CUDASPHSolverBase2::SetContainer(const BoundingBox2F& cont)
{
    m_container = cont;
}

CUDAParticleSystemData2* CUDASPHSolverBase2::ParticleSystemData()
{
    return m_sphSystemData.get();
}

const CUDAParticleSystemData2* CUDASPHSolverBase2::ParticleSystemData() const
{
    return m_sphSystemData.get();
}

CUDASPHSystemData2* CUDASPHSolverBase2::SPHSystemData()
{
    return m_sphSystemData.get();
}

const CUDASPHSystemData2* CUDASPHSolverBase2::SPHSystemData() const
{
    return m_sphSystemData.get();
}

unsigned int CUDASPHSolverBase2::GetNumberOfSubTimeSteps(
    double timeIntervalInSeconds) const
{
    const auto particles = SPHSystemData();
    const auto kernelRadius = static_cast<double>(particles->KernelRadius());
    const auto mass = static_cast<double>(particles->Mass());

    const double maxForceMagnitude = GRAVITY;

    const double timeStepLimitBySpeed = TIME_STEP_LIMIT_BY_SPEED_FACTOR *
                                        kernelRadius /
                                        static_cast<double>(m_speedOfSound);
    const double timeStepLimitByForce =
        TIME_STEP_LIMIT_BY_FORCE_FACTOR *
        std::sqrt(kernelRadius * mass / maxForceMagnitude);

    const double desiredTimeStep =
        static_cast<double>(TimeStepLimitScale()) *
        std::min(timeStepLimitBySpeed, timeStepLimitByForce);

    return static_cast<unsigned int>(
        std::ceil(timeIntervalInSeconds / desiredTimeStep));
}

CUDAArrayView1<float2> CUDASPHSolverBase2::Forces() const
{
    return m_sphSystemData->VectorDataAt(m_forcesIdx);
}

CUDAArrayView1<float2> CUDASPHSolverBase2::SmoothedVelocities() const
{
    return m_sphSystemData->VectorDataAt(m_smoothedVelIdx);
}

#endif
