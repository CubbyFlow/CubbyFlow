/*************************************************************************
> File Name: FDMLinearSystem3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Linear system (Ax=b) for 3-D finite differencing.
> Created Time: 2017/08/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM3_H
#define CUBBYFLOW_FDM_LINEAR_SYSTEM3_H

#include <Core/Array/Array3.h>
#include <Core/Matrix/MatrixCSR.h>
#include <Core/Size/Size3.h>
#include <Core/Vector/VectorN.h>

namespace CubbyFlow
{
	//! The row of FDMMatrix3 where row corresponds to (i, j, k) grid point.
	struct FDMMatrixRow3
	{
		//! Diagonal component of the matrix (row, row).
		double center = 0.0;

		//! Off-diagonal element where column refers to (i+1, j, k) grid point.
		double right = 0.0;

		//! Off-diagonal element where column refers to (i, j+1, k) grid point.
		double up = 0.0;

		//! OFf-diagonal element where column refers to (i, j, k+1) grid point.
		double front = 0.0;
	};

	//! Vector type for 3-D finite differencing.
	using FDMVector3 = Array3<double>;

	//! Matrix type for 3-D finite differencing.
	using FDMMatrix3 = Array3<FDMMatrixRow3>;

	//! Linear system (Ax=b) for 3-D finite differencing.
	struct FDMLinearSystem3
	{
		//! System matrix.
		FDMMatrix3 A;

		//! Solution vector.
		FDMVector3 x;

		//! RHS vector.
		FDMVector3 b;

		//! Clears all the data.
		void Clear();

		//! Resizes the arrays with given grid size.
		void Resize(const Size3& size);
	};

	//! Compressed linear system (Ax=b) for 3-D finite differencing.
	struct FDMCompressedLinearSystem3
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

	//! BLAS operator wrapper for 3-D finite differencing.
	struct FDMBLAS3
	{
		using ScalarType = double;
		using VectorType = FDMVector3;
		using MatrixType = FDMMatrix3;

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

	//! BLAS operator wrapper for compressed 3-D finite differencing.
	struct FDMCompressedBLAS3
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