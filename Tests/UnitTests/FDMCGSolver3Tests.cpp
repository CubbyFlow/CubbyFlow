#include "pch.hpp"

#include <FDMLinearSystemSolverTestHelper3.hpp>

#include <Core/Solver/FDM/FDMCGSolver3.hpp>

using namespace CubbyFlow;

TEST(FDMCGSolver3, Solve)
{
    FDMLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestLinearSystem(&system,
                                                            { 3, 3, 3 });

    FDMCGSolver3 solver(100, 1e-9);
    solver.Solve(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}

TEST(FDMCGSolver3, SolveCompressed)
{
    FDMCompressedLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestCompressedLinearSystem(
        &system, { 3, 3, 3 });

    FDMCGSolver3 solver(100, 1e-9);
    solver.SolveCompressed(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}