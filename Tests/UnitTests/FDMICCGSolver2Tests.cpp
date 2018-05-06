#include "pch.h"

#include <FDMLinearSystemSolverTestHelper2.h>

#include <Core/Solver/FDM/FDMICCGSolver2.h>

using namespace CubbyFlow;

TEST(FDMICCGSolver2, SolveLowRes)
{
    FDMLinearSystem2 system;
    FDMLinearSystemSolverTestHelper2::BuildTestLinearSystem(&system, { 3, 3 });

    FDMICCGSolver2 solver(10, 1e-9);
    solver.Solve(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}

TEST(FDMICCGSolver2, Solve)
{
    FDMLinearSystem2 system;
    FDMLinearSystemSolverTestHelper2::BuildTestLinearSystem(&system, { 128, 128 });

    FDMICCGSolver2 solver(200, 1e-4);
    EXPECT_TRUE(solver.Solve(&system));
}

TEST(FDMICCGSolver2, SolveCompressed)
{
    FDMLinearSystem2 system;
    FDMLinearSystemSolverTestHelper2::BuildTestLinearSystem(&system, { 3, 3 });

    FDMICCGSolver2 solver(200, 1e-4);
    EXPECT_TRUE(solver.Solve(&system));
}