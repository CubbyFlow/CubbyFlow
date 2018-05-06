/*************************************************************************
> File Name: FDMLinearSystem2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Linear system (Ax=b) for 2-D finite differencing.
> Created Time: 2017/08/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/FDM/FDMLinearSystem2.h>
#include <Core/Math/MathUtils.h>

#include <cassert>

namespace CubbyFlow
{
	void FDMLinearSystem2::Clear()
	{
		A.Clear();
		x.Clear();
		b.Clear();
	}

	void FDMLinearSystem2::Resize(const Size2& size)
	{
		A.Resize(size);
		x.Resize(size);
		b.Resize(size);
	}

	void FDMCompressedLinearSystem2::Clear()
	{
		A.Clear();
		x.Clear();
		b.Clear();
	}

	void FDMBLAS2::Set(double s, FDMVector2* result)
	{
		result->Set(s);
	}

	void FDMBLAS2::Set(const FDMVector2& v, FDMVector2* result)
	{
		result->Set(v);
	}

	void FDMBLAS2::Set(double s, FDMMatrix2* result)
	{
		FDMMatrixRow2 row;
		row.center = row.right = row.up = s;
		result->Set(row);
	}

	void FDMBLAS2::Set(const FDMMatrix2& m, FDMMatrix2* result)
	{
		result->Set(m);
	}

	double FDMBLAS2::Dot(const FDMVector2& a, const FDMVector2& b)
	{
		Size2 size = a.size();
		
		assert(size == b.size());

		double result = 0.0;

		for (size_t j = 0; j < size.y; ++j)
		{
			for (size_t i = 0; i < size.x; ++i)
			{
				result += a(i, j) * b(i, j);
			}
		}

		return result;
	}

	void FDMBLAS2::AXPlusY(double a, const FDMVector2& x, const FDMVector2& y, FDMVector2* result)
	{
		assert(x.size() == y.size());
		assert(x.size() == result->size());

		x.ParallelForEachIndex([&](size_t i, size_t j)
		{
			(*result)(i, j) = a * x(i, j) + y(i, j);
		});
	}

	void FDMBLAS2::MVM(const FDMMatrix2& m, const FDMVector2& v, FDMVector2* result)
	{
		Size2 size = m.size();

		assert(size == v.size());
		assert(size == result->size());

		m.ParallelForEachIndex([&](size_t i, size_t j)
		{
			(*result)(i, j) =
				m(i, j).center * v(i, j) +
				((i > 0) ? m(i - 1, j).right * v(i - 1, j) : 0.0) +
				((i + 1 < size.x) ? m(i, j).right * v(i + 1, j) : 0.0) +
				((j > 0) ? m(i, j - 1).up * v(i, j - 1) : 0.0) +
				((j + 1 < size.y) ? m(i, j).up * v(i, j + 1) : 0.0);
		});
	}

	void FDMBLAS2::Residual(const FDMMatrix2& a, const FDMVector2& x, const FDMVector2& b, FDMVector2* result)
	{
		Size2 size = a.size();

		assert(size == x.size());
		assert(size == b.size());
		assert(size == result->size());

		a.ParallelForEachIndex([&](size_t i, size_t j)
		{
			(*result)(i, j) =
				b(i, j) - 
				a(i, j).center * x(i, j) -
				((i > 0) ? a(i - 1, j).right * x(i - 1, j) : 0.0) -
				((i + 1 < size.x) ? a(i, j).right * x(i + 1, j) : 0.0) -
				((j > 0) ? a(i, j - 1).up * x(i, j - 1) : 0.0) -
				((j + 1 < size.y) ? a(i, j).up * x(i, j + 1) : 0.0);
		});
	}

	double FDMBLAS2::L2Norm(const FDMVector2& v)
	{
		return std::sqrt(Dot(v, v));
	}

	double FDMBLAS2::LInfNorm(const FDMVector2& v)
	{
		Size2 size = v.size();
		double result = 0.0;

		for (size_t j = 0; j < size.y; ++j)
		{
			for (size_t i = 0; i < size.x; ++i)
			{
				result = AbsMax(result, v(i, j));
			}
		}

		return std::fabs(result);
	}

	void FDMCompressedBLAS2::Set(double s, VectorND* result)
	{
		result->Set(s);
	}

	void FDMCompressedBLAS2::Set(const VectorND& v, VectorND* result)
	{
		result->Set(v);
	}

	void FDMCompressedBLAS2::Set(double s, MatrixCSRD* result)
	{
		result->Set(s);
	}

	void FDMCompressedBLAS2::Set(const MatrixCSRD& m, MatrixCSRD* result)
	{
		result->Set(m);
	}

	double FDMCompressedBLAS2::Dot(const VectorND& a, const VectorND& b)
	{
		return a.Dot(b);
	}

	void FDMCompressedBLAS2::AXPlusY(double a, const VectorND& x, const VectorND& y, VectorND* result)
	{
		*result = a * x + y;
	}

	void FDMCompressedBLAS2::MVM(const MatrixCSRD& m, const VectorND& v, VectorND* result)
	{
		const auto rp = m.RowPointersBegin();
		const auto ci = m.ColumnIndicesBegin();
		const auto nnz = m.NonZeroBegin();

		v.ParallelForEachIndex([&](size_t i)
		{
			const size_t rowBegin = rp[i];
			const size_t rowEnd = rp[i + 1];

			double sum = 0.0;

			for (size_t jj = rowBegin; jj < rowEnd; ++jj)
			{
				size_t j = ci[jj];
				sum += nnz[jj] * v[j];
			}

			(*result)[i] = sum;
		});
	}

	void FDMCompressedBLAS2::Residual(const MatrixCSRD& a, const VectorND& x, const VectorND& b, VectorND* result)
	{
		const auto rp = a.RowPointersBegin();
		const auto ci = a.ColumnIndicesBegin();
		const auto nnz = a.NonZeroBegin();

		x.ParallelForEachIndex([&](size_t i)
		{
			const size_t rowBegin = rp[i];
			const size_t rowEnd = rp[i + 1];

			double sum = 0.0;

			for (size_t jj = rowBegin; jj < rowEnd; ++jj)
			{
				size_t j = ci[jj];
				sum += nnz[jj] * x[j];
			}

			(*result)[i] = b[i] - sum;
		});
	}

	double FDMCompressedBLAS2::L2Norm(const VectorND& v)
	{
		return std::sqrt(v.Dot(v));
	}

	double FDMCompressedBLAS2::LInfNorm(const VectorND& v)
	{
		return std::fabs(v.AbsMax());
	}
}