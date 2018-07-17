#include "pch.h"

#include <FDMLinearSystemSolverTestHelper2.h>

#include <Solver/FDM/FDMJacobiSolver2.h>

using namespace CubbyFlow;

TEST(FDMJacobiSolver2, Solve)
{
    FDMLinearSystem2 system;
    FDMLinearSystemSolverTestHelper2::BuildTestLinearSystem(&system, { 3, 3 });

    FDMJacobiSolver2 solver(100, 10, 1e-9);
    solver.Solve(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}

TEST(FDMJacobiSolver2, SolveCompressed)
{
    FDMCompressedLinearSystem2 system;
    FDMLinearSystemSolverTestHelper2::BuildTestCompressedLinearSystem(&system, { 3, 3 });

    FDMJacobiSolver2 solver(100, 10, 1e-9);
    solver.SolveCompressed(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}