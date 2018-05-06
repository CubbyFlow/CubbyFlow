/*************************************************************************
> File Name: FLIPSolver3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D Fluid-Implicit Particle (FLIP) implementation.
> Created Time: 2017/09/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Solver/Hybrid/FLIP/FLIPSolver3.h>

namespace CubbyFlow
{
	FLIPSolver3::FLIPSolver3() :
		FLIPSolver3({ 1, 1, 1 }, { 1, 1, 1 }, { 0, 0, 0 })
	{
		// Do nothing
	}

	FLIPSolver3::FLIPSolver3(
		const Size3& resolution,
		const Vector3D& gridSpacing,
		const Vector3D& gridOrigin) :
		PICSolver3(resolution, gridSpacing, gridOrigin)
	{
		// Do nothing
	}

	FLIPSolver3::~FLIPSolver3()
	{
		// Do nothing
	}

	double FLIPSolver3::GetPICBlendingFactor() const
	{
		return m_picBlendingFactor;
	}

	void FLIPSolver3::SetPICBlendingFactor(double factor)
	{
		m_picBlendingFactor = std::clamp(factor, 0.0, 1.0);
	}

	void FLIPSolver3::TransferFromParticlesToGrids()
	{
		PICSolver3::TransferFromParticlesToGrids();

		// Store snapshot
		auto vel = GetGridSystemData()->GetVelocity();
		auto u = GetGridSystemData()->GetVelocity()->GetUConstAccessor();
		auto v = GetGridSystemData()->GetVelocity()->GetVConstAccessor();
		auto w = GetGridSystemData()->GetVelocity()->GetWConstAccessor();
		m_uDelta.Resize(u.size());
		m_vDelta.Resize(v.size());
		m_wDelta.Resize(w.size());

		vel->ParallelForEachUIndex([&](size_t i, size_t j, size_t k)
		{
			m_uDelta(i, j, k) = static_cast<float>(u(i, j, k));
		});
		vel->ParallelForEachVIndex([&](size_t i, size_t j, size_t k)
		{
			m_vDelta(i, j, k) = static_cast<float>(v(i, j, k));
		});
		vel->ParallelForEachWIndex([&](size_t i, size_t j, size_t k)
		{
			m_wDelta(i, j, k) = static_cast<float>(w(i, j, k));
		});
	}

	void FLIPSolver3::TransferFromGridsToParticles()
	{
		auto flow = GetGridSystemData()->GetVelocity();
		auto positions = GetParticleSystemData()->GetPositions();
		auto velocities = GetParticleSystemData()->GetVelocities();
		size_t numberOfParticles = GetParticleSystemData()->GetNumberOfParticles();

		// Compute delta
		flow->ParallelForEachUIndex([&](size_t i, size_t j, size_t k)
		{
			m_uDelta(i, j, k) = static_cast<float>(flow->GetU(i, j, k)) - m_uDelta(i, j, k);
		});

		flow->ParallelForEachVIndex([&](size_t i, size_t j, size_t k)
		{
			m_vDelta(i, j, k) = static_cast<float>(flow->GetV(i, j, k)) - m_vDelta(i, j, k);
		});

		flow->ParallelForEachWIndex([&](size_t i, size_t j, size_t k)
		{
			m_wDelta(i, j, k) = static_cast<float>(flow->GetW(i, j, k)) - m_wDelta(i, j, k);
		});

		LinearArraySampler3<float, float> uSampler(
			m_uDelta.ConstAccessor(),
			flow->GridSpacing().CastTo<float>(),
			flow->GetUOrigin().CastTo<float>());
		LinearArraySampler3<float, float> vSampler(
			m_vDelta.ConstAccessor(),
			flow->GridSpacing().CastTo<float>(),
			flow->GetVOrigin().CastTo<float>());
		LinearArraySampler3<float, float> wSampler(
			m_wDelta.ConstAccessor(),
			flow->GridSpacing().CastTo<float>(),
			flow->GetWOrigin().CastTo<float>());

		auto sampler = [uSampler, vSampler, wSampler](const Vector3D& x)
		{
			const auto xf = x.CastTo<float>();
			double u = uSampler(xf);
			double v = vSampler(xf);
			double w = wSampler(xf);
			return Vector3D(u, v, w);
		};

		// Transfer delta to the particles
		ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i)
		{
			Vector3D flipVel = velocities[i] + sampler(positions[i]);

			if (m_picBlendingFactor > 0.0) 
			{
				Vector3D picVel = flow->Sample(positions[i]);
				flipVel = Lerp(flipVel, picVel, m_picBlendingFactor);
			}

			velocities[i] = flipVel;
		});
	}

	FLIPSolver3::Builder FLIPSolver3::GetBuilder()
	{
		return Builder();
	}
	
	FLIPSolver3 FLIPSolver3::Builder::Build() const
	{
		return FLIPSolver3(m_resolution, GetGridSpacing(), m_gridOrigin);
	}

	FLIPSolver3Ptr FLIPSolver3::Builder::MakeShared() const
	{
		return std::shared_ptr<FLIPSolver3>(new FLIPSolver3(m_resolution, GetGridSpacing(), m_gridOrigin),
			[](FLIPSolver3* obj)
		{
			delete obj;
		});
	}
}