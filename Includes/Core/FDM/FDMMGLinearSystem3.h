/*************************************************************************
> File Name: FDMMGLinearSystem3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Multigrid-syle 3-D linear system.
> Created Time: 2017/11/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_MG_LINEAR_SYSTEM3_H
#define CUBBYFLOW_FDM_MG_LINEAR_SYSTEM3_H

#include <Core/FDM/FDMLinearSystem3.h>
#include <Core/Utils/MG.h>

namespace CubbyFlow
{
	//! Multigrid-style 3-D FDM matrix.
	using FDMMGMatrix3 = MGMatrix<FDMBLAS3>;

	//! Multigrid-style 3-D FDM vector.
	using FDMMGVector3 = MGVector<FDMBLAS3>;

	//! Multigrid-syle 3-D linear system.
	struct FDMMGLinearSystem3
	{
		//! The system matrix.
		FDMMGMatrix3 A;

		//! The solution vector.
		FDMMGVector3 x;

		//! The RHS vector.
		FDMMGVector3 b;

		//! Clears the linear system.
		void Clear();

		//! Returns the number of multigrid levels.
		size_t GetNumberOfLevels() const;

		//! Resizes the system with the coarsest resolution and number of levels.
		void ResizeWithCoarsest(const Size3& coarsestResolution, size_t numberOfLevels);

		//!
		//! \brief Resizes the system with the finest resolution and max number of levels.
		//!
		//! This function resizes the system with multiple levels until the
		//! resolution is divisible with 2^(level-1).
		//!
		//! \param finestResolution - The finest grid resolution.
		//! \param maxNumberOfLevels - Maximum number of multigrid levels.
		//!
		void ResizeWithFinest(const Size3& finestResolution, size_t maxNumberOfLevels);
	};

	//! Multigrid utilities for 2-D FDM system.
	class FDMMGUtils3
	{
	public:
		//! Restricts given finer grid to the coarser grid.
		static void Restrict(const FDMVector3& finer, FDMVector3* coarser);

		//! Corrects given coarser grid to the finer grid.
		static void Correct(const FDMVector3& coarser, FDMVector3* finer);

		//! Resizes the array with the coarsest resolution and number of levels.
		template <typename T>
		static void ResizeArrayWithCoarsest(const Size3& coarsestResolution,
			size_t numberOfLevels, std::vector<Array3<T>>* levels);

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
		static void ResizeArrayWithFinest(const Size3& finestResolution,
			size_t maxNumberOfLevels, std::vector<Array3<T>>* levels);
	};
}

#include <Core/FDM/FDMMGLinearSystem3-Impl.h>

#endif