#include "pch.h"

#include <FDMLinearSystemSolverTestHelper3.h>

#include <Core/Solver/FDM/FDMJacobiSolver3.hpp>

using namespace CubbyFlow;

TEST(FDMJacobiSolver3, Solve)
{
    FDMLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestLinearSystem(&system, { 3, 3, 3 });

    FDMJacobiSolver3 solver(100, 10, 1e-9);
    solver.Solve(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}

TEST(FDMJacobiSolver3, SolveCompressed)
{
    FDMCompressedLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestCompressedLinearSystem(&system, { 3, 3, 3 });

    FDMJacobiSolver3 solver(100, 10, 1e-9);
    solver.SolveCompressed(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}