// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/FDM/FDMICCGSolver3.hpp>
#include <Core/Solver/Grid/GridBlockedBoundaryConditionSolver3.hpp>
#include <Core/Solver/Grid/GridSinglePhasePressureSolver3.hpp>
#include <Core/Utils/LevelSetUtils.hpp>

namespace CubbyFlow
{
const char FLUID = 0;
const char AIR = 1;
const char BOUNDARY = 2;

const double DEFAULT_TOLERANCE = 1e-6;

namespace
{
void BuildSingleSystem(FDMMatrix3* A, FDMVector3* b,
                       const Array3<char>& markers,
                       const FaceCenteredGrid3& input)
{
    Vector3UZ size = input.Resolution();
    const Vector3D invH = 1.0 / input.GridSpacing();
    Vector3D invHSqr = ElemMul(invH, invH);

    // Build linear system
    ParallelForEachIndex(A->Size(), [&](size_t i, size_t j, size_t k) {
        FDMMatrixRow3& row = (*A)(i, j, k);

        // initialize
        row.center = row.right = row.up = row.front = 0.0;
        (*b)(i, j, k) = 0.0;

        if (markers(i, j, k) == FLUID)
        {
            (*b)(i, j, k) = input.DivergenceAtCellCenter(i, j, k);

            if (i + 1 < size.x && markers(i + 1, j, k) != BOUNDARY)
            {
                row.center += invHSqr.x;
                if (markers(i + 1, j, k) == FLUID)
                {
                    row.right -= invHSqr.x;
                }
            }

            if (i > 0 && markers(i - 1, j, k) != BOUNDARY)
            {
                row.center += invHSqr.x;
            }

            if (j + 1 < size.y && markers(i, j + 1, k) != BOUNDARY)
            {
                row.center += invHSqr.y;
                if (markers(i, j + 1, k) == FLUID)
                {
                    row.up -= invHSqr.y;
                }
            }

            if (j > 0 && markers(i, j - 1, k) != BOUNDARY)
            {
                row.center += invHSqr.y;
            }

            if (k + 1 < size.z && markers(i, j, k + 1) != BOUNDARY)
            {
                row.center += invHSqr.z;
                if (markers(i, j, k + 1) == FLUID)
                {
                    row.front -= invHSqr.z;
                }
            }

            if (k > 0 && markers(i, j, k - 1) != BOUNDARY)
            {
                row.center += invHSqr.z;
            }
        }
        else
        {
            row.center = 1.0;
        }
    });
}

void BuildSingleSystem(MatrixCSRD* A, VectorND* x, VectorND* b,
                       const Array3<char>& markers,
                       const FaceCenteredGrid3& input)
{
    Vector3UZ size = input.Resolution();
    const Vector3D invH = 1.0 / input.GridSpacing();
    Vector3D invHSqr = ElemMul(invH, invH);

    ConstArrayView3<char> markerAcc{ markers };

    A->Clear();
    b->Clear();

    size_t numRows = 0;
    Array3<size_t> coordToIndex{ size };
    ForEachIndex(markers.Size(), [&](size_t i, size_t j, size_t k) {
        const size_t cIdx = markerAcc.Index(i, j, k);

        if (markerAcc[cIdx] == FLUID)
        {
            coordToIndex[cIdx] = numRows++;
        }
    });

    ForEachIndex(markers.Size(), [&](size_t i, size_t j, size_t k) {
        const size_t cIdx = markerAcc.Index(i, j, k);

        if (markerAcc[cIdx] == FLUID)
        {
            b->AddElement(input.DivergenceAtCellCenter(i, j, k));

            std::vector<double> row(1, 0.0);
            std::vector<size_t> colIdx(1, coordToIndex[cIdx]);

            if (i + 1 < size.x && markers(i + 1, j, k) != BOUNDARY)
            {
                row[0] += invHSqr.x;
                const size_t rIdx = markerAcc.Index(i + 1, j, k);

                if (markers[rIdx] == FLUID)
                {
                    row.push_back(-invHSqr.x);
                    colIdx.push_back(coordToIndex[rIdx]);
                }
            }

            if (i > 0 && markers(i - 1, j, k) != BOUNDARY)
            {
                row[0] += invHSqr.x;
                const size_t lIdx = markerAcc.Index(i - 1, j, k);

                if (markers[lIdx] == FLUID)
                {
                    row.push_back(-invHSqr.x);
                    colIdx.push_back(coordToIndex[lIdx]);
                }
            }

            if (j + 1 < size.y && markers(i, j + 1, k) != BOUNDARY)
            {
                row[0] += invHSqr.y;
                const size_t uIdx = markerAcc.Index(i, j + 1, k);

                if (markers[uIdx] == FLUID)
                {
                    row.push_back(-invHSqr.y);
                    colIdx.push_back(coordToIndex[uIdx]);
                }
            }

            if (j > 0 && markers(i, j - 1, k) != BOUNDARY)
            {
                row[0] += invHSqr.y;
                const size_t dIdx = markerAcc.Index(i, j - 1, k);

                if (markers[dIdx] == FLUID)
                {
                    row.push_back(-invHSqr.y);
                    colIdx.push_back(coordToIndex[dIdx]);
                }
            }

            if (k + 1 < size.z && markers(i, j, k + 1) != BOUNDARY)
            {
                row[0] += invHSqr.z;
                const size_t fIdx = markerAcc.Index(i, j, k + 1);

                if (markers[fIdx] == FLUID)
                {
                    row.push_back(-invHSqr.z);
                    colIdx.push_back(coordToIndex[fIdx]);
                }
            }

            if (k > 0 && markers(i, j, k - 1) != BOUNDARY)
            {
                row[0] += invHSqr.z;
                const size_t bIdx = markerAcc.Index(i, j, k - 1);

                if (markers[bIdx] == FLUID)
                {
                    row.push_back(-invHSqr.z);
                    colIdx.push_back(coordToIndex[bIdx]);
                }
            }

            A->AddRow(row, colIdx);
        }
    });

    x->Resize(b->GetRows(), 0.0);
}
}  // namespace

GridSinglePhasePressureSolver3::GridSinglePhasePressureSolver3()
{
    m_systemSolver = std::make_shared<FDMICCGSolver3>(100, DEFAULT_TOLERANCE);
}

void GridSinglePhasePressureSolver3::Solve(const FaceCenteredGrid3& input,
                                           double timeIntervalInSeconds,
                                           FaceCenteredGrid3* output,
                                           const ScalarField3& boundarySDF,
                                           const VectorField3& boundaryVelocity,
                                           const ScalarField3& fluidSDF,
                                           bool useCompressed)
{
    UNUSED_VARIABLE(timeIntervalInSeconds);
    UNUSED_VARIABLE(boundaryVelocity);

    const GridDataPositionFunc<3> pos = input.CellCenterPosition();

    BuildMarkers(input.Resolution(), pos, boundarySDF, fluidSDF);
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

GridBoundaryConditionSolver3Ptr
GridSinglePhasePressureSolver3::SuggestedBoundaryConditionSolver() const
{
    return std::make_shared<GridBlockedBoundaryConditionSolver3>();
}

const FDMLinearSystemSolver3Ptr&
GridSinglePhasePressureSolver3::GetLinearSystemSolver() const
{
    return m_systemSolver;
}

void GridSinglePhasePressureSolver3::SetLinearSystemSolver(
    const FDMLinearSystemSolver3Ptr& solver)
{
    m_systemSolver = solver;
    m_mgSystemSolver = std::dynamic_pointer_cast<FDMMGSolver3>(m_systemSolver);

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

const FDMVector3& GridSinglePhasePressureSolver3::GetPressure() const
{
    if (m_mgSystemSolver == nullptr)
    {
        return m_system.x;
    }

    return m_mgSystem.x.levels.front();
}

void GridSinglePhasePressureSolver3::BuildMarkers(
    const Vector3UZ& size,
    const std::function<Vector3D(size_t, size_t, size_t)>& pos,
    const ScalarField3& boundarySDF, const ScalarField3& fluidSDF)
{
    // Build levels
    size_t maxLevels = 1;
    if (m_mgSystemSolver != nullptr)
    {
        maxLevels = m_mgSystemSolver->GetParams().maxNumberOfLevels;
    }
    FDMMGUtils3::ResizeArrayWithFinest(size, maxLevels, &m_markers);

    // Build top-level markers
    ParallelForEachIndex(m_markers[0].Size(),
                         [&](size_t i, size_t j, size_t k) {
                             const Vector3D pt = pos(i, j, k);

                             if (IsInsideSDF(boundarySDF.Sample(pt)))
                             {
                                 m_markers[0](i, j, k) = BOUNDARY;
                             }
                             else if (IsInsideSDF(fluidSDF.Sample(pt)))
                             {
                                 m_markers[0](i, j, k) = FLUID;
                             }
                             else
                             {
                                 m_markers[0](i, j, k) = AIR;
                             }
                         });

    // Build sub-level markers
    for (size_t l = 1; l < m_markers.size(); ++l)
    {
        const Array3<char>& finer = m_markers[l - 1];
        Array3<char>& coarser = m_markers[l];
        const Vector3UZ n = coarser.Size();

        ParallelRangeFor(
            ZERO_SIZE, n.x, ZERO_SIZE, n.y, ZERO_SIZE, n.z,
            [&](size_t iBegin, size_t iEnd, size_t jBegin, size_t jEnd,
                size_t kBegin, size_t kEnd) {
                std::array<size_t, 4> kIndices{};

                for (size_t k = kBegin; k < kEnd; ++k)
                {
                    kIndices[0] = (k > 0) ? 2 * k - 1 : 2 * k;
                    kIndices[1] = 2 * k;
                    kIndices[2] = 2 * k + 1;
                    kIndices[3] = (k + 1 < n.z) ? 2 * k + 2 : 2 * k + 1;

                    std::array<size_t, 4> jIndices{};

                    for (size_t j = jBegin; j < jEnd; ++j)
                    {
                        jIndices[0] = (j > 0) ? 2 * j - 1 : 2 * j;
                        jIndices[1] = 2 * j;
                        jIndices[2] = 2 * j + 1;
                        jIndices[3] = (j + 1 < n.y) ? 2 * j + 2 : 2 * j + 1;

                        std::array<size_t, 4> iIndices{};
                        for (size_t i = iBegin; i < iEnd; ++i)
                        {
                            iIndices[0] = (i > 0) ? 2 * i - 1 : 2 * i;
                            iIndices[1] = 2 * i;
                            iIndices[2] = 2 * i + 1;
                            iIndices[3] = (i + 1 < n.x) ? 2 * i + 2 : 2 * i + 1;

                            int cnt[3] = { 0, 0, 0 };
                            for (size_t z = 0; z < 4; ++z)
                            {
                                for (size_t y = 0; y < 4; ++y)
                                {
                                    for (size_t x = 0; x < 4; ++x)
                                    {
                                        const char f =
                                            finer(iIndices[x], jIndices[y],
                                                  kIndices[z]);
                                        if (f == BOUNDARY)
                                        {
                                            ++cnt[static_cast<int>(BOUNDARY)];
                                        }
                                        else if (f == FLUID)
                                        {
                                            ++cnt[static_cast<int>(FLUID)];
                                        }
                                        else
                                        {
                                            ++cnt[static_cast<int>(AIR)];
                                        }
                                    }
                                }
                            }

                            coarser(i, j, k) = static_cast<char>(
                                ArgMax3(cnt[0], cnt[1], cnt[2]));
                        }
                    }
                }
            });
    }
}

void GridSinglePhasePressureSolver3::DecompressSolution()
{
    ConstArrayView3<char> acc{ m_markers[0] };
    m_system.x.Resize(acc.Size());

    size_t row = 0;
    ForEachIndex(m_markers[0].Size(), [&](size_t i, size_t j, size_t k) {
        if (acc(i, j, k) == FLUID)
        {
            m_system.x(i, j, k) = m_compSystem.x[row];
            ++row;
        }
    });
}

void GridSinglePhasePressureSolver3::BuildSystem(const FaceCenteredGrid3& input,
                                                 bool useCompressed)
{
    const Vector3UZ size = input.Resolution();
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
        const size_t maxLevels =
            m_mgSystemSolver->GetParams().maxNumberOfLevels;
        FDMMGUtils3::ResizeArrayWithFinest(size, maxLevels,
                                           &m_mgSystem.A.levels);
        FDMMGUtils3::ResizeArrayWithFinest(size, maxLevels,
                                           &m_mgSystem.x.levels);
        FDMMGUtils3::ResizeArrayWithFinest(size, maxLevels,
                                           &m_mgSystem.b.levels);

        numLevels = m_mgSystem.A.levels.size();
    }

    // Build top level
    const FaceCenteredGrid3* finer = &input;
    if (m_mgSystemSolver == nullptr)
    {
        if (useCompressed)
        {
            BuildSingleSystem(&m_compSystem.A, &m_compSystem.x, &m_compSystem.b,
                              m_markers[0], *finer);
        }
        else
        {
            BuildSingleSystem(&m_system.A, &m_system.b, m_markers[0], *finer);
        }
    }
    else
    {
        BuildSingleSystem(&m_mgSystem.A.levels.front(),
                          &m_mgSystem.b.levels.front(), m_markers[0], *finer);
    }

    // Build sub-levels
    FaceCenteredGrid3 coarser;
    for (size_t l = 1; l < numLevels; ++l)
    {
        Vector3UZ res = finer->Resolution();
        Vector3D h = finer->GridSpacing();
        const Vector3D& o = finer->Origin();
        res.x = res.x >> 1;
        res.y = res.y >> 1;
        res.z = res.z >> 1;
        h *= 2.0;

        // Down sample
        coarser.Resize(res, h, o);
        coarser.Fill(finer->Sampler());

        BuildSingleSystem(&m_mgSystem.A.levels[l], &m_mgSystem.b.levels[l],
                          m_markers[l], coarser);

        finer = &coarser;
    }
}

void GridSinglePhasePressureSolver3::ApplyPressureGradient(
    const FaceCenteredGrid3& input, FaceCenteredGrid3* output)
{
    Vector3UZ size = input.Resolution();
    ConstArrayView3<double> u = input.UView();
    ConstArrayView3<double> v = input.VView();
    ConstArrayView3<double> w = input.WView();
    ArrayView3<double> u0 = output->UView();
    ArrayView3<double> v0 = output->VView();
    ArrayView3<double> w0 = output->WView();

    const FDMVector3& x = GetPressure();

    Vector3D invH = 1.0 / input.GridSpacing();

    ParallelForEachIndex(x.Size(), [&](size_t i, size_t j, size_t k) {
        if (m_markers[0](i, j, k) == FLUID)
        {
            if (i + 1 < size.x && m_markers[0](i + 1, j, k) != BOUNDARY)
            {
                u0(i + 1, j, k) =
                    u(i + 1, j, k) + invH.x * (x(i + 1, j, k) - x(i, j, k));
            }
            if (j + 1 < size.y && m_markers[0](i, j + 1, k) != BOUNDARY)
            {
                v0(i, j + 1, k) =
                    v(i, j + 1, k) + invH.y * (x(i, j + 1, k) - x(i, j, k));
            }
            if (k + 1 < size.z && m_markers[0](i, j, k + 1) != BOUNDARY)
            {
                w0(i, j, k + 1) =
                    w(i, j, k + 1) + invH.z * (x(i, j, k + 1) - x(i, j, k));
            }
        }
    });
}
}  // namespace CubbyFlow