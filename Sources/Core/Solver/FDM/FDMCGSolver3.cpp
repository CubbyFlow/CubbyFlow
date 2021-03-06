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
#include <Core/Solver/FDM/FDMCGSolver3.hpp>

namespace CubbyFlow
{
FDMCGSolver3::FDMCGSolver3(unsigned int maxNumberOfIterations, double tolerance)
    : m_maxNumberOfIterations{ maxNumberOfIterations },
      m_lastNumberOfIterations{ 0 },
      m_tolerance{ tolerance },
      m_lastResidual{ std::numeric_limits<double>::max() }
{
    // Do nothing
}

bool FDMCGSolver3::Solve(FDMLinearSystem3* system)
{
    FDMMatrix3& matrix = system->A;
    FDMVector3& solution = system->x;
    FDMVector3& rhs = system->b;

    assert(matrix.Size() == rhs.Size());
    assert(matrix.Size() == solution.Size());

    ClearCompressedVectors();

    const Vector3UZ& size = matrix.Size();
    m_r.Resize(size);
    m_d.Resize(size);
    m_q.Resize(size);
    m_s.Resize(size);

    system->x.Fill(0.0);
    m_r.Fill(0.0);
    m_d.Fill(0.0);
    m_q.Fill(0.0);
    m_s.Fill(0.0);

    CG<FDMBLAS3>(matrix, rhs, m_maxNumberOfIterations, m_tolerance, &solution,
                 &m_r, &m_d, &m_q, &m_s, &m_lastNumberOfIterations,
                 &m_lastResidual);

    return (m_lastResidual <= m_tolerance) ||
           (m_lastNumberOfIterations < m_maxNumberOfIterations);
}

bool FDMCGSolver3::SolveCompressed(FDMCompressedLinearSystem3* system)
{
    MatrixCSRD& matrix = system->A;
    VectorND& solution = system->x;
    VectorND& rhs = system->b;

    ClearUncompressedVectors();

    const size_t size = solution.GetRows();
    m_rComp.Resize(size);
    m_dComp.Resize(size);
    m_qComp.Resize(size);
    m_sComp.Resize(size);

    system->x.Fill(0.0);
    m_rComp.Fill(0.0);
    m_dComp.Fill(0.0);
    m_qComp.Fill(0.0);
    m_sComp.Fill(0.0);

    CG<FDMCompressedBLAS3>(matrix, rhs, m_maxNumberOfIterations, m_tolerance,
                           &solution, &m_rComp, &m_dComp, &m_qComp, &m_sComp,
                           &m_lastNumberOfIterations, &m_lastResidual);

    return (m_lastResidual <= m_tolerance) ||
           (m_lastNumberOfIterations < m_maxNumberOfIterations);
}

unsigned int FDMCGSolver3::GetMaxNumberOfIterations() const
{
    return m_maxNumberOfIterations;
}

unsigned int FDMCGSolver3::GetLastNumberOfIterations() const
{
    return m_lastNumberOfIterations;
}

double FDMCGSolver3::GetTolerance() const
{
    return m_tolerance;
}

double FDMCGSolver3::GetLastResidual() const
{
    return m_lastResidual;
}

void FDMCGSolver3::ClearUncompressedVectors()
{
    m_r.Clear();
    m_d.Clear();
    m_q.Clear();
    m_s.Clear();
}

void FDMCGSolver3::ClearCompressedVectors()
{
    m_rComp.Clear();
    m_dComp.Clear();
    m_qComp.Clear();
    m_sComp.Clear();
}
}  // namespace CubbyFlow