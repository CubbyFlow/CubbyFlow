#include "pch.hpp"

#include <FDMLinearSystemSolverTestHelper2.hpp>

#include <Core/Solver/FDM/FDMCGSolver2.hpp>

using namespace CubbyFlow;

TEST(FDMCGSolver2, Solve)
{
    FDMLinearSystem2 system;
    FDMLinearSystemSolverTestHelper2::BuildTestLinearSystem(&system, { 3, 3 });

    FDMCGSolver2 solver(10, 1e-9);
    solver.Solve(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}

TEST(FDMCGSolver2, SolveCompressed)
{
    FDMCompressedLinearSystem2 system;
    FDMLinearSystemSolverTestHelper2::BuildTestCompressedLinearSystem(&system,
                                                                      { 3, 3 });

    FDMCGSolver2 solver(10, 1e-9);
    solver.SolveCompressed(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}