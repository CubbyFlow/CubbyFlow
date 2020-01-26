/*************************************************************************
> File Name: FDMLinearSystem2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Linear system (Ax=b) for 2-D finite differencing.
> Created Time: 2017/08/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM2_H
#define CUBBYFLOW_FDM_LINEAR_SYSTEM2_H

#include <Core/Array/Array2.hpp>
#include <Core/Matrix/MatrixCSR.h>
#include <Core/Size/Size2.h>
#include <Core/Vector/VectorN.h>

namespace CubbyFlow
{
	//! The row of FDMMatrix2 where row corresponds to (i, j) grid point.
	struct FDMMatrixRow2
	{
		//! Diagonal component of the matrix (row, row).
		double center = 0.0;

		//! Off-diagonal element where column refers to (i+1, j) grid point.
		double right = 0.0;

		//! Off-diagonal element where column refers to (i, j+1) grid point.
		double up = 0.0;
	};

	//! Vector type for 2-D finite differencing.
	using FDMVector2 = Array2<double>;

	//! Matrix type for 2-D finite differencing.
	using FDMMatrix2 = Array2<FDMMatrixRow2>;

	//! Linear system (Ax=b) for 2-D finite differencing.
	struct FDMLinearSystem2
	{
		//! System matrix.
		FDMMatrix2 A;

		//! Solution vector.
		FDMVector2 x;
		
		//! RHS vector.
		FDMVector2 b;

		//! Clears all the data.
		void Clear();

		//! Resizes the arrays with given grid size.
		void Resize(const Size2& size);
	};

	//! Compressed linear system (Ax=b) for 2-D finite differencing.
	struct FDMCompressedLinearSystem2
	{
		//! System matrix.
		MatrixCSRD A;

		//! Solution vector.
		VectorND x;

		//! RHS vector.
		VectorND b;

		//! Clears all the data.
		void Clear();
	};

	//! BLAS operator wrapper for 2-D finite differencing.
	struct FDMBLAS2
	{
		using ScalarType = double;
		using VectorType = FDMVector2;
		using MatrixType = FDMMatrix2;

		//! Sets entire element of given vector \p result with scalar \p s.
		static void Set(ScalarType s, VectorType* result);

		//! Copies entire element of given vector \p result with other vector \p v.
		static void Set(const VectorType& v, VectorType* result);

		//! Sets entire element of given matrix \p result with scalar \p s.
		static void Set(ScalarType s, MatrixType* result);

		//! Copies entire element of given matrix \p result with other matrix \p v.
		static void Set(const MatrixType& m, MatrixType* result);

		//! Performs dot product with vector \p a and \p b.
		static double Dot(const VectorType& a, const VectorType& b);

		//! Performs ax + y operation where \p a is a matrix and \p x and \p y are vectors.
		static void AXPlusY(double a, const VectorType& x, const VectorType& y, VectorType* result);

		//! Performs matrix-vector multiplication.
		static void MVM(const MatrixType& m, const VectorType& v, VectorType* result);

		//! Computes residual vector (b - ax).
		static void Residual(const MatrixType& a, const VectorType& x, const VectorType& b, VectorType* result);

		//! Returns L2-norm of the given vector \p v.
		static ScalarType L2Norm(const VectorType& v);

		//! Returns Linf-norm of the given vector \p v.
		static ScalarType LInfNorm(const VectorType& v);
	};

	//! BLAS operator wrapper for compressed 2-D finite differencing.
	struct FDMCompressedBLAS2
	{
		using ScalarType = double;
		using VectorType = VectorND;
		using MatrixType = MatrixCSRD;

		//! Sets entire element of given vector \p result with scalar \p s.
		static void Set(ScalarType s, VectorType* result);

		//! Copies entire element of given vector \p result with other vector \p v.
		static void Set(const VectorType& v, VectorType* result);

		//! Sets entire element of given matrix \p result with scalar \p s.
		static void Set(ScalarType s, MatrixType* result);

		//! Copies entire element of given matrix \p result with other matrix \p v.
		static void Set(const MatrixType& m, MatrixType* result);

		//! Performs dot product with vector \p a and \p b.
		static double Dot(const VectorType& a, const VectorType& b);

		//! Performs ax + y operation where \p a is a matrix and \p x and \p y are vectors.
		static void AXPlusY(double a, const VectorType& x, const VectorType& y, VectorType* result);

		//! Performs matrix-vector multiplication.
		static void MVM(const MatrixType& m, const VectorType& v, VectorType* result);

		//! Computes residual vector (b - ax).
		static void Residual(const MatrixType& a, const VectorType& x, const VectorType& b, VectorType* result);

		//! Returns L2-norm of the given vector \p v.
		static ScalarType L2Norm(const VectorType& v);

		//! Returns Linf-norm of the given vector \p v.
		static ScalarType LInfNorm(const VectorType& v);
	};
}

#endif