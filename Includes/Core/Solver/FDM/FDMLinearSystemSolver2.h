/*************************************************************************
> File Name: FDMLinearSystemSolver2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 2-D finite difference-type linear system solver.
> Created Time: 2017/08/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER2_H
#define CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER2_H

#include <Core/FDM/FDMLinearSystem2.h>

namespace CubbyFlow
{
	//! Abstract base class for 2-D finite difference-type linear system solver.
	class FDMLinearSystemSolver2
	{
	public:
		virtual ~FDMLinearSystemSolver2() = default;

		//! Solves the given linear system.
		virtual bool Solve(FDMLinearSystem2* system) = 0;

		//! Solves the given compressed linear system.
		virtual bool SolveCompressed(FDMCompressedLinearSystem2*)
		{
			return false;
		}
	};

	//! Shared pointer type for the FDMLinearSystemSolver2.
	using FDMLinearSystemSolver2Ptr = std::shared_ptr<FDMLinearSystemSolver2>;
}

#endif