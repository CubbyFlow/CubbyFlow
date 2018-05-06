/*************************************************************************
> File Name: CubicSemiLagrangian2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Implementation of 2-D cubic semi-Lagrangian advection solver.
> Created Time: 2017/08/08
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CUBIC_SEMI_LAGRANGIAN2_H
#define CUBBYFLOW_CUBIC_SEMI_LAGRANGIAN2_H

#include <Core/SemiLagrangian/SemiLagrangian2.h>

namespace CubbyFlow
{
	//!
	//! \brief Implementation of 2-D cubic semi-Lagrangian advection solver.
	//!
	//! This class implements 3rd-order cubic 2-D semi-Lagrangian advection solver.
	//!
	class CubicSemiLagrangian2 final : public SemiLagrangian2
	{
	public:
		CubicSemiLagrangian2();

	protected:
		//!
		//! \brief Returns spatial interpolation function object for given scalar grid.
		//!
		//! This function overrides the original function with cubic interpolation.
		//!
		std::function<double(const Vector2D&)> GetScalarSamplerFunc(const ScalarGrid2& source) const override;

		//!
		//! \brief Returns spatial interpolation function object for given
		//! collocated vector grid.
		//!
		//! This function overrides the original function with cubic interpolation.
		//!
		std::function<Vector2D(const Vector2D&)> GetVectorSamplerFunc(const CollocatedVectorGrid2& source) const override;

		//!
		//! \brief Returns spatial interpolation function object for given
		//! face-centered vector grid.
		//!
		//! This function overrides the original function with cubic interpolation.
		//!
		std::function<Vector2D(const Vector2D&)> GetVectorSamplerFunc(const FaceCenteredGrid2& source) const override;
	};

	using CubicSemiLagrangian2Ptr = std::shared_ptr<CubicSemiLagrangian2>;
}

#endif