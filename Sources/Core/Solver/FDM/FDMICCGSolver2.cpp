// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Math/CG.hpp>
#include <Core/Solver/FDM/FDMICCGSolver2.hpp>
#include <Core/Utils/Logging.hpp>

namespace CubbyFlow
{
void FDMICCGSolver2::Preconditioner::Build(const FDMMatrix2& matrix)
{
    const Size2 size = matrix.size();
    A = matrix.ConstAccessor();

    d.Resize(size, 0.0);
    y.Resize(size, 0.0);

    matrix.ForEachIndex([&](size_t i, size_t j) {
        double denom =
            matrix(i, j).center -
            ((i > 0) ? Square(matrix(i - 1, j).right) * d(i - 1, j) : 0.0) -
            ((j > 0) ? Square(matrix(i, j - 1).up) * d(i, j - 1) : 0.0);

        if (std::fabs(denom) > 0.0)
        {
            d(i, j) = 1.0 / denom;
        }
        else
        {
            d(i, j) = 0.0;
        }
    });
}

void FDMICCGSolver2::Preconditioner::Solve(const FDMVector2& b, FDMVector2* x)
{
    const Size2 size = b.size();
    const ssize_t sx = static_cast<ssize_t>(size.x);
    const ssize_t sy = static_cast<ssize_t>(size.y);

    b.ForEachIndex([&](size_t i, size_t j) {
        y(i, j) = (b(i, j) - ((i > 0) ? A(i - 1, j).right * y(i - 1, j) : 0.0) -
                   ((j > 0) ? A(i, j - 1).up * y(i, j - 1) : 0.0)) *
                  d(i, j);
    });

    for (ssize_t j = sy - 1; j >= 0; --j)
    {
        for (ssize_t i = sx - 1; i >= 0; --i)
        {
            (*x)(i, j) =
                (y(i, j) -
                 ((i + 1 < sx) ? A(i, j).right * (*x)(i + 1, j) : 0.0) -
                 ((j + 1 < sy) ? A(i, j).up * (*x)(i, j + 1) : 0.0)) *
                d(i, j);
        }
    }
}

void FDMICCGSolver2::PreconditionerCompressed::Build(const MatrixCSRD& matrix)
{
    const size_t size = matrix.Cols();
    A = &matrix;

    d.Resize(size, 0.0);
    y.Resize(size, 0.0);

    const auto rp = A->RowPointersBegin();
    const auto ci = A->ColumnIndicesBegin();
    const auto nnz = A->NonZeroBegin();

    d.ForEachIndex([&](size_t i) {
        const size_t rowBegin = rp[i];
        const size_t rowEnd = rp[i + 1];

        double denom = 0.0;
        for (size_t jj = rowBegin; jj < rowEnd; ++jj)
        {
            size_t j = ci[jj];

            if (j == i)
            {
                denom += nnz[jj];
            }
            else if (j < i)
            {
                denom -= Square(nnz[jj]) * d[j];
            }
        }

        if (std::fabs(denom) > 0.0)
        {
            d[i] = 1.0 / denom;
        }
        else
        {
            d[i] = 0.0;
        }
    });
}

void FDMICCGSolver2::PreconditionerCompressed::Solve(const VectorND& b,
                                                     VectorND* x)
{
    const ssize_t size = static_cast<ssize_t>(b.size());

    const auto rp = A->RowPointersBegin();
    const auto ci = A->ColumnIndicesBegin();
    const auto nnz = A->NonZeroBegin();

    b.ForEachIndex([&](size_t i) {
        const size_t rowBegin = rp[i];
        const size_t rowEnd = rp[i + 1];

        double sum = b[i];
        for (size_t jj = rowBegin; jj < rowEnd; ++jj)
        {
            size_t j = ci[jj];

            if (j < i)
            {
                sum -= nnz[jj] * y[j];
            }
        }

        y[i] = sum * d[i];
    });

    for (ssize_t i = size - 1; i >= 0; --i)
    {
        const size_t rowBegin = rp[i];
        const size_t rowEnd = rp[i + 1];

        double sum = y[i];
        for (size_t jj = rowBegin; jj < rowEnd; ++jj)
        {
            const ssize_t j = static_cast<ssize_t>(ci[jj]);

            if (j > i)
            {
                sum -= nnz[jj] * (*x)[j];
            }
        }

        (*x)[i] = sum * d[i];
    }
}

FDMICCGSolver2::FDMICCGSolver2(unsigned int maxNumberOfIterations,
                               double tolerance)
    : m_maxNumberOfIterations(maxNumberOfIterations),
      m_lastNumberOfIterations(0),
      m_tolerance(tolerance),
      m_lastResidualNorm(std::numeric_limits<double>::max())
{
    // Do nothing
}

bool FDMICCGSolver2::Solve(FDMLinearSystem2* system)
{
    FDMMatrix2& matrix = system->A;
    FDMVector2& solution = system->x;
    FDMVector2& rhs = system->b;

    assert(matrix.size() == rhs.size());
    assert(matrix.size() == solution.size());

    ClearCompressedVectors();

    const Size2 size = matrix.size();
    m_r.Resize(size);
    m_d.Resize(size);
    m_q.Resize(size);
    m_s.Resize(size);

    system->x.Set(0.0);
    m_r.Set(0.0);
    m_d.Set(0.0);
    m_q.Set(0.0);
    m_s.Set(0.0);

    m_precond.Build(matrix);

    PCG<FDMBLAS2, Preconditioner>(matrix, rhs, m_maxNumberOfIterations,
                                  m_tolerance, &m_precond, &solution, &m_r,
                                  &m_d, &m_q, &m_s, &m_lastNumberOfIterations,
                                  &m_lastResidualNorm);

    CUBBYFLOW_INFO << "Residual after solving ICCG: " << m_lastResidualNorm
                   << " Number of ICCG iterations: "
                   << m_lastNumberOfIterations;

    return (m_lastResidualNorm <= m_tolerance) ||
           (m_lastNumberOfIterations < m_maxNumberOfIterations);
}

bool FDMICCGSolver2::SolveCompressed(FDMCompressedLinearSystem2* system)
{
    MatrixCSRD& matrix = system->A;
    VectorND& solution = system->x;
    VectorND& rhs = system->b;

    ClearUncompressedVectors();

    const size_t size = solution.size();
    m_rComp.Resize(size);
    m_dComp.Resize(size);
    m_qComp.Resize(size);
    m_sComp.Resize(size);

    system->x.Set(0.0);
    m_rComp.Set(0.0);
    m_dComp.Set(0.0);
    m_qComp.Set(0.0);
    m_sComp.Set(0.0);

    m_precondComp.Build(matrix);

    PCG<FDMCompressedBLAS2, PreconditionerCompressed>(
        matrix, rhs, m_maxNumberOfIterations, m_tolerance, &m_precondComp,
        &solution, &m_rComp, &m_dComp, &m_qComp, &m_sComp,
        &m_lastNumberOfIterations, &m_lastResidualNorm);

    CUBBYFLOW_INFO << "Residual after solving ICCG: " << m_lastResidualNorm
                   << " Number of ICCG iterations: "
                   << m_lastNumberOfIterations;

    return (m_lastResidualNorm <= m_tolerance) ||
           (m_lastNumberOfIterations < m_maxNumberOfIterations);
}

unsigned int FDMICCGSolver2::GetMaxNumberOfIterations() const
{
    return m_maxNumberOfIterations;
}

unsigned int FDMICCGSolver2::GetLastNumberOfIterations() const
{
    return m_lastNumberOfIterations;
}

double FDMICCGSolver2::GetTolerance() const
{
    return m_tolerance;
}

double FDMICCGSolver2::GetLastResidual() const
{
    return m_lastResidualNorm;
}

void FDMICCGSolver2::ClearUncompressedVectors()
{
    m_r.Clear();
    m_d.Clear();
    m_q.Clear();
    m_s.Clear();
}

void FDMICCGSolver2::ClearCompressedVectors()
{
    m_r.Clear();
    m_d.Clear();
    m_q.Clear();
    m_s.Clear();
}
}  // namespace CubbyFlow