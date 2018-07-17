/*************************************************************************
> File Name: GridBackwardEulerDiffusionSolver3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D grid-based backward Euler diffusion Solver.
> Created Time: 2017/08/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <LevelSet/LevelSetUtils.h>
#include <Solver/FDM/FDMICCGSolver3.h>
#include <Solver/Grid/GridBackwardEulerDiffusionSolver3.h>

namespace CubbyFlow
{
	const char FLUID = 0;
	const char AIR = 1;
	const char BOUNDARY = 2;

	GridBackwardEulerDiffusionSolver3::GridBackwardEulerDiffusionSolver3(BoundaryType boundaryType) :
		m_boundaryType(boundaryType)
	{
		m_systemSolver = std::make_shared<FDMICCGSolver3>(100, std::numeric_limits<double>::epsilon());
	}

	void GridBackwardEulerDiffusionSolver3::Solve(
		const ScalarGrid3& source,
		double diffusionCoefficient,
		double timeIntervalInSeconds,
		ScalarGrid3* dest,
		const ScalarField3& boundarySDF,
		const ScalarField3& fluidSDF)
	{
		auto pos = source.GetDataPosition();
		Vector3D h = source.GridSpacing();
		Vector3D c = timeIntervalInSeconds * diffusionCoefficient / (h * h);

		BuildMarkers(source.GetDataSize(), pos, boundarySDF, fluidSDF);
		BuildMatrix(source.GetDataSize(), c);
		BuildVectors(source.GetConstDataAccessor(), c);

		if (m_systemSolver != nullptr)
		{
			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Assign the solution
			source.ParallelForEachDataPointIndex(
				[&](size_t i, size_t j, size_t k)
			{
				(*dest)(i, j, k) = m_system.x(i, j, k);
			});
		}
	}

	void GridBackwardEulerDiffusionSolver3::Solve(
		const CollocatedVectorGrid3& source,
		double diffusionCoefficient,
		double timeIntervalInSeconds,
		CollocatedVectorGrid3* dest,
		const ScalarField3& boundarySDF,
		const ScalarField3& fluidSDF)
	{
		auto pos = source.GetDataPosition();
		Vector3D h = source.GridSpacing();
		Vector3D c = timeIntervalInSeconds * diffusionCoefficient / (h * h);

		BuildMarkers(source.GetDataSize(), pos, boundarySDF, fluidSDF);
		BuildMatrix(source.GetDataSize(), c);

		// u
		BuildVectors(source.GetConstDataAccessor(), c, 0);

		if (m_systemSolver != nullptr)
		{
			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Assign the solution
			source.ParallelForEachDataPointIndex(
				[&](size_t i, size_t j, size_t k)
			{
				(*dest)(i, j, k).x = m_system.x(i, j, k);
			});
		}

		// v
		BuildVectors(source.GetConstDataAccessor(), c, 1);

		if (m_systemSolver != nullptr)
		{
			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Assign the solution
			source.ParallelForEachDataPointIndex(
				[&](size_t i, size_t j, size_t k)
			{
				(*dest)(i, j, k).y = m_system.x(i, j, k);
			});
		}

		// w
		BuildVectors(source.GetConstDataAccessor(), c, 2);

		if (m_systemSolver != nullptr)
		{
			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Assign the solution
			source.ParallelForEachDataPointIndex(
				[&](size_t i, size_t j, size_t k)
			{
				(*dest)(i, j, k).z = m_system.x(i, j, k);
			});
		}
	}

	void GridBackwardEulerDiffusionSolver3::Solve(
		const FaceCenteredGrid3& source,
		double diffusionCoefficient,
		double timeIntervalInSeconds,
		FaceCenteredGrid3* dest,
		const ScalarField3& boundarySDF,
		const ScalarField3& fluidSDF)
	{
		Vector3D h = source.GridSpacing();
		Vector3D c = timeIntervalInSeconds * diffusionCoefficient / (h * h);

		// u
		auto uPos = source.GetUPosition();
		BuildMarkers(source.GetUSize(), uPos, boundarySDF, fluidSDF);
		BuildMatrix(source.GetUSize(), c);
		BuildVectors(source.GetUConstAccessor(), c);

		if (m_systemSolver != nullptr)
		{
			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Assign the solution
			source.ParallelForEachUIndex(
				[&](size_t i, size_t j, size_t k)
			{
				dest->GetU(i, j, k) = m_system.x(i, j, k);
			});
		}

		// v
		auto vPos = source.GetVPosition();
		BuildMarkers(source.GetVSize(), vPos, boundarySDF, fluidSDF);
		BuildMatrix(source.GetVSize(), c);
		BuildVectors(source.GetVConstAccessor(), c);

		if (m_systemSolver != nullptr)
		{
			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Assign the solution
			source.ParallelForEachVIndex(
				[&](size_t i, size_t j, size_t k)
			{
				dest->GetV(i, j, k) = m_system.x(i, j, k);
			});
		}

		// w
		auto wPos = source.GetWPosition();
		BuildMarkers(source.GetWSize(), wPos, boundarySDF, fluidSDF);
		BuildMatrix(source.GetWSize(), c);
		BuildVectors(source.GetWConstAccessor(), c);

		if (m_systemSolver != nullptr)
		{
			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Assign the solution
			source.ParallelForEachWIndex(
				[&](size_t i, size_t j, size_t k)
			{
				dest->GetW(i, j, k) = m_system.x(i, j, k);
			});
		}
	}

	void GridBackwardEulerDiffusionSolver3::SetLinearSystemSolver(const FDMLinearSystemSolver3Ptr& Solver)
	{
		m_systemSolver = Solver;
	}

	void GridBackwardEulerDiffusionSolver3::BuildMarkers(
		const Size3& size,
		const std::function<Vector3D(size_t, size_t, size_t)>& pos,
		const ScalarField3& boundarySDF,
		const ScalarField3& fluidSDF)
	{
		m_markers.Resize(size);

		m_markers.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
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

	void GridBackwardEulerDiffusionSolver3::BuildMatrix(const Size3& size, const Vector3D& c)
	{
		m_system.A.Resize(size);

		bool isBoundaryType = (m_boundaryType == BoundaryType::Dirichlet);

		// Build linear system
		m_system.A.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			auto& row = m_system.A(i, j, k);

			// Initialize
			row.center = 1.0;
			row.right = row.up = row.front = 0.0;

			if (m_markers(i, j, k) == FLUID)
			{
				if (i + 1 < size.x)
				{
					if ((isBoundaryType && m_markers(i + 1, j, k) != AIR) || m_markers(i + 1, j, k) == FLUID)
					{
						row.center += c.x;
					}

					if (m_markers(i + 1, j, k) == FLUID)
					{
						row.right -= c.x;
					}
				}

				if (i > 0 && ((isBoundaryType && m_markers(i - 1, j, k) != AIR) || m_markers(i - 1, j, k) == FLUID))
				{
					row.center += c.x;
				}

				if (j + 1 < size.y)
				{
					if ((isBoundaryType && m_markers(i, j + 1, k) != AIR) || m_markers(i, j + 1, k) == FLUID)
					{
						row.center += c.y;
					}

					if (m_markers(i, j + 1, k) == FLUID)
					{
						row.up -= c.y;
					}
				}

				if (j > 0 && ((isBoundaryType && m_markers(i, j - 1, k) != AIR) || m_markers(i, j - 1, k) == FLUID))
				{
					row.center += c.y;
				}

				if (k + 1 < size.z)
				{
					if ((isBoundaryType && m_markers(i, j, k + 1) != AIR) || m_markers(i, j, k + 1) == FLUID)
					{
						row.center += c.z;
					}

					if (m_markers(i, j, k + 1) == FLUID)
					{
						row.front -= c.z;
					}
				}

				if (k > 0 && ((isBoundaryType && m_markers(i, j, k - 1) != AIR) || m_markers(i, j, k - 1) == FLUID))
				{
					row.center += c.z;
				}
			}
		});
	}

	void GridBackwardEulerDiffusionSolver3::BuildVectors(const ConstArrayAccessor3<double>& f, const Vector3D& c)
	{
		Size3 size = f.size();

		m_system.x.Resize(size, 0.0);
		m_system.b.Resize(size, 0.0);

		// Build linear system
		m_system.x.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			m_system.b(i, j, k) = m_system.x(i, j, k) = f(i, j, k);

			if (m_boundaryType == BoundaryType::Dirichlet && m_markers(i, j, k) == FLUID)
			{
				if (i + 1 < size.x && m_markers(i + 1, j, k) == BOUNDARY)
				{
					m_system.b(i, j, k) += c.x * f(i + 1, j, k);
				}

				if (i > 0 && m_markers(i - 1, j, k) == BOUNDARY)
				{
					m_system.b(i, j, k) += c.x * f(i - 1, j, k);
				}

				if (j + 1 < size.y && m_markers(i, j + 1, k) == BOUNDARY)
				{
					m_system.b(i, j, k) += c.y * f(i, j + 1, k);
				}

				if (j > 0 && m_markers(i, j - 1, k) == BOUNDARY)
				{
					m_system.b(i, j, k) += c.y * f(i, j - 1, k);
				}

				if (k + 1 < size.z && m_markers(i, j, k + 1) == BOUNDARY)
				{
					m_system.b(i, j, k) += c.z * f(i, j, k + 1);
				}

				if (k > 0 && m_markers(i, j, k - 1) == BOUNDARY)
				{
					m_system.b(i, j, k) += c.z * f(i, j, k - 1);
				}
			}
		});
	}

	void GridBackwardEulerDiffusionSolver3::BuildVectors(const ConstArrayAccessor3<Vector3D>& f, const Vector3D& c, size_t component)
	{
		Size3 size = f.size();

		m_system.x.Resize(size, 0.0);
		m_system.b.Resize(size, 0.0);

		// Build linear system
		m_system.x.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			m_system.b(i, j, k) = m_system.x(i, j, k) = f(i, j, k)[component];

			if (m_boundaryType == BoundaryType::Dirichlet && m_markers(i, j, k) == FLUID)
			{
				if (i + 1 < size.x && m_markers(i + 1, j, k) == BOUNDARY)
				{
					m_system.b(i, j, k) += c.x * f(i + 1, j, k)[component];
				}

				if (i > 0 && m_markers(i - 1, j, k) == BOUNDARY)
				{
					m_system.b(i, j, k) += c.x * f(i - 1, j, k)[component];
				}

				if (j + 1 < size.y && m_markers(i, j + 1, k) == BOUNDARY)
				{
					m_system.b(i, j, k) += c.y * f(i, j + 1, k)[component];
				}

				if (j > 0 && m_markers(i, j - 1, k) == BOUNDARY)
				{
					m_system.b(i, j, k) += c.y * f(i, j - 1, k)[component];
				}

				if (k + 1 < size.z && m_markers(i, j, k + 1) == BOUNDARY)
				{
					m_system.b(i, j, k) += c.z * f(i, j, k + 1)[component];
				}

				if (k > 0 && m_markers(i, j, k - 1) == BOUNDARY)
				{
					m_system.b(i, j, k) += c.z * f(i, j, k - 1)[component];
				}
			}
		});
	}
}