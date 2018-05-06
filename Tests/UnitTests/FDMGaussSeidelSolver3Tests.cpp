#include "pch.h"

#include <FDMLinearSystemSolverTestHelper3.h>

#include <Core/Solver/FDM/FDMGaussSeidelSolver3.h>

using namespace CubbyFlow;

TEST(FDMGaussSeidelSolver3, SolveLowRes)
{
    FDMLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestLinearSystem(&system, { 3, 3, 3 });

    FDMGaussSeidelSolver3 solver(100, 10, 1e-9);
    solver.Solve(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}

TEST(FDMGaussSeidelSolver3, Solve)
{
    FDMLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestLinearSystem(&system, { 32, 32, 32 });

    auto buffer = system.x;
    FDMBLAS3::Residual(system.A, system.x, system.b, &buffer);
    double norm0 = FDMBLAS3::L2Norm(buffer);

    FDMGaussSeidelSolver3 solver(100, 10, 1e-9);
    solver.Solve(&system);

    FDMBLAS3::Residual(system.A, system.x, system.b, &buffer);
    double norm1 = FDMBLAS3::L2Norm(buffer);

    EXPECT_LT(norm1, norm0);
}

TEST(FDMGaussSeidelSolver3, Relax)
{
    FDMLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestLinearSystem(&system, { 32, 32, 32 });

    auto buffer = system.x;
    FDMBLAS3::Residual(system.A, system.x, system.b, &buffer);
    double norm0 = FDMBLAS3::L2Norm(buffer);

    for (int i = 0; i < 200; ++i)
    {
        FDMGaussSeidelSolver3::Relax(system.A, system.b, 1.0, &system.x);

        FDMBLAS3::Residual(system.A, system.x, system.b, &buffer);
        double norm = FDMBLAS3::L2Norm(buffer);
        EXPECT_LT(norm, norm0);

        norm0 = norm;
    }
}

TEST(FDMGaussSeidelSolver3, RelaxRedBlack)
{
    FDMLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestLinearSystem(&system, { 32, 32, 32 });

    auto buffer = system.x;
    FDMBLAS3::Residual(system.A, system.x, system.b, &buffer);
    double norm0 = FDMBLAS3::L2Norm(buffer);

    for (int i = 0; i < 200; ++i)
    {
        FDMGaussSeidelSolver3::RelaxRedBlack(system.A, system.b, 1.0, &system.x);

        FDMBLAS3::Residual(system.A, system.x, system.b, &buffer);
        double norm = FDMBLAS3::L2Norm(buffer);

        if (i > 0)
        {
            EXPECT_LT(norm, norm0);
        }

        norm0 = norm;
    }
}

TEST(FDMGaussSeidelSolver3, SolveCompressedLowRes)
{
    FDMCompressedLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestCompressedLinearSystem(&system, { 3, 3, 3 });

    FDMGaussSeidelSolver3 solver(100, 10, 1e-9);
    solver.SolveCompressed(&system);

    EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}

TEST(FDMGaussSeidelSolver3, SolveCompressed)
{
    FDMCompressedLinearSystem3 system;
    FDMLinearSystemSolverTestHelper3::BuildTestCompressedLinearSystem(&system, { 32, 32, 32 });

    auto buffer = system.x;
    FDMCompressedBLAS3::Residual(system.A, system.x, system.b, &buffer);
    double norm0 = FDMCompressedBLAS3::L2Norm(buffer);

    FDMGaussSeidelSolver3 solver(100, 10, 1e-9);
    solver.SolveCompressed(&system);

    FDMCompressedBLAS3::Residual(system.A, system.x, system.b, &buffer);
    double norm1 = FDMCompressedBLAS3::L2Norm(buffer);

    EXPECT_LT(norm1, norm0);
}