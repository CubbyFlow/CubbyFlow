/*************************************************************************
> File Name: SemiLagrangian2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Implementation of 2-D semi-Lagrangian advection solver.
> Created Time: 2017/08/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/SemiLagrangian/SemiLagrangian2.h>

namespace CubbyFlow
{
	SemiLagrangian2::SemiLagrangian2()
	{
		// Do nothing
	}

	SemiLagrangian2::~SemiLagrangian2()
	{
		// Do nothing
	}

	void SemiLagrangian2::Advect(
		const ScalarGrid2& input,
		const VectorField2& flow,
		double dt,
		ScalarGrid2* output,
		const ScalarField2& boundarySDF)
	{
		auto inputSamplerFunc = GetScalarSamplerFunc(input);
		double h = std::min(output->GridSpacing().x, output->GridSpacing().y);

		auto inputDataPos = input.GetDataPosition();
		auto outputDataPos = output->GetDataPosition();
		auto outputDataAcc = output->GetDataAccessor();
		
		output->ParallelForEachDataPointIndex([&](size_t i, size_t j)
		{
			if (boundarySDF.Sample(inputDataPos(i, j)) > 0.0)
			{
				Vector2D pt = BackTrace(flow, dt, h, outputDataPos(i, j), boundarySDF);
				outputDataAcc(i, j) = inputSamplerFunc(pt);
			}
		});
	}

	void SemiLagrangian2::Advect(
		const CollocatedVectorGrid2& input,
		const VectorField2& flow,
		double dt,
		CollocatedVectorGrid2* output,
		const ScalarField2& boundarySDF)
	{
		auto inputSamplerFunc = GetVectorSamplerFunc(input);
		double h = std::min(output->GridSpacing().x, output->GridSpacing().y);

		auto inputDataPos = input.GetDataPosition();
		auto outputDataPos = output->GetDataPosition();
		auto outputDataAcc = output->GetDataAccessor();

		output->ParallelForEachDataPointIndex([&](size_t i, size_t j)
		{
			if (boundarySDF.Sample(inputDataPos(i, j)) > 0.0)
			{
				Vector2D pt = BackTrace(flow, dt, h, outputDataPos(i, j), boundarySDF);
				outputDataAcc(i, j) = inputSamplerFunc(pt);
			}
		});
	}

	void SemiLagrangian2::Advect(
		const FaceCenteredGrid2& input,
		const VectorField2& flow,
		double dt,
		FaceCenteredGrid2* output,
		const ScalarField2& boundarySDF)
	{
		auto inputSamplerFunc = GetVectorSamplerFunc(input);
		double h = std::min(output->GridSpacing().x, output->GridSpacing().y);

		auto uSourceDataPos = input.GetUPosition();
		auto uTargetDataPos = output->GetUPosition();
		auto uTargetDataAcc = output->GetUAccessor();

		output->ParallelForEachUIndex([&](size_t i, size_t j)
		{
			if (boundarySDF.Sample(uSourceDataPos(i, j)) > 0.0)
			{
				Vector2D pt = BackTrace(flow, dt, h, uTargetDataPos(i, j), boundarySDF);
				uTargetDataAcc(i, j) = inputSamplerFunc(pt).x;
			}
		});

		auto vSourceDataPos = input.GetVPosition();
		auto vTargetDataPos = output->GetVPosition();
		auto vTargetDataAcc = output->GetVAccessor();

		output->ParallelForEachVIndex([&](size_t i, size_t j)
		{
			if (boundarySDF.Sample(vSourceDataPos(i, j)) > 0.0)
			{
				Vector2D pt = BackTrace(flow, dt, h, vTargetDataPos(i, j), boundarySDF);
				vTargetDataAcc(i, j) = inputSamplerFunc(pt).y;
			}
		});
	}

	Vector2D SemiLagrangian2::BackTrace(
		const VectorField2& flow,
		double dt,
		double h,
		const Vector2D& startPt,
		const ScalarField2& boundarySDF) const
	{
		double remainingT = dt;
		Vector2D pt0 = startPt;
		Vector2D pt1 = startPt;

		while (remainingT > std::numeric_limits<double>::epsilon())
		{
			// Adaptive time-stepping
			Vector2D vel0 = flow.Sample(pt0);
			double numSubSteps = std::max(std::ceil(vel0.Length() * remainingT / h), 1.0);
			dt = remainingT / numSubSteps;

			// Mid-point rule
			Vector2D midPt = pt0 - 0.5 * dt * vel0;
			Vector2D midVel = flow.Sample(midPt);
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

	std::function<double(const Vector2D&)> SemiLagrangian2::GetScalarSamplerFunc(const ScalarGrid2& input) const
	{
		return input.Sampler();
	}

	std::function<Vector2D(const Vector2D&)> SemiLagrangian2::GetVectorSamplerFunc(const CollocatedVectorGrid2& input) const
	{
		return input.Sampler();
	}

	std::function<Vector2D(const Vector2D&)> SemiLagrangian2::GetVectorSamplerFunc(const FaceCenteredGrid2& input) const
	{
		return input.Sampler();
	}
}