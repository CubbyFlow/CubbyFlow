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

#include <Core/CUDA/CUDAParticleSystemSolverBase3.hpp>

using namespace CubbyFlow;

CUDAParticleSystemSolverBase3::CUDAParticleSystemSolverBase3()
{
    m_particleSystemData = std::make_shared<CUDAParticleSystemData3>();
}

float CUDAParticleSystemSolverBase3::DragCoefficient() const
{
    return m_dragCoefficient;
}

void CUDAParticleSystemSolverBase3::SetDragCoefficient(float newDragCoefficient)
{
    m_dragCoefficient = std::max(newDragCoefficient, 0.0f);
}

float CUDAParticleSystemSolverBase3::RestitutionCoefficient() const
{
    return m_restitutionCoefficient;
}

void CUDAParticleSystemSolverBase3::SetRestitutionCoefficient(
    float newRestitutionCoefficient)
{
    m_restitutionCoefficient = Clamp(newRestitutionCoefficient, 0.0f, 1.0f);
}

const Vector3F& CUDAParticleSystemSolverBase3::Gravity() const
{
    return m_gravity;
}

void CUDAParticleSystemSolverBase3::SetGravity(const Vector3F& newGravity)
{
    m_gravity = newGravity;
}

CUDAParticleSystemData3* CUDAParticleSystemSolverBase3::ParticleSystemData()
{
    return m_particleSystemData.get();
}

const CUDAParticleSystemData3*
CUDAParticleSystemSolverBase3::ParticleSystemData() const
{
    return m_particleSystemData.get();
}

void CUDAParticleSystemSolverBase3::OnInitialize()
{
    // When initializing the solver, update the collider and emitter state as
    // well since they also affects the initial condition of the simulation.
    UpdateCollider(0.0);
    UpdateEmitter(0.0);
}

void CUDAParticleSystemSolverBase3::UpdateCollider(double timeStepInSeconds)
{
    UNUSED_VARIABLE(timeStepInSeconds);
}

void CUDAParticleSystemSolverBase3::UpdateEmitter(double timeStepInSeconds)
{
    UNUSED_VARIABLE(timeStepInSeconds);
}

#endif
