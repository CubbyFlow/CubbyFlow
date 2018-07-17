/*************************************************************************
> File Name: CubicSemiLagrangian2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Implementation of 2-D cubic semi-Lagrangian advection solver.
> Created Time: 2017/08/08
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <SemiLagrangian/CubicSemiLagrangian2.h>

namespace CubbyFlow
{
	CubicSemiLagrangian2::CubicSemiLagrangian2()
	{
		// Do nothing
	}

	std::function<double(const Vector2D&)> CubicSemiLagrangian2::GetScalarSamplerFunc(const ScalarGrid2& source) const
	{
		auto sourceSampler = CubicArraySampler2<double, double>(
			source.GetConstDataAccessor(),
			source.GridSpacing(),
			source.GetDataOrigin());
		return sourceSampler.Functor();
	}

	std::function<Vector2D(const Vector2D&)> CubicSemiLagrangian2::GetVectorSamplerFunc(const CollocatedVectorGrid2& source) const
	{
		auto sourceSampler = CubicArraySampler2<Vector2D, double>(
			source.GetConstDataAccessor(),
			source.GridSpacing(),
			source.GetDataOrigin());
		return sourceSampler.Functor();
	}

	std::function<Vector2D(const Vector2D&)> CubicSemiLagrangian2::GetVectorSamplerFunc(const FaceCenteredGrid2& source) const
	{
		auto uSourceSampler = CubicArraySampler2<double, double>(
			source.GetUConstAccessor(),
			source.GridSpacing(),
			source.GetUOrigin());
		auto vSourceSampler = CubicArraySampler2<double, double>(
			source.GetVConstAccessor(),
			source.GridSpacing(),
			source.GetVOrigin());
		return [uSourceSampler, vSourceSampler](const Vector2D& x)
		{
			return Vector2D(uSourceSampler(x), vSourceSampler(x));
		};
	}
}