/*************************************************************************
> File Name: MultiGrid.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Multi-grid wrapper functions for CubbyFlow.
> Created Time: 2017/09/26
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_MULTI_GRID_H
#define CUBBYFLOW_MULTI_GRID_H

#include <functional>

namespace CubbyFlow
{
	//! Multi-grid matrix wrapper.
	template <typename BlasType>
	struct MGMatrix
	{
		std::vector<typename BlasType::MatrixType> levels;
		const typename BlasType::MatrixType& operator[](size_t i) const;
		typename BlasType::MatrixType& operator[](size_t i);
		const typename BlasType::MatrixType& Finest() const;
		typename BlasType::MatrixType& Finest();
	};

	//! Multi-grid vector wrapper.
	template <typename BlasType>
	struct MGVector
	{
		std::vector<typename BlasType::VectorType> levels;
		const typename BlasType::VectorType& operator[](size_t i) const;
		typename BlasType::VectorType& operator[](size_t i);
		const typename BlasType::VectorType& Finest() const;
		typename BlasType::VectorType& Finest();
	};

	//! Multi-grid relax function type.
	template <typename BlasType>
	using MGRelaxFunc = std::function<void(
		const typename BlasType::MatrixType& A,
		const typename BlasType::VectorType& b, unsigned int numberOfIterations,
		double maxTolerance, typename BlasType::VectorType* x,
		typename BlasType::VectorType* buffer)>;

	//! Multi-grid restriction function type.
	template <typename BlasType>
	using MGRestrictFunc = std::function<void(
		const typename BlasType::VectorType& finer,
		typename BlasType::VectorType* coarser)>;

	//! Multi-grid correction function type.
	template <typename BlasType>
	using MGCorrectFunc = std::function<void(
		const typename BlasType::VectorType& coarser,
		typename BlasType::VectorType* finer)>;

	//! Multi-grid input parameter set.
	template <typename BlasType>
	struct MGParameters
	{
		//! Max number of multi-grid levels.
		size_t maxNumberOfLevels = 1;

		//! Number of iteration at restriction step.
		unsigned int numberOfRestrictionIter = 5;

		//! Number of iteration at correction step.
		unsigned int numberOfCorrectionIter = 5;

		//! Number of iteration at coarsest step.
		unsigned int numberOfCoarsestIter = 20;

		//! Number of iteration at final step.
		unsigned int numberOfFinalIter = 20;

		//! Relaxation function such as Jacobi or Gauss-Seidel.
		MGRelaxFunc<BlasType> relaxFunc;

		//! Restrict function that maps finer to coarser grid.
		MGRestrictFunc<BlasType> restrictFunc;

		//! Correction function that maps coarser to finer grid.
		MGCorrectFunc<BlasType> correctFunc;

		//! Max error tolerance.
		double maxTolerance = 1e-9;
	};

	//! Multi-grid result type.
	struct MGResult
	{
		//! Lastly measured norm of residual.
		double lastResidualNorm;
	};

	//!
	//! \brief Performs Multi-grid with V-cycle.
	//!
	//! For given linear system matrix \p A and RHS vector \p b, this function
	//! computes the solution \p x using Multi-grid method with V-cycle.
	//!
	template <typename BlasType>
	MGResult MGCycle(
		const MGMatrix<BlasType>& A, MGParameters<BlasType> params,
		MGVector<BlasType>* x, MGVector<BlasType>* b,
		MGVector<BlasType>* buffer);
}

#include <Core/Utils/MG-Impl.h>

#endif