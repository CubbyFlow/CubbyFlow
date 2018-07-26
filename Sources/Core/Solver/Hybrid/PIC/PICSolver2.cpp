/*************************************************************************
> File Name: PICSolver2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D Particle-in-Cell (PIC) implementation.
> Created Time: 2017/09/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Array/ArrayUtils.h>
#include <Core/Grid/CellCenteredScalarGrid2.h>
#include <Core/Solver/Hybrid/PIC/PICSolver2.h>
#include <Core/Utils/Logging.h>
#include <Core/Utils/Timer.h>

namespace CubbyFlow
{
	PICSolver2::PICSolver2() :
		PICSolver2({ 1, 1 }, { 1, 1 }, { 0, 0 })
	{
		// Do nothing
	}

	PICSolver2::PICSolver2(
		const Size2& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin) :
		GridFluidSolver2(resolution, gridSpacing, gridOrigin)
	{
		auto grids = GetGridSystemData();
		m_signedDistanceFieldID = grids->AddScalarData(std::make_shared<CellCenteredScalarGrid2::Builder>(), std::numeric_limits<double>::max());
		m_particles = std::make_shared<ParticleSystemData2>();
	}

	PICSolver2::~PICSolver2()
	{
		// Do nothing
	}

	ScalarGrid2Ptr PICSolver2::GetSignedDistanceField() const
	{
		return GetGridSystemData()->GetScalarDataAt(m_signedDistanceFieldID);
	}

	const ParticleSystemData2Ptr& PICSolver2::GetParticleSystemData() const
	{
		return m_particles;
	}

	const ParticleEmitter2Ptr& PICSolver2::GetParticleEmitter() const
	{
		return m_particleEmitter;
	}

	void PICSolver2::SetParticleEmitter(const ParticleEmitter2Ptr& newEmitter)
	{
		m_particleEmitter = newEmitter;
		newEmitter->SetTarget(m_particles);
	}

	void PICSolver2::OnInitialize()
	{
		GridFluidSolver2::OnInitialize();

		Timer timer;
		UpdateParticleEmitter(0.0);
		CUBBYFLOW_INFO << "Update particle emitter took "
			<< timer.DurationInSeconds() << " seconds";
	}

	void PICSolver2::OnBeginAdvanceTimeStep(double timeIntervalInSeconds)
	{
		Timer timer;
		UpdateParticleEmitter(timeIntervalInSeconds);
		CUBBYFLOW_INFO << "Update particle emitter took "
			<< timer.DurationInSeconds() << " seconds";

		CUBBYFLOW_INFO << "Number of PIC-type particles: "
			<< m_particles->GetNumberOfParticles();

		timer.Reset();
		TransferFromParticlesToGrids();
		CUBBYFLOW_INFO << "TransferFromParticlesToGrids took "
			<< timer.DurationInSeconds() << " seconds";

		timer.Reset();
		BuildSignedDistanceField();
		CUBBYFLOW_INFO << "BuildSignedDistanceField took "
			<< timer.DurationInSeconds() << " seconds";

		timer.Reset();
		ExtrapolateVelocityToAir();
		CUBBYFLOW_INFO << "ExtrapolateVelocityToAir took "
			<< timer.DurationInSeconds() << " seconds";

		ApplyBoundaryCondition();
	}

	void PICSolver2::ComputeAdvection(double timeIntervalInSeconds)
	{
		Timer timer;
		ExtrapolateVelocityToAir();
		CUBBYFLOW_INFO << "ExtrapolateVelocityToAir took "
			<< timer.DurationInSeconds() << " seconds";

		ApplyBoundaryCondition();

		timer.Reset();
		TransferFromGridsToParticles();
		CUBBYFLOW_INFO << "TransferFromGridsToParticles took "
			<< timer.DurationInSeconds() << " seconds";

		timer.Reset();
		MoveParticles(timeIntervalInSeconds);
		CUBBYFLOW_INFO << "MoveParticles took "
			<< timer.DurationInSeconds() << " seconds";
	}

	ScalarField2Ptr PICSolver2::GetFluidSDF() const
	{
		return GetSignedDistanceField();
	}

	void PICSolver2::TransferFromParticlesToGrids()
	{
		auto flow = GetGridSystemData()->GetVelocity();
		auto positions = m_particles->GetPositions();
		auto velocities = m_particles->GetVelocities();
		size_t numberOfParticles = m_particles->GetNumberOfParticles();

		// Clear velocity to zero
		flow->Fill(Vector2D());

		// Weighted-average velocity
		auto u = flow->GetUAccessor();
		auto v = flow->GetVAccessor();
		Array2<double> uWeight(u.size());
		Array2<double> vWeight(v.size());
		m_uMarkers.Resize(u.size());
		m_vMarkers.Resize(v.size());
		m_uMarkers.Set(0);
		m_vMarkers.Set(0);
		LinearArraySampler2<double, double> uSampler(
			flow->GetUConstAccessor(),
			flow->GridSpacing(),
			flow->GetUOrigin());
		LinearArraySampler2<double, double> vSampler(
			flow->GetVConstAccessor(),
			flow->GridSpacing(),
			flow->GetVOrigin());
		for (size_t i = 0; i < numberOfParticles; ++i)
		{
			std::array<Point2UI, 4> indices;
			std::array<double, 4> weights;

			uSampler.GetCoordinatesAndWeights(positions[i], &indices, &weights);
			for (int j = 0; j < 4; ++j)
			{
				u(indices[j]) += velocities[i].x * weights[j];
				uWeight(indices[j]) += weights[j];
				m_uMarkers(indices[j]) = 1;
			}

			vSampler.GetCoordinatesAndWeights(positions[i], &indices, &weights);
			for (int j = 0; j < 4; ++j)
			{
				v(indices[j]) += velocities[i].y * weights[j];
				vWeight(indices[j]) += weights[j];
				m_vMarkers(indices[j]) = 1;
			}
		}

		uWeight.ParallelForEachIndex([&](size_t i, size_t j)
		{
			if (uWeight(i, j) > 0.0)
			{
				u(i, j) /= uWeight(i, j);
			}
		});
		vWeight.ParallelForEachIndex([&](size_t i, size_t j)
		{
			if (vWeight(i, j) > 0.0)
			{
				v(i, j) /= vWeight(i, j);
			}
		});
	}

	void PICSolver2::TransferFromGridsToParticles()
	{
		auto flow = GetGridSystemData()->GetVelocity();
		auto positions = m_particles->GetPositions();
		auto velocities = m_particles->GetVelocities();
		size_t numberOfParticles = m_particles->GetNumberOfParticles();

		ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i)
		{
			velocities[i] = flow->Sample(positions[i]);
		});
	}

	void PICSolver2::MoveParticles(double timeIntervalInSeconds)
	{
		auto flow = GetGridSystemData()->GetVelocity();
		auto positions = m_particles->GetPositions();
		auto velocities = m_particles->GetVelocities();
		size_t numberOfParticles = m_particles->GetNumberOfParticles();
		int domainBoundaryFlag = GetClosedDomainBoundaryFlag();
		BoundingBox2D boundingBox = flow->BoundingBox();

		ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i)
		{
			Vector2D pt0 = positions[i];
			Vector2D pt1 = pt0;
			Vector2D vel = velocities[i];

			// Adaptive time-stepping
			unsigned int numSubSteps = static_cast<unsigned int>(std::max(GetMaxCFL(), 1.0));
			double dt = timeIntervalInSeconds / numSubSteps;
			for (unsigned int t = 0; t < numSubSteps; ++t)
			{
				Vector2D vel0 = flow->Sample(pt0);

				// Mid-point rule
				Vector2D midPt = pt0 + 0.5 * dt * vel0;
				Vector2D midVel = flow->Sample(midPt);
				pt1 = pt0 + dt * midVel;

				pt0 = pt1;
			}

			if ((domainBoundaryFlag & DIRECTION_LEFT) && pt1.x <= boundingBox.lowerCorner.x)
			{
				pt1.x = boundingBox.lowerCorner.x;
				vel.x = 0.0;
			}
			if ((domainBoundaryFlag & DIRECTION_RIGHT) && pt1.x >= boundingBox.upperCorner.x)
			{
				pt1.x = boundingBox.upperCorner.x;
				vel.x = 0.0;
			}
			if ((domainBoundaryFlag & DIRECTION_DOWN) && pt1.y <= boundingBox.lowerCorner.y)
			{
				pt1.y = boundingBox.lowerCorner.y;
				vel.y = 0.0;
			}
			if ((domainBoundaryFlag & DIRECTION_UP) && pt1.y >= boundingBox.upperCorner.y)
			{
				pt1.y = boundingBox.upperCorner.y;
				vel.y = 0.0;
			}

			positions[i] = pt1;
			velocities[i] = vel;
		});

		Collider2Ptr col = GetCollider();
		if (col != nullptr)
		{
			ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i)
			{
				col->ResolveCollision(0.0, 0.0, &positions[i], &velocities[i]);
			});
		}
	}

	void PICSolver2::ExtrapolateVelocityToAir() const
	{
		auto vel = GetGridSystemData()->GetVelocity();
		auto u = vel->GetUAccessor();
		auto v = vel->GetVAccessor();

		unsigned int depth = static_cast<unsigned int>(std::ceil(GetMaxCFL()));
		ExtrapolateToRegion(vel->GetUConstAccessor(), m_uMarkers, depth, u);
		ExtrapolateToRegion(vel->GetVConstAccessor(), m_vMarkers, depth, v);
	}

	void PICSolver2::BuildSignedDistanceField()
	{
		auto sdf = GetSignedDistanceField();
		auto sdfPos = sdf->GetDataPosition();
		double maxH = std::max(sdf->GridSpacing().x, sdf->GridSpacing().y);
		double radius = 1.2 * maxH / std::sqrt(2.0);

		m_particles->BuildNeighborSearcher(2 * radius);
		auto searcher = m_particles->GetNeighborSearcher();
		sdf->ParallelForEachDataPointIndex([&](size_t i, size_t j)
		{
			Vector2D pt = sdfPos(i, j);
			double minDist = 2.0 * radius;
			
			searcher->ForEachNearbyPoint(pt, 2.0 * radius, [&](size_t, const Vector2D& x)
			{
				minDist = std::min(minDist, pt.DistanceTo(x));
			});
			(*sdf)(i, j) = minDist - radius;
		});

		ExtrapolateIntoCollider(sdf.get());
	}

	void PICSolver2::UpdateParticleEmitter(double timeIntervalInSeconds) const
	{
		if (m_particleEmitter != nullptr)
		{
			m_particleEmitter->Update(GetCurrentTimeInSeconds(), timeIntervalInSeconds);
		}
	}

	PICSolver2::Builder PICSolver2::GetBuilder()
	{
		return Builder();
	}
	
	PICSolver2 PICSolver2::Builder::Build() const
	{
		return PICSolver2(m_resolution, GetGridSpacing(), m_gridOrigin);
	}

	PICSolver2Ptr PICSolver2::Builder::MakeShared() const
	{
		return std::shared_ptr<PICSolver2>(new PICSolver2(m_resolution, GetGridSpacing(), m_gridOrigin),
			[](PICSolver2* obj)
		{
			delete obj;
		});
	}
}