/*************************************************************************
> File Name: ParticleSystemSolver2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Basic 2-D particle system solver.
> Created Time: 2017/05/21
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Array/ArrayUtils.hpp>
#include <Core/Field/ConstantVectorField2.h>
#include <Core/Solver/Particle/ParticleSystemSolver2.h>
#include <Core/Utils/Logging.h>
#include <Core/Utils/Parallel.h>
#include <Core/Utils/Timer.h>

#include <algorithm>

namespace CubbyFlow
{
	ParticleSystemSolver2::ParticleSystemSolver2() :
		ParticleSystemSolver2(1e-3, 1e-3)
	{
		// Do nothing
	}

	ParticleSystemSolver2::ParticleSystemSolver2(double radius, double mass)
	{
		m_particleSystemData = std::make_shared<ParticleSystemData2>();
		m_particleSystemData->SetRadius(radius);
		m_particleSystemData->SetMass(mass);
		m_wind = std::make_shared<ConstantVectorField2>(Vector2D());
	}

	ParticleSystemSolver2::~ParticleSystemSolver2()
	{
		// Do nothing
	}

	double ParticleSystemSolver2::GetDragCoefficient() const
	{
		return m_dragCoefficient;
	}

	void ParticleSystemSolver2::SetDragCoefficient(double newDragCoefficient)
	{
		m_dragCoefficient = std::max(newDragCoefficient, 0.0);
	}

	double ParticleSystemSolver2::GetRestitutionCoefficient() const
	{
		return m_restitutionCoefficient;
	}

	void ParticleSystemSolver2::SetRestitutionCoefficient(double newRestitutionCoefficient)
	{
		m_restitutionCoefficient = std::clamp(newRestitutionCoefficient, 0.0, 1.0);
	}

	const Vector2D& ParticleSystemSolver2::GetGravity() const
	{
		return m_gravity;
	}

	void ParticleSystemSolver2::SetGravity(const Vector2D& newGravity)
	{
		m_gravity = newGravity;
	}

	const ParticleSystemData2Ptr& ParticleSystemSolver2::GetParticleSystemData() const
	{
		return m_particleSystemData;
	}

	const Collider2Ptr& ParticleSystemSolver2::GetCollider() const
	{
		return m_collider;
	}

	void ParticleSystemSolver2::SetCollider(const Collider2Ptr& newCollider)
	{
		m_collider = newCollider;
	}

	const ParticleEmitter2Ptr& ParticleSystemSolver2::GetEmitter() const
	{
		return m_emitter;
	}

	void ParticleSystemSolver2::SetEmitter(const ParticleEmitter2Ptr& newEmitter)
	{
		m_emitter = newEmitter;
		newEmitter->SetTarget(m_particleSystemData);
	}

	const VectorField2Ptr& ParticleSystemSolver2::GetWind() const
	{
		return m_wind;
	}

	void ParticleSystemSolver2::SetWind(const VectorField2Ptr& newWind)
	{
		m_wind = newWind;
	}

	void ParticleSystemSolver2::OnInitialize()
	{
		// When initializing the solver, update the collider and emitter state as
		// well since they also affects the initial condition of the simulation.
		Timer timer;
		UpdateCollider(0.0);
		CUBBYFLOW_INFO << "Update collider took "
			<< timer.DurationInSeconds() << " seconds";

		timer.Reset();
		UpdateEmitter(0.0);
		CUBBYFLOW_INFO << "Update emitter took "
			<< timer.DurationInSeconds() << " seconds";
	}

	void ParticleSystemSolver2::OnAdvanceTimeStep(double timeStepInSeconds)
	{
		BeginAdvanceTimeStep(timeStepInSeconds);

		Timer timer;
		AccumulateForces(timeStepInSeconds);
		CUBBYFLOW_INFO << "Accumulating forces took "
			<< timer.DurationInSeconds() << " seconds";

		timer.Reset();
		TimeIntegration(timeStepInSeconds);
		CUBBYFLOW_INFO << "Time integration took "
			<< timer.DurationInSeconds() << " seconds";

		timer.Reset();
		ResolveCollision();
		CUBBYFLOW_INFO << "Resolving collision took "
			<< timer.DurationInSeconds() << " seconds";

		EndAdvanceTimeStep(timeStepInSeconds);
	}

	void ParticleSystemSolver2::AccumulateForces(double timeStepInSeconds)
	{
		UNUSED_VARIABLE(timeStepInSeconds);

		// Add external forces
		AccumulateExternalForces();
	}

	void ParticleSystemSolver2::BeginAdvanceTimeStep(double timeStepInSeconds)
	{
		// Clear forces
		auto forces = m_particleSystemData->GetForces();
		SetRange1(forces.size(), Vector2D(), &forces);

		// Update collider and emitter
		Timer timer;
		UpdateCollider(timeStepInSeconds);
		CUBBYFLOW_INFO << "Update collider took "
			<< timer.DurationInSeconds() << " seconds";

		timer.Reset();
		UpdateEmitter(timeStepInSeconds);
		CUBBYFLOW_INFO << "Update emitter took "
			<< timer.DurationInSeconds() << " seconds";

		// Allocate buffers
		size_t n = m_particleSystemData->GetNumberOfParticles();
		m_newPositions.Resize(n);
		m_newVelocities.Resize(n);

		OnBeginAdvanceTimeStep(timeStepInSeconds);
	}

	void ParticleSystemSolver2::EndAdvanceTimeStep(double timeStepInSeconds)
	{
		// Update data
		size_t n = m_particleSystemData->GetNumberOfParticles();
		auto positions = m_particleSystemData->GetPositions();
		auto velocities = m_particleSystemData->GetVelocities();

		ParallelFor(ZERO_SIZE, n, [&](size_t i)
		{
			positions[i] = m_newPositions[i];
			velocities[i] = m_newVelocities[i];
		});

		OnEndAdvanceTimeStep(timeStepInSeconds);
	}

	void ParticleSystemSolver2::OnBeginAdvanceTimeStep(double timeStepInSeconds)
	{
		UNUSED_VARIABLE(timeStepInSeconds);
	}

	void ParticleSystemSolver2::OnEndAdvanceTimeStep(double timeStepInSeconds)
	{
		UNUSED_VARIABLE(timeStepInSeconds);
	}

	void ParticleSystemSolver2::ResolveCollision()
	{
		ResolveCollision(m_newPositions.Accessor(), m_newVelocities.Accessor());
	}

	void ParticleSystemSolver2::ResolveCollision(
		ArrayAccessor1<Vector2D> newPositions,
		ArrayAccessor1<Vector2D> newVelocities)
	{
		if (m_collider != nullptr)
		{
			size_t numberOfParticles = m_particleSystemData->GetNumberOfParticles();
			const double radius = m_particleSystemData->GetRadius();

			ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i)
			{
				m_collider->ResolveCollision(
					radius,
					m_restitutionCoefficient,
					&newPositions[i],
					&newVelocities[i]);
			});
		}
	}

	void ParticleSystemSolver2::SetParticleSystemData(const ParticleSystemData2Ptr& newParticles)
	{
		m_particleSystemData = newParticles;
	}

	void ParticleSystemSolver2::AccumulateExternalForces()
	{
		size_t n = m_particleSystemData->GetNumberOfParticles();
		auto forces = m_particleSystemData->GetForces();
		auto velocities = m_particleSystemData->GetVelocities();
		auto positions = m_particleSystemData->GetPositions();
		const double mass = m_particleSystemData->GetMass();

		ParallelFor(ZERO_SIZE, n, [&](size_t i)
		{
			// Gravity
			Vector2D force = mass * m_gravity;

			// Wind forces
			Vector2D relativeVel = velocities[i] - m_wind->Sample(positions[i]);
			force += -m_dragCoefficient * relativeVel;

			forces[i] += force;
		});
	}

	void ParticleSystemSolver2::TimeIntegration(double timeStepInSeconds)
	{
		size_t n = m_particleSystemData->GetNumberOfParticles();
		auto forces = m_particleSystemData->GetForces();
		auto velocities = m_particleSystemData->GetVelocities();
		auto positions = m_particleSystemData->GetPositions();
		const double mass = m_particleSystemData->GetMass();

		ParallelFor(ZERO_SIZE, n, [&](size_t i)
		{
			// Integrate velocity first
			Vector2D& newVelocity = m_newVelocities[i];
			newVelocity = velocities[i] + timeStepInSeconds * forces[i] / mass;

			// Integrate position.
			Vector2D& newPosition = m_newPositions[i];
			newPosition = positions[i] + timeStepInSeconds * newVelocity;
		});
	}

	void ParticleSystemSolver2::UpdateCollider(double timeStepInSeconds) const
	{
		if (m_collider != nullptr)
		{
			m_collider->Update(GetCurrentTimeInSeconds(), timeStepInSeconds);
		}
	}

	void ParticleSystemSolver2::UpdateEmitter(double timeStepInSeconds) const
	{
		if (m_emitter != nullptr)
		{
			m_emitter->Update(GetCurrentTimeInSeconds(), timeStepInSeconds);
		}
	}

	ParticleSystemSolver2::Builder ParticleSystemSolver2::GetBuilder()
	{
		return Builder();
	}
	
	ParticleSystemSolver2 ParticleSystemSolver2::Builder::Build() const
	{
		return ParticleSystemSolver2(m_radius, m_mass);
	}

	ParticleSystemSolver2Ptr ParticleSystemSolver2::Builder::MakeShared() const
	{
		return std::shared_ptr<ParticleSystemSolver2>(
			new ParticleSystemSolver2(m_radius, m_mass),
			[](ParticleSystemSolver2* obj)
		{
			delete obj;
		});
	}
}