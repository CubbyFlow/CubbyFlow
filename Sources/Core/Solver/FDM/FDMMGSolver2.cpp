// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/FDM/FDMGaussSeidelSolver2.hpp>
#include <Core/Solver/FDM/FDMMGSolver2.hpp>
#include <Core/Utils/MG.hpp>

namespace CubbyFlow
{
FDMMGSolver2::FDMMGSolver2(size_t maxNumberOfLevels,
                           unsigned int numberOfRestrictionIter,
                           unsigned int numberOfCorrectionIter,
                           unsigned int numberOfCoarsestIter,
                           unsigned int numberOfFinalIter, double maxTolerance,
                           double sorFactor, bool useRedBlackOrdering)
{
    m_mgParams.maxNumberOfLevels = maxNumberOfLevels;
    m_mgParams.numberOfRestrictionIter = numberOfRestrictionIter;
    m_mgParams.numberOfCorrectionIter = numberOfCorrectionIter;
    m_mgParams.numberOfCoarsestIter = numberOfCoarsestIter;
    m_mgParams.numberOfFinalIter = numberOfFinalIter;
    m_mgParams.maxTolerance = maxTolerance;

    if (useRedBlackOrdering)
    {
        m_mgParams.relaxFunc =
            [sorFactor](const FDMMatrix2& A, const FDMVector2& b,
                        unsigned int numberOfIterations, double _maxTolerance,
                        FDMVector2* x, FDMVector2* buffer) {
                UNUSED_VARIABLE(_maxTolerance);
                UNUSED_VARIABLE(buffer);

                for (unsigned int iter = 0; iter < numberOfIterations; ++iter)
                {
                    FDMGaussSeidelSolver2::RelaxRedBlack(A, b, sorFactor, x);
                }
            };
    }
    else
    {
        m_mgParams.relaxFunc =
            [sorFactor](const FDMMatrix2& A, const FDMVector2& b,
                        unsigned int numberOfIterations, double _maxTolerance,
                        FDMVector2* x, FDMVector2* buffer) {
                UNUSED_VARIABLE(_maxTolerance);
                UNUSED_VARIABLE(buffer);

                for (unsigned int iter = 0; iter < numberOfIterations; ++iter)
                {
                    FDMGaussSeidelSolver2::Relax(A, b, sorFactor, x);
                }
            };
    }

    m_mgParams.restrictFunc = FDMMGUtils2::Restrict;
    m_mgParams.correctFunc = FDMMGUtils2::Correct;
    m_sorFactor = sorFactor;
    m_useRedBlackOrdering = useRedBlackOrdering;
}

const MGParameters<FDMBLAS2>& FDMMGSolver2::GetParams() const
{
    return m_mgParams;
}

double FDMMGSolver2::GetSORFactor() const
{
    return m_sorFactor;
}

bool FDMMGSolver2::GetUseRedBlackOrdering() const
{
    return m_useRedBlackOrdering;
}

bool FDMMGSolver2::Solve(FDMLinearSystem2* system)
{
    UNUSED_VARIABLE(system);

    return false;
}

bool FDMMGSolver2::Solve(FDMMGLinearSystem2* system)
{
    FDMMGVector2 buffer = system->x;
    const MGResult result =
        MGVCycle(system->A, m_mgParams, &system->x, &system->b, &buffer);
    return result.lastResidualNorm < m_mgParams.maxTolerance;
}
}  // namespace CubbyFlow
