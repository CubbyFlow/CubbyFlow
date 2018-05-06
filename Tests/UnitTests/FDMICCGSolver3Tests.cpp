#include "pch.h"

#include <FDMLinearSystemSolverTestHelper3.h>

#include <Core/Solver/FDM/FDMICCGSolver3.h>

using namespace CubbyFlow;

TEST(FDMICCGSolver3, SolveLowRes)
{
    FDMLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestLinearSystem(&system, { 3, 3, 3 });

    FDMICCGSolver3 solver(100, 1e-9);
    solver.Solve(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}

TEST(FDMICCGSolver3, Solve)
{
    FDMLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestLinearSystem(&system, { 32, 32, 32 });

    FDMICCGSolver3 solver(100, 1e-4);
    solver.Solve(&system);

    EXPECT_TRUE(solver.Solve(&system));
}

TEST(FDMICCGSolver3, SolveCompressed)
{
    FDMCompressedLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestCompressedLinearSystem(&system, { 3, 3, 3 });

    FDMICCGSolver3 solver(100, 1e-4);
    solver.SolveCompressed(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}