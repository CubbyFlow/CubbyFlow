/*************************************************************************
> File Name: CubicSemiLagrangian3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Implementation of 3-D cubic semi-Lagrangian advection solver.
> Created Time: 2017/08/08
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CUBIC_SEMI_LAGRANGIAN3_H
#define CUBBYFLOW_CUBIC_SEMI_LAGRANGIAN3_H

#include <SemiLagrangian/SemiLagrangian3.h>

namespace CubbyFlow
{
	//!
	//! \brief Implementation of 3-D cubic semi-Lagrangian advection solver.
	//!
	//! This class implements 3rd-order cubic 3-D semi-Lagrangian advection solver.
	//!
	class CubicSemiLagrangian3 final : public SemiLagrangian3
	{
	public:
		CubicSemiLagrangian3();

	protected:
		//!
		//! \brief Returns spatial interpolation function object for given scalar grid.
		//!
		//! This function overrides the original function with cubic interpolation.
		//!
		std::function<double(const Vector3D&)> GetScalarSamplerFunc(const ScalarGrid3& source) const override;

		//!
		//! \brief Returns spatial interpolation function object for given
		//! collocated vector grid.
		//!
		//! This function overrides the original function with cubic interpolation.
		//!
		std::function<Vector3D(const Vector3D&)> GetVectorSamplerFunc(const CollocatedVectorGrid3& source) const override;

		//!
		//! \brief Returns spatial interpolation function object for given
		//! face-centered vector grid.
		//!
		//! This function overrides the original function with cubic interpolation.
		//!
		std::function<Vector3D(const Vector3D&)> GetVectorSamplerFunc(const FaceCenteredGrid3& source) const override;
	};

	using CubicSemiLagrangian3Ptr = std::shared_ptr<CubicSemiLagrangian3>;
}

#endif