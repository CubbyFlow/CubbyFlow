/*************************************************************************
> File Name: GridSmokeSolver3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D grid-based smoke solver.
> Created Time: 2017/08/18
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Grid/CellCenteredScalarGrid3.h>
#include <Core/Solver/Grid/GridSmokeSolver3.h>

namespace CubbyFlow
{
	GridSmokeSolver3::GridSmokeSolver3() :
		GridSmokeSolver3({ 1, 1, 1 }, { 1, 1, 1 }, { 0, 0, 0 })
	{
		// Do nothing
	}

	GridSmokeSolver3::GridSmokeSolver3(
		const Size3& resolution,
		const Vector3D& gridSpacing,
		const Vector3D& gridOrigin) :
		GridFluidSolver3(resolution, gridSpacing, gridOrigin)
	{
		auto grids = GetGridSystemData();
		m_smokeDensityDataID = grids->AddAdvectableScalarData(std::make_shared<CellCenteredScalarGrid3::Builder>(), 0.0);
		m_temperatureDataID = grids->AddAdvectableScalarData(std::make_shared<CellCenteredScalarGrid3::Builder>(), 0.0);
	}

	GridSmokeSolver3::~GridSmokeSolver3()
	{
		// Do nothing
	}

	double GridSmokeSolver3::GetSmokeDiffusionCoefficient() const
	{
		return m_smokeDiffusionCoefficient;
	}

	void GridSmokeSolver3::SetSmokeDiffusionCoefficient(double newValue)
	{
		m_smokeDiffusionCoefficient = std::max(newValue, 0.0);
	}

	double GridSmokeSolver3::GetTemperatureDiffusionCoefficient() const
	{
		return m_temperatureDiffusionCoefficient;
	}

	void GridSmokeSolver3::SetTemperatureDiffusionCoefficient(double newValue)
	{
		m_temperatureDiffusionCoefficient = std::max(newValue, 0.0);
	}

	double GridSmokeSolver3::GetBuoyancySmokeDensityFactor() const
	{
		return m_buoyancySmokeDensityFactor;
	}

	void GridSmokeSolver3::SetBuoyancySmokeDensityFactor(double newValue)
	{
		m_buoyancySmokeDensityFactor = newValue;
	}

	double GridSmokeSolver3::GetBuoyancyTemperatureFactor() const
	{
		return m_buoyancyTemperatureFactor;
	}

	void GridSmokeSolver3::SetBuoyancyTemperatureFactor(double newValue)
	{
		m_buoyancyTemperatureFactor = newValue;
	}

	double GridSmokeSolver3::GetSmokeDecayFactor() const
	{
		return m_smokeDecayFactor;
	}

	void GridSmokeSolver3::SetSmokeDecayFactor(double newValue)
	{
		m_smokeDecayFactor = std::clamp(newValue, 0.0, 1.0);
	}

	double GridSmokeSolver3::GetTemperatureDecayFactor() const
	{
		return m_temperatureDecayFactor;
	}

	void GridSmokeSolver3::SetTemperatureDecayFactor(double newValue)
	{
		m_temperatureDecayFactor = std::clamp(newValue, 0.0, 1.0);
	}

	ScalarGrid3Ptr GridSmokeSolver3::GetSmokeDensity() const
	{
		return GetGridSystemData()->GetAdvectableScalarDataAt(m_smokeDensityDataID);
	}

	ScalarGrid3Ptr GridSmokeSolver3::GetTemperature() const
	{
		return GetGridSystemData()->GetAdvectableScalarDataAt(m_temperatureDataID);
	}

	void GridSmokeSolver3::OnEndAdvanceTimeStep(double timeIntervalInSeconds)
	{
		ComputeDiffusion(timeIntervalInSeconds);
	}

	void GridSmokeSolver3::ComputeExternalForces(double timeIntervalInSeconds)
	{
		ComputeBuoyancyForce(timeIntervalInSeconds);
	}

	void GridSmokeSolver3::ComputeDiffusion(double timeIntervalInSeconds)
	{
		if (GetDiffusionSolver() != nullptr)
		{
			if (m_smokeDiffusionCoefficient > std::numeric_limits<double>::epsilon())
			{
				auto den = GetSmokeDensity();
				const auto den0 = std::dynamic_pointer_cast<CellCenteredScalarGrid3>(den->Clone());

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
				const auto temp0 = std::dynamic_pointer_cast<CellCenteredScalarGrid3>(temp->Clone());

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
		den->ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k)
		{
			(*den)(i, j, k) *= 1.0 - m_smokeDecayFactor;
		});
		auto temp = GetTemperature();
		temp->ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k)
		{
			(*temp)(i, j, k) *= 1.0 - m_temperatureDecayFactor;
		});
	}

	void GridSmokeSolver3::ComputeBuoyancyForce(double timeIntervalInSeconds)
	{
		const auto grids = GetGridSystemData();
		auto vel = grids->GetVelocity();

		Vector3D up(0, 0, 1);
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
			temp->ForEachCellIndex([&](size_t i, size_t j, size_t k)
			{
				tAmb += (*temp)(i, j, k);
			});

			tAmb /= static_cast<double>(temp->Resolution().x * temp->Resolution().y * temp->Resolution().z);

			auto u = vel->GetUAccessor();
			auto v = vel->GetVAccessor();
			auto w = vel->GetWAccessor();
			auto uPos = vel->GetUPosition();
			auto vPos = vel->GetVPosition();
			auto wPos = vel->GetWPosition();

			if (std::abs(up.x) > std::numeric_limits<double>::epsilon())
			{
				vel->ParallelForEachUIndex([&](size_t i, size_t j, size_t k)
				{
					Vector3D pt = uPos(i, j, k);
					double fBuoy =
						m_buoyancySmokeDensityFactor * den->Sample(pt) +
						m_buoyancyTemperatureFactor * (temp->Sample(pt) - tAmb);
					u(i, j, k) += timeIntervalInSeconds * fBuoy * up.x;
				});
			}

			if (std::abs(up.y) > std::numeric_limits<double>::epsilon())
			{
				vel->ParallelForEachVIndex([&](size_t i, size_t j, size_t k)
				{
					Vector3D pt = vPos(i, j, k);
					double fBuoy =
						m_buoyancySmokeDensityFactor * den->Sample(pt) +
						m_buoyancyTemperatureFactor * (temp->Sample(pt) - tAmb);
					v(i, j, k) += timeIntervalInSeconds * fBuoy * up.y;
				});
			}

			if (std::abs(up.z) > std::numeric_limits<double>::epsilon())
			{
				vel->ParallelForEachWIndex([&](size_t i, size_t j, size_t k)
				{
					Vector3D pt = wPos(i, j, k);
					double fBuoy =
						m_buoyancySmokeDensityFactor * den->Sample(pt) +
						m_buoyancyTemperatureFactor * (temp->Sample(pt) - tAmb);
					w(i, j, k) += timeIntervalInSeconds * fBuoy * up.z;
				});
			}

			ApplyBoundaryCondition();
		}
	}

	GridSmokeSolver3::Builder GridSmokeSolver3::GetBuilder()
	{
		return Builder();
	}

	GridSmokeSolver3 GridSmokeSolver3::Builder::Build() const
	{
		return GridSmokeSolver3(m_resolution, GetGridSpacing(), m_gridOrigin);
	}

	GridSmokeSolver3Ptr GridSmokeSolver3::Builder::MakeShared() const
	{
		return std::shared_ptr<GridSmokeSolver3>(
			new GridSmokeSolver3(m_resolution, GetGridSpacing(), m_gridOrigin),
			[](GridSmokeSolver3* obj)
		{
			delete obj;
		});
	}
}