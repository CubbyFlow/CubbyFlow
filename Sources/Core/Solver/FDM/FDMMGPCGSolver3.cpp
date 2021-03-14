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
#include <Core/Solver/FDM/FDMMGPCGSolver3.hpp>
#include <Core/Utils/Logging.hpp>

#include <utility>

namespace CubbyFlow
{
void FDMMGPCGSolver3::Preconditioner::Build(FDMMGLinearSystem3* _system,
                                            MGParameters<FDMBLAS3> _mgParams)
{
    system = _system;
    mgParams = std::move(_mgParams);
}

void FDMMGPCGSolver3::Preconditioner::Solve(const FDMVector3& b,
                                            FDMVector3* x) const
{
    // Copy dimension
    FDMMGVector3 mgX = system->x;
    FDMMGVector3 mgB = system->x;
    FDMMGVector3 mgBuffer = system->x;

    // Copy input to the top
    mgX.levels.front().CopyFrom(*x);
    mgB.levels.front().CopyFrom(b);

    MGVCycle(system->A, mgParams, &mgX, &mgB, &mgBuffer);

    // Copy result to the output
    x->CopyFrom(mgX.levels.front());
}

FDMMGPCGSolver3::FDMMGPCGSolver3(
    unsigned int numberOfCGIter, size_t maxNumberOfLevels,
    unsigned int numberOfRestrictionIter, unsigned int numberOfCorrectionIter,
    unsigned int numberOfCoarsestIter, unsigned int numberOfFinalIter,
    double maxTolerance, double sorFactor, bool useRedBlackOrdering)
    : FDMMGSolver3{ maxNumberOfLevels,
                    numberOfRestrictionIter,
                    numberOfCorrectionIter,
                    numberOfCoarsestIter,
                    numberOfFinalIter,
                    maxTolerance,
                    sorFactor,
                    useRedBlackOrdering },
      m_maxNumberOfIterations{ numberOfCGIter },
      m_lastNumberOfIterations{ 0 },
      m_tolerance{ maxTolerance },
      m_lastResidualNorm{ std::numeric_limits<double>::max() }
{
    // Do nothing
}

bool FDMMGPCGSolver3::Solve(FDMMGLinearSystem3* system)
{
    const Vector3UZ size = system->A.levels.front().Size();
    m_r.Resize(size);
    m_d.Resize(size);
    m_q.Resize(size);
    m_s.Resize(size);

    system->x.levels.front().Fill(0.0);
    m_r.Fill(0.0);
    m_d.Fill(0.0);
    m_q.Fill(0.0);
    m_s.Fill(0.0);

    m_precond.Build(system, GetParams());

    PCG<FDMBLAS3, Preconditioner>(
        system->A.levels.front(), system->b.levels.front(),
        m_maxNumberOfIterations, m_tolerance, &m_precond,
        &system->x.levels.front(), &m_r, &m_d, &m_q, &m_s,
        &m_lastNumberOfIterations, &m_lastResidualNorm);

    CUBBYFLOW_INFO << "Residual after solving MGPCG: " << m_lastResidualNorm
                   << " Number of MGPCG iterations: "
                   << m_lastNumberOfIterations;

    return m_lastResidualNorm <= m_tolerance ||
           m_lastNumberOfIterations < m_maxNumberOfIterations;
}

unsigned int FDMMGPCGSolver3::GetMaxNumberOfIterations() const
{
    return m_maxNumberOfIterations;
}

unsigned int FDMMGPCGSolver3::GetLastNumberOfIterations() const
{
    return m_lastNumberOfIterations;
}

double FDMMGPCGSolver3::GetTolerance() const
{
    return m_tolerance;
}

double FDMMGPCGSolver3::GetLastResidual() const
{
    return m_lastResidualNorm;
}
}  // namespace CubbyFlow