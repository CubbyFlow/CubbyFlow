/*************************************************************************
> File Name: GridForwardEulerDiffusionSolver3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D grid-based forward Euler diffusion Solver.
> Created Time: 2017/08/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/FDM/FDMUtils.h>
#include <Core/LevelSet/LevelSetUtils.h>
#include <Core/Solver/Grid/GridForwardEulerDiffusionSolver3.h>

namespace CubbyFlow
{
	static const char FLUID = 0;
	static const char AIR = 1;
	static const char BOUNDARY = 2;

	template <typename T>
	T Laplacian(
		const ConstArrayAccessor3<T>& data,
		const Array3<char>& marker,
		const Vector3D& GridSpacing,
		size_t i, size_t j, size_t k)
	{
		const T center = data(i, j, k);
		const Size3 ds = data.size();

		assert(i < ds.x && j < ds.y && k < ds.z);

		T dLeft = Zero<T>();
		T dRight = Zero<T>();
		T dDown = Zero<T>();
		T dUp = Zero<T>();
		T dBack = Zero<T>();
		T dFront = Zero<T>();

		if (i > 0 && marker(i - 1, j, k) == FLUID)
		{
			dLeft = center - data(i - 1, j, k);
		}
		if (i + 1 < ds.x && marker(i + 1, j, k) == FLUID)
		{
			dRight = data(i + 1, j, k) - center;
		}

		if (j > 0 && marker(i, j - 1, k) == FLUID)
		{
			dDown = center - data(i, j - 1, k);
		}
		if (j + 1 < ds.y && marker(i, j + 1, k) == FLUID)
		{
			dUp = data(i, j + 1, k) - center;
		}

		if (k > 0 && marker(i, j, k - 1) == FLUID)
		{
			dBack = center - data(i, j, k - 1);
		}
		if (k + 1 < ds.z && marker(i, j, k + 1) == FLUID)
		{
			dFront = data(i, j, k + 1) - center;
		}

		return
			(dRight - dLeft) / Square(GridSpacing.x) +
			(dUp - dDown) / Square(GridSpacing.y) +
			(dFront - dBack) / Square(GridSpacing.z);
	}

	GridForwardEulerDiffusionSolver3::GridForwardEulerDiffusionSolver3()
	{
		// Do nothing
	}

	void GridForwardEulerDiffusionSolver3::Solve(
		const ScalarGrid3& source,
		double diffusionCoefficient,
		double timeIntervalInSeconds,
		ScalarGrid3* dest,
		const ScalarField3& boundarySDF,
		const ScalarField3& fluidSDF)
	{
		auto src = source.GetConstDataAccessor();
		Vector3D h = source.GridSpacing();
		auto pos = source.GetDataPosition();

		BuildMarkers(source.Resolution(), pos, boundarySDF, fluidSDF);

		source.ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k)
		{
			if (m_markers(i, j, k) == FLUID)
			{
				(*dest)(i, j, k) = source(i, j, k) + diffusionCoefficient * timeIntervalInSeconds * Laplacian(src, m_markers, h, i, j, k);
			}
			else
			{
				(*dest)(i, j, k) = source(i, j, k);
			}
		});
	}

	void GridForwardEulerDiffusionSolver3::Solve(
		const CollocatedVectorGrid3& source,
		double diffusionCoefficient,
		double timeIntervalInSeconds,
		CollocatedVectorGrid3* dest,
		const ScalarField3& boundarySDF,
		const ScalarField3& fluidSDF)
	{
		auto src = source.GetConstDataAccessor();
		Vector3D h = source.GridSpacing();
		auto pos = source.GetDataPosition();

		BuildMarkers(source.Resolution(), pos, boundarySDF, fluidSDF);

		source.ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k)
		{
			if (m_markers(i, j, k) == FLUID)
			{
				(*dest)(i, j, k) = src(i, j, k) + diffusionCoefficient * timeIntervalInSeconds * Laplacian(src, m_markers, h, i, j, k);
			}
			else
			{
				(*dest)(i, j, k) = source(i, j, k);
			}
		});
	}

	void GridForwardEulerDiffusionSolver3::Solve(
		const FaceCenteredGrid3& source,
		double diffusionCoefficient,
		double timeIntervalInSeconds,
		FaceCenteredGrid3* dest,
		const ScalarField3& boundarySDF,
		const ScalarField3& fluidSDF)
	{
		auto uSrc = source.GetUConstAccessor();
		auto vSrc = source.GetVConstAccessor();
		auto wSrc = source.GetWConstAccessor();
		auto u = dest->GetUAccessor();
		auto v = dest->GetVAccessor();
		auto w = dest->GetWAccessor();
		auto uPos = source.GetUPosition();
		auto vPos = source.GetVPosition();
		auto wPos = source.GetWPosition();
		Vector3D h = source.GridSpacing();

		BuildMarkers(source.GetUSize(), uPos, boundarySDF, fluidSDF);

		source.ParallelForEachUIndex([&](size_t i, size_t j, size_t k)
		{
			if (!IsInsideSDF(boundarySDF.Sample(uPos(i, j, k))))
			{
				u(i, j, k) = uSrc(i, j, k) + diffusionCoefficient * timeIntervalInSeconds * Laplacian3(uSrc, h, i, j, k);
			}
		});

		BuildMarkers(source.GetVSize(), vPos, boundarySDF, fluidSDF);

		source.ParallelForEachVIndex([&](size_t i, size_t j, size_t k)
		{
			if (!IsInsideSDF(boundarySDF.Sample(vPos(i, j, k))))
			{
				v(i, j, k) = vSrc(i, j, k) + diffusionCoefficient * timeIntervalInSeconds * Laplacian3(vSrc, h, i, j, k);
			}
		});

		BuildMarkers(source.GetWSize(), wPos, boundarySDF, fluidSDF);

		source.ParallelForEachUIndex([&](size_t i, size_t j, size_t k)
		{
			if (!IsInsideSDF(boundarySDF.Sample(wPos(i, j, k))))
			{
				w(i, j, k) = wSrc(i, j, k) + diffusionCoefficient * timeIntervalInSeconds * Laplacian3(wSrc, h, i, j, k);
			}
		});
	}

	void GridForwardEulerDiffusionSolver3::BuildMarkers(
		const Size3& size,
		const std::function<Vector3D(size_t, size_t, size_t)>& pos,
		const ScalarField3& boundarySDF,
		const ScalarField3& fluidSDF)
	{
		m_markers.Resize(size);

		m_markers.ForEachIndex([&](size_t i, size_t j, size_t k)
		{
			if (IsInsideSDF(boundarySDF.Sample(pos(i, j, k))))
			{
				m_markers(i, j, k) = BOUNDARY;
			}
			else if (IsInsideSDF(fluidSDF.Sample(pos(i, j, k))))
			{
				m_markers(i, j, k) = FLUID;
			}
			else
			{
				m_markers(i, j, k) = AIR;
			}
		});
	}
}