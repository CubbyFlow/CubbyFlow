/*************************************************************************
> File Name: FDMMGLinearSystem2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Multigrid-syle 2-D linear system.
> Created Time: 2017/11/02
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_MG_LINEAR_SYSTEM2_H
#define CUBBYFLOW_FDM_MG_LINEAR_SYSTEM2_H

#include <FDM/FDMLinearSystem2.h>
#include <Utils/MG.h>

namespace CubbyFlow
{
	//! Multigrid-style 2-D FDM matrix.
	using FDMMGMatrix2 = MGMatrix<FDMBLAS2>;

	//! Multigrid-style 2-D FDM vector.
	using FDMMGVector2 = MGVector<FDMBLAS2>;

	//! Multigrid-syle 2-D linear system.
	struct FDMMGLinearSystem2
	{
		//! The system matrix.
		FDMMGMatrix2 A;

		//! The solution vector.
		FDMMGVector2 x;

		//! The RHS vector.
		FDMMGVector2 b;

		//! Clears the linear system.
		void Clear();

		//! Returns the number of multigrid levels.
		size_t GetNumberOfLevels() const;

		//! Resizes the system with the coarsest resolution and number of levels.
		void ResizeWithCoarsest(const Size2& coarsestResolution, size_t numberOfLevels);

		//!
		//! \brief Resizes the system with the finest resolution and max number of levels.
		//!
		//! This function resizes the system with multiple levels until the
		//! resolution is divisible with 2^(level-1).
		//!
		//! \param finestResolution - The finest grid resolution.
		//! \param maxNumberOfLevels - Maximum number of multigrid levels.
		//!
		void ResizeWithFinest(const Size2& finestResolution, size_t maxNumberOfLevels);
	};

	//! Multigrid utilities for 2-D FDM system.
	class FDMMGUtils2
	{
	public:
		//! Restricts given finer grid to the coarser grid.
		static void Restrict(const FDMVector2& finer, FDMVector2* coarser);

		//! Corrects given coarser grid to the finer grid.
		static void Correct(const FDMVector2& coarser, FDMVector2* finer);

		//! Resizes the array with the coarsest resolution and number of levels.
		template <typename T>
		static void ResizeArrayWithCoarsest(const Size2& coarsestResolution,
			size_t numberOfLevels, std::vector<Array2<T>>* levels);

		//!
		//! \brief Resizes the array with the finest resolution and max number of levels.
		//!
		//! This function resizes the system with multiple levels until the
		//! resolution is divisible with 2^(level-1).
		//!
		//! \param finestResolution - The finest grid resolution.
		//! \param maxNumberOfLevels - Maximum number of multigrid levels.
		//!
		template <typename T>
		static void ResizeArrayWithFinest(const Size2& finestResolution,
			size_t maxNumberOfLevels, std::vector<Array2<T>>* levels);
	};
}

#include <FDM/FDMMGLinearSystem2-Impl.h>

#endif