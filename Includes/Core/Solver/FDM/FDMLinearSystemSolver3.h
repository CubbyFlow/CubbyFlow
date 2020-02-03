/*************************************************************************
> File Name: FDMLinearSystemSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D finite difference-type linear system solver.
> Created Time: 2017/08/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER3_H
#define CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER3_H

#include <Core/FDM/FDMLinearSystem3.hpp>

namespace CubbyFlow
{
	//! Abstract base class for 3-D finite difference-type linear system solver.
	class FDMLinearSystemSolver3
	{
	public:
		virtual ~FDMLinearSystemSolver3() = default;

		//! Solves the given linear system.
		virtual bool Solve(FDMLinearSystem3* system) = 0;

		//! Solves the given compressed linear system.
		virtual bool SolveCompressed(FDMCompressedLinearSystem3*)
		{
			return false;
		}
	};

	//! Shared pointer type for the FDMLinearSystemSolver3.
	using FDMLinearSystemSolver3Ptr = std::shared_ptr<FDMLinearSystemSolver3>;
}

#endif