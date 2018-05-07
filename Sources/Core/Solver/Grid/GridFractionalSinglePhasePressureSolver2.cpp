/*************************************************************************
> File Name: GridFractionalSinglePhasePressureSolver2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D fractional single-phase pressure solver.
> Created Time: 2017/08/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/

//
// Adopted the code from:
// http://www.cs.ubc.ca/labs/imager/tr/2007/Batty_VariationalFluids/
// and
// https://github.com/christopherbatty/FluidRigidCoupling2D
//

#include <Core/LevelSet/LevelSetUtils.h>
#include <Core/Solver/FDM/FDMICCGSolver2.h>
#include <Core/Solver/Grid/GridFractionalBoundaryConditionSolver2.h>
#include <Core/Solver/Grid/GridFractionalSinglePhasePressureSolver2.h>

namespace CubbyFlow
{
	const double DEFAULT_TOLERANCE = 1e-6;
	const double MIN_WEIGHT = 0.01;

	namespace
	{
		void Restrict(const Array2<float>& finer, Array2<float>* coarser)
		{
			// --*--|--*--|--*--|--*--
			//  1/8   3/8   3/8   1/8
			//           to
			// -----|-----*-----|-----
			static const std::array<float, 4> centeredKernel = { { 0.125f, 0.375f, 0.375f, 0.125f } };

			// -|----|----|----|----|-
			//      1/4  1/2  1/4
			//           to
			// -|---------|---------|-
			static const std::array<float, 4> staggeredKernel = { { 0.f, 1.f, 0.f, 0.f } };

			std::array<int, 2> kernelSize;
			kernelSize[0] = finer.size().x != 2 * coarser->size().x ? 3 : 4;
			kernelSize[1] = finer.size().y != 2 * coarser->size().y ? 3 : 4;

			std::array<std::array<float, 4>, 2> kernels;
			kernels[0] = (kernelSize[0] == 3) ? staggeredKernel : centeredKernel;
			kernels[1] = (kernelSize[1] == 3) ? staggeredKernel : centeredKernel;

			const Size2 n = coarser->size();
			ParallelRangeFor(ZERO_SIZE, n.x, ZERO_SIZE, n.y,
				[&](size_t iBegin, size_t iEnd, size_t jBegin, size_t jEnd)
			{
				std::array<size_t, 4> jIndices;

				for (size_t j = jBegin; j < jEnd; ++j)
				{
					if (kernelSize[1] == 3)
					{
						jIndices[0] = (j > 0) ? 2 * j - 1 : 2 * j;
						jIndices[1] = 2 * j;
						jIndices[2] = (j + 1 < n.y) ? 2 * j + 1 : 2 * j;
					}
					else
					{
						jIndices[0] = (j > 0) ? 2 * j - 1 : 2 * j;
						jIndices[1] = 2 * j;
						jIndices[2] = 2 * j + 1;
						jIndices[3] = (j + 1 < n.y) ? 2 * j + 2 : 2 * j + 1;
					}

					std::array<size_t, 4> iIndices;
					for (size_t i = iBegin; i < iEnd; ++i)
					{
						if (kernelSize[0] == 3)
						{
							iIndices[0] = (i > 0) ? 2 * i - 1 : 2 * i;
							iIndices[1] = 2 * i;
							iIndices[2] = (i + 1 < n.x) ? 2 * i + 1 : 2 * i;
						}
						else
						{
							iIndices[0] = (i > 0) ? 2 * i - 1 : 2 * i;
							iIndices[1] = 2 * i;
							iIndices[2] = 2 * i + 1;
							iIndices[3] = (i + 1 < n.x) ? 2 * i + 2 : 2 * i + 1;
						}

						float sum = 0.0f;
						for (int y = 0; y < kernelSize[1]; ++y)
						{
							for (int x = 0; x < kernelSize[0]; ++x)
							{
								float w = kernels[0][x] * kernels[1][y];
								sum += w * finer(iIndices[x], jIndices[y]);
							}
						}

						(*coarser)(i, j) = sum;
					}
				}
			});
		}

		void BuildSingleSystem(FDMMatrix2* A, FDMVector2* b,
			const Array2<float>& fluidSDF,
			const Array2<float>& uWeights,
			const Array2<float>& vWeights,
			std::function<Vector2D(const Vector2D&)> boundaryVel,
			const FaceCenteredGrid2& input)
		{
			const Size2 size = input.Resolution();
			const auto uPos = input.GetUPosition();
			const auto vPos = input.GetVPosition();

			const Vector2D invH = 1.0 / input.GridSpacing();
			const Vector2D invHSqr = invH * invH;

			// Build linear system
			A->ParallelForEachIndex([&](size_t i, size_t j)
			{
				auto& row = (*A)(i, j);

				// initialize
				row.center = row.right = row.up = 0.0;
				(*b)(i, j) = 0.0;

				double centerPhi = fluidSDF(i, j);

				if (IsInsideSDF(centerPhi))
				{
					double term;

					if (i + 1 < size.x)
					{
						term = uWeights(i + 1, j) * invHSqr.x;
						double rightPhi = fluidSDF(i + 1, j);

						if (IsInsideSDF(rightPhi))
						{
							row.center += term;
							row.right -= term;
						}
						else
						{
							double theta = FractionInsideSDF(centerPhi, rightPhi);
							theta = std::max(theta, 0.01);
							row.center += term / theta;
						}

						(*b)(i, j) += uWeights(i + 1, j) * input.GetU(i + 1, j) * invH.x;
					}
					else
					{
						(*b)(i, j) += input.GetU(i + 1, j) * invH.x;
					}

					if (i > 0)
					{
						term = uWeights(i, j) * invHSqr.x;
						double leftPhi = fluidSDF(i - 1, j);
						
						if (IsInsideSDF(leftPhi))
						{
							row.center += term;
						}
						else
						{
							double theta = FractionInsideSDF(centerPhi, leftPhi);
							theta = std::max(theta, 0.01);
							row.center += term / theta;
						}

						(*b)(i, j) -= uWeights(i, j) * input.GetU(i, j) * invH.x;
					}
					else
					{
						(*b)(i, j) -= input.GetU(i, j) * invH.x;
					}

					if (j + 1 < size.y)
					{
						term = vWeights(i, j + 1) * invHSqr.y;
						double upPhi = fluidSDF(i, j + 1);
						
						if (IsInsideSDF(upPhi))
						{
							row.center += term;
							row.up -= term;
						}
						else
						{
							double theta = FractionInsideSDF(centerPhi, upPhi);
							theta = std::max(theta, 0.01);
							row.center += term / theta;
						}

						(*b)(i, j) += vWeights(i, j + 1) * input.GetV(i, j + 1) * invH.y;
					}
					else
					{
						(*b)(i, j) += input.GetV(i, j + 1) * invH.y;
					}

					if (j > 0)
					{
						term = vWeights(i, j) * invHSqr.y;
						double downPhi = fluidSDF(i, j - 1);

						if (IsInsideSDF(downPhi))
						{
							row.center += term;
						}
						else
						{
							double theta = FractionInsideSDF(centerPhi, downPhi);
							theta = std::max(theta, 0.01);
							row.center += term / theta;
						}

						(*b)(i, j) -= vWeights(i, j) * input.GetV(i, j) * invH.y;
					}
					else
					{
						(*b)(i, j) -= input.GetV(i, j) * invH.y;
					}

					// Accumulate contributions from the moving boundary
					double boundaryContribution =
						(1.0 - uWeights(i + 1, j)) * boundaryVel(uPos(i + 1, j)).x * invH.x -
						(1.0 - uWeights(i, j)) * boundaryVel(uPos(i, j)).x * invH.x +
						(1.0 - vWeights(i, j + 1)) * boundaryVel(vPos(i, j + 1)).y * invH.y -
						(1.0 - vWeights(i, j)) * boundaryVel(vPos(i, j)).y * invH.y;
					(*b)(i, j) += boundaryContribution;

					// If row.center is near-zero, the cell is likely inside a solid boundary.
					if (row.center < std::numeric_limits<double>::epsilon())
					{
						row.center = 1.0;
						(*b)(i, j) = 0.0;
					}
				}
				else
				{
					row.center = 1.0;
				}
			});
		}

		void BuildSingleSystem(MatrixCSRD* A, VectorND* x, VectorND* b,
			const Array2<float>& fluidSDF,
			const Array2<float>& uWeights,
			const Array2<float>& vWeights,
			std::function<Vector2D(const Vector2D&)> boundaryVel,
			const FaceCenteredGrid2& input)
		{
			const Size2 size = input.Resolution();
			const auto uPos = input.GetUPosition();
			const auto vPos = input.GetVPosition();

			const Vector2D invH = 1.0 / input.GridSpacing();
			const Vector2D invHSqr = invH * invH;

			const auto fluidSDFAcc = fluidSDF.ConstAccessor();

			A->Clear();
			b->Clear();

			size_t numRows = 0;
			Array2<size_t> coordToIndex(size);
			fluidSDF.ForEachIndex([&](size_t i, size_t j)
			{
				const size_t cIdx = fluidSDFAcc.Index(i, j);
				const double centerPhi = fluidSDF[cIdx];

				if (IsInsideSDF(centerPhi))
				{
					coordToIndex[cIdx] = numRows++;
				}
			});

			fluidSDF.ForEachIndex([&](size_t i, size_t j)
			{
				const size_t cIdx = fluidSDFAcc.Index(i, j);
				const double centerPhi = fluidSDF(i, j);

				if (IsInsideSDF(centerPhi))
				{
					double bij = 0.0;

					std::vector<double> row(1, 0.0);
					std::vector<size_t> colIdx(1, coordToIndex[cIdx]);

					double term;

					if (i + 1 < size.x)
					{
						term = uWeights(i + 1, j) * invHSqr.x;
						const double rightPhi = fluidSDF(i + 1, j);

						if (IsInsideSDF(rightPhi))
						{
							row[0] += term;
							row.push_back(-term);
							colIdx.push_back(coordToIndex(i + 1, j));
						}
						else
						{
							double theta = FractionInsideSDF(centerPhi, rightPhi);
							theta = std::max(theta, 0.01);
							row[0] += term / theta;
						}

						bij += uWeights(i + 1, j) * input.GetU(i + 1, j) * invH.x;
					}
					else
					{
						bij += input.GetU(i + 1, j) * invH.x;
					}

					if (i > 0)
					{
						term = uWeights(i, j) * invHSqr.x;
						const double leftPhi = fluidSDF(i - 1, j);
						
						if (IsInsideSDF(leftPhi))
						{
							row[0] += term;
							row.push_back(-term);
							colIdx.push_back(coordToIndex(i - 1, j));
						}
						else
						{
							double theta = FractionInsideSDF(centerPhi, leftPhi);
							theta = std::max(theta, 0.01);
							row[0] += term / theta;
						}
						
						bij -= uWeights(i, j) * input.GetU(i, j) * invH.x;
					}
					else
					{
						bij -= input.GetU(i, j) * invH.x;
					}

					if (j + 1 < size.y)
					{
						term = vWeights(i, j + 1) * invHSqr.y;
						const double upPhi = fluidSDF(i, j + 1);
						
						if (IsInsideSDF(upPhi))
						{
							row[0] += term;
							row.push_back(-term);
							colIdx.push_back(coordToIndex(i, j + 1));
						}
						else
						{
							double theta = FractionInsideSDF(centerPhi, upPhi);
							theta = std::max(theta, 0.01);
							row[0] += term / theta;
						}

						bij += vWeights(i, j + 1) * input.GetV(i, j + 1) * invH.y;
					}
					else
					{
						bij += input.GetV(i, j + 1) * invH.y;
					}

					if (j > 0)
					{
						term = vWeights(i, j) * invHSqr.y;
						const double downPhi = fluidSDF(i, j - 1);
					   
						if (IsInsideSDF(downPhi))
						{
							row[0] += term;
							row.push_back(-term);
							colIdx.push_back(coordToIndex(i, j - 1));
						}
						else
						{
							double theta = FractionInsideSDF(centerPhi, downPhi);
							theta = std::max(theta, 0.01);
							row[0] += term / theta;
						}

						bij -= vWeights(i, j) * input.GetV(i, j) * invH.y;
					}
					else
					{
						bij -= input.GetV(i, j) * invH.y;
					}

					// Accumulate contributions from the moving boundary
					const double boundaryContribution =
						(1.0 - uWeights(i + 1, j)) * boundaryVel(uPos(i + 1, j)).x * invH.x -
						(1.0 - uWeights(i, j)) * boundaryVel(uPos(i, j)).x * invH.x +
						(1.0 - vWeights(i, j + 1)) * boundaryVel(vPos(i, j + 1)).y * invH.y -
						(1.0 - vWeights(i, j)) * boundaryVel(vPos(i, j)).y * invH.y;
					bij += boundaryContribution;

					// If row.center is near-zero, the cell is likely inside a solid boundary.
					if (row[0] < std::numeric_limits<double>::epsilon())
					{
						row[0] = 1.0;
						bij = 0.0;
					}

					A->AddRow(row, colIdx);
					b->Append(bij);
				}
			});

			x->Resize(b->size(), 0.0);
		}
	}

	GridFractionalSinglePhasePressureSolver2::GridFractionalSinglePhasePressureSolver2()
	{
		m_systemSolver = std::make_shared<FDMICCGSolver2>(100, DEFAULT_TOLERANCE);
	}

	GridFractionalSinglePhasePressureSolver2::~GridFractionalSinglePhasePressureSolver2()
	{
		// Do nothing
	}

	void GridFractionalSinglePhasePressureSolver2::Solve(
		const FaceCenteredGrid2& input,
		double timeIntervalInSeconds,
		FaceCenteredGrid2* output,
		const ScalarField2& boundarySDF,
		const VectorField2& boundaryVelocity,
		const ScalarField2& fluidSDF,
		bool useCompressed)
	{
		UNUSED_VARIABLE(timeIntervalInSeconds);

		BuildWeights(input, boundarySDF, boundaryVelocity, fluidSDF);
		BuildSystem(input, useCompressed);

		if (m_systemSolver != nullptr)
		{
			// Solve the system
			if (m_mgSystemSolver == nullptr)
			{
				if (useCompressed)
				{
					m_system.Clear();
					m_systemSolver->SolveCompressed(&m_compSystem);
					DecompressSolution();
				}
				else
				{
					m_compSystem.Clear();
					m_systemSolver->Solve(&m_system);
				}
			}
			else
			{
				m_mgSystemSolver->Solve(&m_mgSystem);
			}

			// Apply pressure gradient
			ApplyPressureGradient(input, output);
		}
	}

	GridBoundaryConditionSolver2Ptr GridFractionalSinglePhasePressureSolver2::SuggestedBoundaryConditionSolver() const
	{
		return std::make_shared<GridFractionalBoundaryConditionSolver2>();
	}

	const FDMLinearSystemSolver2Ptr& GridFractionalSinglePhasePressureSolver2::GetLinearSystemSolver() const
	{
		return m_systemSolver;
	}

	void GridFractionalSinglePhasePressureSolver2::SetLinearSystemSolver(const FDMLinearSystemSolver2Ptr& solver)
	{
		m_systemSolver = solver;
		m_mgSystemSolver = std::dynamic_pointer_cast<FDMMGSolver2>(m_systemSolver);

		if (m_mgSystemSolver == nullptr)
		{
			// In case of non-mg system, use flat structure.
			m_mgSystem.Clear();
		}
		else
		{
			// In case of mg system, use multi-level structure.
			m_system.Clear();
			m_compSystem.Clear();
		}
	}

	const FDMVector2& GridFractionalSinglePhasePressureSolver2::GetPressure() const
	{
		if (m_mgSystemSolver == nullptr)
		{
			return m_system.x;
		}

		return m_mgSystem.x.levels.front();
	}

	void GridFractionalSinglePhasePressureSolver2::BuildWeights(
		const FaceCenteredGrid2& input,
		const ScalarField2& boundarySDF,
		const VectorField2& boundaryVelocity,
		const ScalarField2& fluidSDF)
	{
		const auto size = input.Resolution();

		// Build levels
		size_t maxLevels = 1;
		if (m_mgSystemSolver != nullptr)
		{
			maxLevels = m_mgSystemSolver->GetParams().maxNumberOfLevels;
		}

		FDMMGUtils2::ResizeArrayWithFinest(size, maxLevels, &m_fluidSDF);
		m_uWeights.resize(m_fluidSDF.size());
		m_vWeights.resize(m_fluidSDF.size());
		for (size_t l = 0; l < m_fluidSDF.size(); ++l)
		{
			m_uWeights[l].Resize(m_fluidSDF[l].size() + Size2(1, 0));
			m_vWeights[l].Resize(m_fluidSDF[l].size() + Size2(0, 1));
		}

		// Build top-level grids
		auto cellPos = input.CellCenterPosition();
		auto uPos = input.GetUPosition();
		auto vPos = input.GetVPosition();
		m_boundaryVel = boundaryVelocity.Sampler();
		Vector2D h = input.GridSpacing();

		m_fluidSDF[0].ParallelForEachIndex([&](size_t i, size_t j)
		{
			m_fluidSDF[0](i, j) = static_cast<float>(fluidSDF.Sample(cellPos(i, j)));
		});

		m_uWeights[0].ParallelForEachIndex([&](size_t i, size_t j)
		{
			Vector2D pt = uPos(i, j);
			double phi0 = boundarySDF.Sample(pt - Vector2D(0.5 * h.x, 0.0));
			double phi1 = boundarySDF.Sample(pt + Vector2D(0.5 * h.x, 0.0));
			double frac = FractionInsideSDF(phi0, phi1);
			double weight = std::clamp(1.0 - frac, 0.0, 1.0);

			// Clamp non-zero weight to kMinWeight. Having nearly-zero element
			// in the matrix can be an issue.
			if (weight < MIN_WEIGHT && weight > 0.0)
			{
				weight = MIN_WEIGHT;
			}

			m_uWeights[0](i, j) = static_cast<float>(weight);
		});

		m_vWeights[0].ParallelForEachIndex([&](size_t i, size_t j)
		{
			Vector2D pt = vPos(i, j);
			double phi0 = boundarySDF.Sample(pt - Vector2D(0.0, 0.5 * h.y));
			double phi1 = boundarySDF.Sample(pt + Vector2D(0.0, 0.5 * h.y));
			double frac = FractionInsideSDF(phi0, phi1);
			double weight = std::clamp(1.0 - frac, 0.0, 1.0);

			// Clamp non-zero weight to kMinWeight. Having nearly-zero element
			// in the matrix can be an issue.
			if (weight < MIN_WEIGHT && weight > 0.0)
			{
				weight = MIN_WEIGHT;
			}

			m_vWeights[0](i, j) = static_cast<float>(weight);
		});

		// Build sub-levels
		for (size_t l = 1; l < m_fluidSDF.size(); ++l)
		{
			const auto& finerFluidSDF = m_fluidSDF[l - 1];
			auto& coarserFluidSDF = m_fluidSDF[l];
			const auto& finerUWeight = m_uWeights[l - 1];
			auto& coarserUWeight = m_uWeights[l];
			const auto& finerVWeight = m_vWeights[l - 1];
			auto& coarserVWeight = m_vWeights[l];

			// Fluid SDF
			Restrict(finerFluidSDF, &coarserFluidSDF);
			Restrict(finerUWeight, &coarserUWeight);
			Restrict(finerVWeight, &coarserVWeight);
		}
	}

	void GridFractionalSinglePhasePressureSolver2::DecompressSolution()
	{
		const auto acc = m_fluidSDF[0].ConstAccessor();
		m_system.x.Resize(acc.size());

		size_t row = 0;
		m_fluidSDF[0].ForEachIndex([&](size_t i, size_t j)
		{
			if (IsInsideSDF(acc(i, j)))
			{
				m_system.x(i, j) = m_compSystem.x[row];
				++row;
			}
		});
	}

	void GridFractionalSinglePhasePressureSolver2::BuildSystem(const FaceCenteredGrid2& input, bool useCompressed)
	{
		const Size2 size = input.Resolution();
		size_t numLevels = 1;

		if (m_mgSystemSolver == nullptr)
		{
			if (!useCompressed)
			{
				m_system.Resize(size);
			}
		}
		else
		{
			// Build levels
			const size_t maxLevels = m_mgSystemSolver->GetParams().maxNumberOfLevels;
			FDMMGUtils2::ResizeArrayWithFinest(size, maxLevels, &m_mgSystem.A.levels);
			FDMMGUtils2::ResizeArrayWithFinest(size, maxLevels, &m_mgSystem.x.levels);
			FDMMGUtils2::ResizeArrayWithFinest(size, maxLevels, &m_mgSystem.b.levels);
			
			numLevels = m_mgSystem.A.levels.size();
		}

		// Build top level
		const FaceCenteredGrid2* finer = &input;
		if (m_mgSystemSolver == nullptr)
		{
			if (useCompressed)
			{
				BuildSingleSystem(
					&m_compSystem.A, &m_compSystem.x, &m_compSystem.b,
					m_fluidSDF[0], m_uWeights[0], m_vWeights[0],
					m_boundaryVel, *finer);
			}
			else
			{
				BuildSingleSystem(
					&m_system.A, &m_system.b,
					m_fluidSDF[0], m_uWeights[0], m_vWeights[0],
					m_boundaryVel, *finer);
			}
		}
		else
		{
			BuildSingleSystem(
				&m_mgSystem.A.levels.front(), &m_mgSystem.b.levels.front(),
				m_fluidSDF[0], m_uWeights[0], m_vWeights[0],
				m_boundaryVel, *finer);
		}

		// Build sub-levels
		FaceCenteredGrid2 coarser;
		for (size_t l = 1; l < numLevels; ++l)
		{
			auto res = finer->Resolution();
			auto h = finer->GridSpacing();
			const auto o = finer->Origin();
			res.x = res.x >> 1;
			res.y = res.y >> 1;
			h *= 2.0;
			
			// Down sample
			coarser.Resize(res, h, o);
			coarser.Fill(finer->Sampler());
			
			BuildSingleSystem(
				&m_mgSystem.A.levels[l], &m_mgSystem.b.levels[l],
				m_fluidSDF[l], m_uWeights[l], m_vWeights[l],
				m_boundaryVel, coarser);
			
			finer = &coarser;
		}
	}

	void GridFractionalSinglePhasePressureSolver2::ApplyPressureGradient(const FaceCenteredGrid2& input, FaceCenteredGrid2* output)
	{
		Size2 size = input.Resolution();
		auto u = input.GetUConstAccessor();
		auto v = input.GetVConstAccessor();
		auto u0 = output->GetUAccessor();
		auto v0 = output->GetVAccessor();

		const auto& x = GetPressure();

		Vector2D invH = 1.0 / input.GridSpacing();

		x.ParallelForEachIndex([&](size_t i, size_t j)
		{
			double centerPhi = m_fluidSDF[0](i, j);

			if (i + 1 < size.x && m_uWeights[0](i + 1, j) > 0.0 &&
				(IsInsideSDF(centerPhi) || IsInsideSDF(m_fluidSDF[0](i + 1, j))))
			{
				double rightPhi = m_fluidSDF[0](i + 1, j);
				double theta = FractionInsideSDF(centerPhi, rightPhi);
				theta = std::max(theta, 0.01);

				u0(i + 1, j) = u(i + 1, j) + invH.x / theta * (x(i + 1, j) - x(i, j));
			}

			if (j + 1 < size.y && m_vWeights[0](i, j + 1) > 0.0 &&
				(IsInsideSDF(centerPhi) || IsInsideSDF(m_fluidSDF[0](i, j + 1))))
			{
				double upPhi = m_fluidSDF[0](i, j + 1);
				double theta = FractionInsideSDF(centerPhi, upPhi);
				theta = std::max(theta, 0.01);

				v0(i, j + 1) = v(i, j + 1) + invH.y / theta * (x(i, j + 1) - x(i, j));
			}
		});
	}
}