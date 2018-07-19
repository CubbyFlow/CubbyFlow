/*************************************************************************
> File Name: SemiLagrangian3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Implementation of 3-D semi-Lagrangian advection solver.
> Created Time: 2017/08/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <SemiLagrangian/SemiLagrangian3.h>

namespace CubbyFlow
{
	SemiLagrangian3::SemiLagrangian3()
	{
		// Do nothing
	}

	SemiLagrangian3::~SemiLagrangian3()
	{
		// Do nothing
	}

	void SemiLagrangian3::Advect(
		const ScalarGrid3& input,
		const VectorField3& flow,
		double dt,
		ScalarGrid3* output,
		const ScalarField3& boundarySDF)
	{
		auto inputSamplerFunc = GetScalarSamplerFunc(input);
		double h = std::min(output->GridSpacing().x, output->GridSpacing().y);

		auto inputDataPos = input.GetDataPosition();
		auto outputDataPos = output->GetDataPosition();
		auto outputDataAcc = output->GetDataAccessor();

		output->ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k)
		{
			if (boundarySDF.Sample(inputDataPos(i, j, k)) > 0.0)
			{
				Vector3D pt = BackTrace(flow, dt, h, outputDataPos(i, j, k), boundarySDF);
				outputDataAcc(i, j, k) = inputSamplerFunc(pt);
			}
		});
	}

	void SemiLagrangian3::Advect(
		const CollocatedVectorGrid3& input,
		const VectorField3& flow,
		double dt,
		CollocatedVectorGrid3* output,
		const ScalarField3& boundarySDF)
	{
		auto inputSamplerFunc = GetVectorSamplerFunc(input);
		double h = std::min(output->GridSpacing().x, output->GridSpacing().y);

		auto inputDataPos = input.GetDataPosition();
		auto outputDataPos = output->GetDataPosition();
		auto outputDataAcc = output->GetDataAccessor();

		output->ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k)
		{
			if (boundarySDF.Sample(inputDataPos(i, j, k)) > 0.0)
			{
				Vector3D pt = BackTrace(flow, dt, h, outputDataPos(i, j, k), boundarySDF);
				outputDataAcc(i, j, k) = inputSamplerFunc(pt);
			}
		});
	}

	void SemiLagrangian3::Advect(
		const FaceCenteredGrid3& input,
		const VectorField3& flow,
		double dt,
		FaceCenteredGrid3* output,
		const ScalarField3& boundarySDF)
	{
		auto inputSamplerFunc = GetVectorSamplerFunc(input);
		double h = std::min(output->GridSpacing().x, output->GridSpacing().y);

		auto uSourceDataPos = input.GetUPosition();
		auto uTargetDataPos = output->GetUPosition();
		auto uTargetDataAcc = output->GetUAccessor();

		output->ParallelForEachUIndex([&](size_t i, size_t j, size_t k)
		{
			if (boundarySDF.Sample(uSourceDataPos(i, j, k)) > 0.0)
			{
				Vector3D pt = BackTrace(flow, dt, h, uTargetDataPos(i, j, k), boundarySDF);
				uTargetDataAcc(i, j, k) = inputSamplerFunc(pt).x;
			}
		});

		auto vSourceDataPos = input.GetVPosition();
		auto vTargetDataPos = output->GetVPosition();
		auto vTargetDataAcc = output->GetVAccessor();

		output->ParallelForEachVIndex([&](size_t i, size_t j, size_t k)
		{
			if (boundarySDF.Sample(vSourceDataPos(i, j, k)) > 0.0)
			{
				Vector3D pt = BackTrace(flow, dt, h, vTargetDataPos(i, j, k), boundarySDF);
				vTargetDataAcc(i, j, k) = inputSamplerFunc(pt).y;
			}
		});

		auto wTargetDataPos = output->GetWPosition();
		auto wTargetDataAcc = output->GetWAccessor();
		auto wSourceDataPos = input.GetWPosition();

		output->ParallelForEachWIndex([&](size_t i, size_t j, size_t k)
		{
			if (boundarySDF.Sample(wSourceDataPos(i, j, k)) > 0.0)
			{
				Vector3D pt = BackTrace(flow, dt, h, wTargetDataPos(i, j, k), boundarySDF);
				wTargetDataAcc(i, j, k) = inputSamplerFunc(pt).z;
			}
		});
	}

	Vector3D SemiLagrangian3::BackTrace(
		const VectorField3& flow,
		double dt,
		double h,
		const Vector3D& startPt,
		const ScalarField3& boundarySDF) const
	{
		double remainingT = dt;
		Vector3D pt0 = startPt;
		Vector3D pt1 = startPt;

		while (remainingT > std::numeric_limits<double>::epsilon())
		{
			// Adaptive time-stepping
			Vector3D vel0 = flow.Sample(pt0);
			double numSubSteps = std::max(std::ceil(vel0.Length() * remainingT / h), 1.0);
			dt = remainingT / numSubSteps;

			// Mid-point rule
			Vector3D midPt = pt0 - 0.5 * dt * vel0;
			Vector3D midVel = flow.Sample(midPt);
			pt1 = pt0 - dt * midVel;

			// Boundary handling
			double phi0 = boundarySDF.Sample(pt0);
			double phi1 = boundarySDF.Sample(pt1);

			if (phi0 * phi1 < 0.0)
			{
				double w = std::fabs(phi1) / (std::fabs(phi0) + std::fabs(phi1));
				pt1 = w * pt0 + (1.0 - w) * pt1;
				break;
			}

			remainingT -= dt;
			pt0 = pt1;
		}

		return pt1;
	}

	std::function<double(const Vector3D&)> SemiLagrangian3::GetScalarSamplerFunc(const ScalarGrid3& input) const
	{
		return input.Sampler();
	}

	std::function<Vector3D(const Vector3D&)> SemiLagrangian3::GetVectorSamplerFunc(const CollocatedVectorGrid3& input) const
	{
		return input.Sampler();
	}

	std::function<Vector3D(const Vector3D&)> SemiLagrangian3::GetVectorSamplerFunc(const FaceCenteredGrid3& input) const
	{
		return input.Sampler();
	}
}