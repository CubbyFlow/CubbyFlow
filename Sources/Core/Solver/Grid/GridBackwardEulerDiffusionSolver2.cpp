/*************************************************************************
> File Name: GridBackwardEulerDiffusionSolver2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D grid-based backward Euler diffusion Solver.
> Created Time: 2017/08/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/LevelSet/LevelSetUtils.hpp>
#include <Core/Solver/FDM/FDMICCGSolver2.h>
#include <Core/Solver/Grid/GridBackwardEulerDiffusionSolver2.h>

namespace CubbyFlow
{
	const char FLUID = 0;
	const char AIR = 1;
	const char BOUNDARY = 2;

	GridBackwardEulerDiffusionSolver2::GridBackwardEulerDiffusionSolver2(BoundaryType boundaryType) :
		m_boundaryType(boundaryType)
	{
		m_systemSolver = std::make_shared<FDMICCGSolver2>(100, std::numeric_limits<double>::epsilon());
	}

	void GridBackwardEulerDiffusionSolver2::Solve(
		const ScalarGrid2& source,
		double diffusionCoefficient,
		double timeIntervalInSeconds,
		ScalarGrid2* dest,
		const ScalarField2& boundarySDF,
		const ScalarField2& fluidSDF)
	{
		if (m_systemSolver != nullptr)
		{
			auto pos = source.GetDataPosition();
			Vector2D h = source.GridSpacing();
			Vector2D c = timeIntervalInSeconds * diffusionCoefficient / (h * h);

			BuildMarkers(source.GetDataSize(), pos, boundarySDF, fluidSDF);
			BuildMatrix(source.GetDataSize(), c);
			BuildVectors(source.GetConstDataAccessor(), c);

			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Assign the solution
			source.ParallelForEachDataPointIndex(
				[&](size_t i, size_t j)
			{
				(*dest)(i, j) = m_system.x(i, j);
			});
		}
	}

	void GridBackwardEulerDiffusionSolver2::Solve(
		const CollocatedVectorGrid2& source,
		double diffusionCoefficient,
		double timeIntervalInSeconds,
		CollocatedVectorGrid2* dest,
		const ScalarField2& boundarySDF,
		const ScalarField2& fluidSDF)
	{
		if (m_systemSolver != nullptr)
		{
			auto pos = source.GetDataPosition();
			Vector2D h = source.GridSpacing();
			Vector2D c = timeIntervalInSeconds * diffusionCoefficient / (h * h);

			BuildMarkers(source.GetDataSize(), pos, boundarySDF, fluidSDF);
			BuildMatrix(source.GetDataSize(), c);

			// u
			BuildVectors(source.GetConstDataAccessor(), c, 0);

			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Assign the solution
			source.ParallelForEachDataPointIndex(
				[&](size_t i, size_t j)
			{
				(*dest)(i, j).x = m_system.x(i, j);
			});

			// v
			BuildVectors(source.GetConstDataAccessor(), c, 1);

			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Assign the solution
			source.ParallelForEachDataPointIndex(
				[&](size_t i, size_t j)
			{
				(*dest)(i, j).y = m_system.x(i, j);
			});
		}
	}

	void GridBackwardEulerDiffusionSolver2::Solve(
		const FaceCenteredGrid2& source,
		double diffusionCoefficient,
		double timeIntervalInSeconds,
		FaceCenteredGrid2* dest,
		const ScalarField2& boundarySDF,
		const ScalarField2& fluidSDF)
	{
		if (m_systemSolver != nullptr)
		{
			Vector2D h = source.GridSpacing();
			Vector2D c = timeIntervalInSeconds * diffusionCoefficient / (h * h);

			// u
			auto uPos = source.GetUPosition();
			BuildMarkers(source.GetUSize(), uPos, boundarySDF, fluidSDF);
			BuildMatrix(source.GetUSize(), c);
			BuildVectors(source.GetUConstAccessor(), c);

			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Assign the solution
			source.ParallelForEachUIndex(
				[&](size_t i, size_t j)
			{
				dest->GetU(i, j) = m_system.x(i, j);
			});

			// v
			auto vPos = source.GetVPosition();
			BuildMarkers(source.GetVSize(), vPos, boundarySDF, fluidSDF);
			BuildMatrix(source.GetVSize(), c);
			BuildVectors(source.GetVConstAccessor(), c);

			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Assign the solution
			source.ParallelForEachVIndex(
				[&](size_t i, size_t j)
			{
				dest->GetV(i, j) = m_system.x(i, j);
			});
		}
	}

	void GridBackwardEulerDiffusionSolver2::SetLinearSystemSolver(const FDMLinearSystemSolver2Ptr& Solver)
	{
		m_systemSolver = Solver;
	}

	void GridBackwardEulerDiffusionSolver2::BuildMarkers(
		const Size2& size,
		const std::function<Vector2D(size_t, size_t)>& pos,
		const ScalarField2& boundarySDF,
		const ScalarField2& fluidSDF)
	{
		m_markers.Resize(size);

		m_markers.ParallelForEachIndex([&](size_t i, size_t j)
		{
			if (IsInsideSDF(boundarySDF.Sample(pos(i, j))))
			{
				m_markers(i, j) = BOUNDARY;
			}
			else if (IsInsideSDF(fluidSDF.Sample(pos(i, j))))
			{
				m_markers(i, j) = FLUID;
			}
			else
			{
				m_markers(i, j) = AIR;
			}
		});
	}

	void GridBackwardEulerDiffusionSolver2::BuildMatrix(const Size2& size, const Vector2D& c)
	{
		m_system.A.Resize(size);

		bool isBoundaryType = (m_boundaryType == BoundaryType::Dirichlet);

		// Build linear system
		m_system.A.ParallelForEachIndex([&](size_t i, size_t j)
		{
			auto& row = m_system.A(i, j);

			// Initialize
			row.center = 1.0;
			row.right = row.up = 0.0;

			if (m_markers(i, j) == FLUID)
			{
				if (i + 1 < size.x)
				{
					if ((isBoundaryType && m_markers(i + 1, j) != AIR) || m_markers(i + 1, j) == FLUID)
					{
						row.center += c.x;
					}

					if (m_markers(i + 1, j) == FLUID)
					{
						row.right -= c.x;
					}
				}

				if (i > 0 && ((isBoundaryType && m_markers(i - 1, j) != AIR) || m_markers(i - 1, j) == FLUID))
				{
					row.center += c.x;
				}

				if (j + 1 < size.y)
				{
					if ((isBoundaryType && m_markers(i, j + 1) != AIR) || m_markers(i, j + 1) == FLUID)
					{
						row.center += c.y;
					}

					if (m_markers(i, j + 1) == FLUID)
					{
						row.up -= c.y;
					}
				}

				if (j > 0 && ((isBoundaryType && m_markers(i, j - 1) != AIR) || m_markers(i, j - 1) == FLUID))
				{
					row.center += c.y;
				}
			}
		});
	}

	void GridBackwardEulerDiffusionSolver2::BuildVectors(const ConstArrayAccessor2<double>& f, const Vector2D& c)
	{
		Size2 size = f.size();

		m_system.x.Resize(size, 0.0);
		m_system.b.Resize(size, 0.0);

		// Build linear system
		m_system.x.ParallelForEachIndex([&](size_t i, size_t j)
		{
			m_system.b(i, j) = m_system.x(i, j) = f(i, j);

			if (m_boundaryType == BoundaryType::Dirichlet && m_markers(i, j) == FLUID)
			{
				if (i + 1 < size.x && m_markers(i + 1, j) == BOUNDARY)
				{
					m_system.b(i, j) += c.x * f(i + 1, j);
				}

				if (i > 0 && m_markers(i - 1, j) == BOUNDARY)
				{
					m_system.b(i, j) += c.x * f(i - 1, j);
				}

				if (j + 1 < size.y && m_markers(i, j + 1) == BOUNDARY)
				{
					m_system.b(i, j) += c.y * f(i, j + 1);
				}

				if (j > 0 && m_markers(i, j - 1) == BOUNDARY)
				{
					m_system.b(i, j) += c.y * f(i, j - 1);
				}
			}
		});
	}

	void GridBackwardEulerDiffusionSolver2::BuildVectors(const ConstArrayAccessor2<Vector2D>& f, const Vector2D& c, size_t component)
	{
		Size2 size = f.size();

		m_system.x.Resize(size, 0.0);
		m_system.b.Resize(size, 0.0);

		// Build linear system
		m_system.x.ParallelForEachIndex([&](size_t i, size_t j)
		{
			m_system.b(i, j) = m_system.x(i, j) = f(i, j)[component];

			if (m_boundaryType == BoundaryType::Dirichlet && m_markers(i, j) == FLUID)
			{
				if (i + 1 < size.x && m_markers(i + 1, j) == BOUNDARY)
				{
					m_system.b(i, j) += c.x * f(i + 1, j)[component];
				}

				if (i > 0 && m_markers(i - 1, j) == BOUNDARY)
				{
					m_system.b(i, j) += c.x * f(i - 1, j)[component];
				}

				if (j + 1 < size.y && m_markers(i, j + 1) == BOUNDARY)
				{
					m_system.b(i, j) += c.y * f(i, j + 1)[component];
				}

				if (j > 0 && m_markers(i, j - 1) == BOUNDARY)
				{
					m_system.b(i, j) += c.y * f(i, j - 1)[component];
				}
			}
		});
	}
}