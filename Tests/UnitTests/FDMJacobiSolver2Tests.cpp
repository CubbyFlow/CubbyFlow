#include "pch.hpp"

#include <FDMLinearSystemSolverTestHelper2.hpp>

#include <Core/Solver/FDM/FDMJacobiSolver2.hpp>

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
    FDMLinearSystemSolverTestHelper2::BuildTestCompressedLinearSystem(&system,
                                                                      { 3, 3 });

    FDMJacobiSolver2 solver(100, 10, 1e-9);
    solver.SolveCompressed(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}