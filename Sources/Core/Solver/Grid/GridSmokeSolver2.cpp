/*************************************************************************
> File Name: GridSmokeSolver2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D grid-based smoke solver.
> Created Time: 2017/08/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Grid/CellCenteredScalarGrid2.h>
#include <Core/Solver/Grid/GridSmokeSolver2.h>

namespace CubbyFlow
{
	GridSmokeSolver2::GridSmokeSolver2() :
		GridSmokeSolver2({ 1, 1 }, { 1, 1 }, { 0, 0 })
	{
		// Do nothing
	}

	GridSmokeSolver2::GridSmokeSolver2(
		const Size2& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin) :
		GridFluidSolver2(resolution, gridSpacing, gridOrigin)
	{
		auto grids = GetGridSystemData();
		m_smokeDensityDataID = grids->AddAdvectableScalarData(std::make_shared<CellCenteredScalarGrid2::Builder>(), 0.0);
		m_temperatureDataID = grids->AddAdvectableScalarData(std::make_shared<CellCenteredScalarGrid2::Builder>(), 0.0);
	}

	GridSmokeSolver2::~GridSmokeSolver2()
	{
		// Do nothing
	}

	double GridSmokeSolver2::GetSmokeDiffusionCoefficient() const
	{
		return m_smokeDiffusionCoefficient;
	}

	void GridSmokeSolver2::SetSmokeDiffusionCoefficient(double newValue)
	{
		m_smokeDiffusionCoefficient = std::max(newValue, 0.0);
	}

	double GridSmokeSolver2::GetTemperatureDiffusionCoefficient() const
	{
		return m_temperatureDiffusionCoefficient;
	}

	void GridSmokeSolver2::SetTemperatureDiffusionCoefficient(double newValue)
	{
		m_temperatureDiffusionCoefficient = std::max(newValue, 0.0);
	}

	double GridSmokeSolver2::GetBuoyancySmokeDensityFactor() const
	{
		return m_buoyancySmokeDensityFactor;
	}

	void GridSmokeSolver2::SetBuoyancySmokeDensityFactor(double newValue)
	{
		m_buoyancySmokeDensityFactor = newValue;
	}

	double GridSmokeSolver2::GetBuoyancyTemperatureFactor() const
	{
		return m_buoyancyTemperatureFactor;
	}

	void GridSmokeSolver2::SetBuoyancyTemperatureFactor(double newValue)
	{
		m_buoyancyTemperatureFactor = newValue;
	}

	double GridSmokeSolver2::GetSmokeDecayFactor() const
	{
		return m_smokeDecayFactor;
	}

	void GridSmokeSolver2::SetSmokeDecayFactor(double newValue)
	{
		m_smokeDecayFactor = std::clamp(newValue, 0.0, 1.0);
	}

	double GridSmokeSolver2::GetTemperatureDecayFactor() const
	{
		return m_temperatureDecayFactor;
	}

	void GridSmokeSolver2::SetTemperatureDecayFactor(double newValue)
	{
		m_temperatureDecayFactor = std::clamp(newValue, 0.0, 1.0);
	}

	ScalarGrid2Ptr GridSmokeSolver2::GetSmokeDensity() const
	{
		return GetGridSystemData()->GetAdvectableScalarDataAt(m_smokeDensityDataID);
	}

	ScalarGrid2Ptr GridSmokeSolver2::GetTemperature() const
	{
		return GetGridSystemData()->GetAdvectableScalarDataAt(m_temperatureDataID);
	}

	void GridSmokeSolver2::OnEndAdvanceTimeStep(double timeIntervalInSeconds)
	{
		ComputeDiffusion(timeIntervalInSeconds);
	}

	void GridSmokeSolver2::ComputeExternalForces(double timeIntervalInSeconds)
	{
		ComputeBuoyancyForce(timeIntervalInSeconds);
	}

	void GridSmokeSolver2::ComputeDiffusion(double timeIntervalInSeconds)
	{
		if (GetDiffusionSolver() != nullptr)
		{
			if (m_smokeDiffusionCoefficient > std::numeric_limits<double>::epsilon())
			{
				auto den = GetSmokeDensity();
				const auto den0 = std::dynamic_pointer_cast<CellCenteredScalarGrid2>(den->Clone());

				GetDiffusionSolver()->Solve(
					*den0,
					m_smokeDiffusionCoefficient,
					timeIntervalInSeconds,
					den.get(),
					*GetColliderSDF());
				ExtrapolateIntoCollider(den.get());
			}

			if (m_temperatureDiffusionCoefficient > std::numeric_limits<double>::epsilon())
			{
				auto temp = GetSmokeDensity();
				const auto temp0 = std::dynamic_pointer_cast<CellCenteredScalarGrid2>(temp->Clone());

				GetDiffusionSolver()->Solve(
					*temp0,
					m_temperatureDiffusionCoefficient,
					timeIntervalInSeconds,
					temp.get(),
					*GetColliderSDF());
				ExtrapolateIntoCollider(temp.get());
			}
		}

		auto den = GetSmokeDensity();
		den->ParallelForEachDataPointIndex([&](size_t i, size_t j)
		{
			(*den)(i, j) *= 1.0 - m_smokeDecayFactor;
		});
		auto temp = GetTemperature();
		temp->ParallelForEachDataPointIndex([&](size_t i, size_t j)
		{
			(*temp)(i, j) *= 1.0 - m_temperatureDecayFactor;
		});
	}

	void GridSmokeSolver2::ComputeBuoyancyForce(double timeIntervalInSeconds)
	{
		const auto grids = GetGridSystemData();
		auto vel = grids->GetVelocity();

		Vector2D up(0, 1);
		if (GetGravity().LengthSquared() > std::numeric_limits<double>::epsilon())
		{
			up = -GetGravity().Normalized();
		}

		if (std::abs(m_buoyancySmokeDensityFactor) > std::numeric_limits<double>::epsilon() ||
			std::abs(m_buoyancyTemperatureFactor) > std::numeric_limits<double>::epsilon())
		{
			auto den = GetSmokeDensity();
			auto temp = GetTemperature();

			double tAmb = 0.0;
			temp->ForEachCellIndex([&](size_t i, size_t j)
			{
				tAmb += (*temp)(i, j);
			});

			tAmb /= static_cast<double>(temp->Resolution().x * temp->Resolution().y);

			auto u = vel->GetUAccessor();
			auto v = vel->GetVAccessor();
			auto uPos = vel->GetUPosition();
			auto vPos = vel->GetVPosition();

			if (std::abs(up.x) > std::numeric_limits<double>::epsilon())
			{
				vel->ParallelForEachUIndex([&](size_t i, size_t j)
				{
					Vector2D pt = uPos(i, j);
					double fBuoy =
						m_buoyancySmokeDensityFactor * den->Sample(pt) +
						m_buoyancyTemperatureFactor * (temp->Sample(pt) - tAmb);
					u(i, j) += timeIntervalInSeconds * fBuoy * up.x;
				});
			}

			if (std::abs(up.y) > std::numeric_limits<double>::epsilon())
			{
				vel->ParallelForEachVIndex([&](size_t i, size_t j)
				{
					Vector2D pt = vPos(i, j);
					double fBuoy =
						m_buoyancySmokeDensityFactor * den->Sample(pt) +
						m_buoyancyTemperatureFactor * (temp->Sample(pt) - tAmb);
					v(i, j) += timeIntervalInSeconds * fBuoy * up.y;
				});
			}

			ApplyBoundaryCondition();
		}
	}

	GridSmokeSolver2::Builder GridSmokeSolver2::GetBuilder()
	{
		return Builder();
	}
	
	GridSmokeSolver2 GridSmokeSolver2::Builder::Build() const
	{
		return GridSmokeSolver2(m_resolution, GetGridSpacing(), m_gridOrigin);
	}

	GridSmokeSolver2Ptr GridSmokeSolver2::Builder::MakeShared() const
	{
		return std::shared_ptr<GridSmokeSolver2>(
			new GridSmokeSolver2(m_resolution, GetGridSpacing(), m_gridOrigin),
			[](GridSmokeSolver2* obj)
		{
			delete obj;
		});
	}
}