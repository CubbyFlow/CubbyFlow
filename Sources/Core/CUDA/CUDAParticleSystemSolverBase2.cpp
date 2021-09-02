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

#include <Core/CUDA/CUDAParticleSystemSolverBase2.hpp>

using namespace CubbyFlow;

CUDAParticleSystemSolverBase2::CUDAParticleSystemSolverBase2()
{
    m_particleSystemData = std::make_shared<CUDAParticleSystemData2>();
}

float CUDAParticleSystemSolverBase2::DragCoefficient() const
{
    return m_dragCoefficient;
}

void CUDAParticleSystemSolverBase2::SetDragCoefficient(float newDragCoefficient)
{
    m_dragCoefficient = std::max(newDragCoefficient, 0.0f);
}

float CUDAParticleSystemSolverBase2::RestitutionCoefficient() const
{
    return m_restitutionCoefficient;
}

void CUDAParticleSystemSolverBase2::SetRestitutionCoefficient(
    float newRestitutionCoefficient)
{
    m_restitutionCoefficient = Clamp(newRestitutionCoefficient, 0.0f, 1.0f);
}

const Vector2F& CUDAParticleSystemSolverBase2::Gravity() const
{
    return m_gravity;
}

void CUDAParticleSystemSolverBase2::SetGravity(const Vector2F& newGravity)
{
    m_gravity = newGravity;
}

CUDAParticleSystemData2* CUDAParticleSystemSolverBase2::ParticleSystemData()
{
    return m_particleSystemData.get();
}

const CUDAParticleSystemData2*
CUDAParticleSystemSolverBase2::ParticleSystemData() const
{
    return m_particleSystemData.get();
}

void CUDAParticleSystemSolverBase2::OnInitialize()
{
    // When initializing the solver, update the collider and emitter state as
    // well since they also affects the initial condition of the simulation.
    UpdateCollider(0.0);
    UpdateEmitter(0.0);
}

void CUDAParticleSystemSolverBase2::UpdateCollider(double timeStepInSeconds)
{
    UNUSED_VARIABLE(timeStepInSeconds);
}

void CUDAParticleSystemSolverBase2::UpdateEmitter(double timeStepInSeconds)
{
    UNUSED_VARIABLE(timeStepInSeconds);
}

#endif
