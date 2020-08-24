#include "pch.hpp"

#include <Core/Grid/CellCenteredScalarGrid2.hpp>
#include <Core/Grid/CellCenteredScalarGrid3.hpp>
#include <Core/Solver/LevelSet/ENOLevelSetSolver2.hpp>
#include <Core/Solver/LevelSet/ENOLevelSetSolver3.hpp>
#include <Core/Solver/LevelSet/FMMLevelSetSolver2.hpp>
#include <Core/Solver/LevelSet/FMMLevelSetSolver3.hpp>
#include <Core/Solver/LevelSet/UpwindLevelSetSolver2.hpp>
#include <Core/Solver/LevelSet/UpwindLevelSetSolver3.hpp>

using namespace CubbyFlow;

TEST(UpwindLevelSetSolver2, Reinitialize)
{
    CellCenteredScalarGrid2 sdf(40, 30), temp(40, 30);

    sdf.Fill([](const Vector2D& x) {
        return (x - Vector2D(20, 20)).Length() - 8.0;
    });

    UpwindLevelSetSolver2 solver;
    solver.Reinitialize(sdf, 5.0, &temp);

    for (size_t j = 0; j < 30; ++j)
    {
        for (size_t i = 0; i < 40; ++i)
        {
            EXPECT_NEAR(sdf(i, j), temp(i, j), 0.5);
        }
    }
}

TEST(UpwindLevelSetSolver2, Extrapolate)
{
    CellCenteredScalarGrid2 sdf(40, 30), temp(40, 30);
    CellCenteredScalarGrid2 field(40, 30);

    sdf.Fill([](const Vector2D& x) {
        return (x - Vector2D(20, 20)).Length() - 8.0;
    });
    field.Fill(5.0);

    UpwindLevelSetSolver2 solver;
    solver.Extrapolate(field, sdf, 5.0, &temp);

    for (size_t j = 0; j < 30; ++j)
    {
        for (size_t i = 0; i < 40; ++i)
        {
            EXPECT_DOUBLE_EQ(5.0, temp(i, j));
        }
    }
}

TEST(UpwindLevelSetSolver3, Reinitialize)
{
    CellCenteredScalarGrid3 sdf(40, 30, 50), temp(40, 30, 50);

    sdf.Fill([](const Vector3D& x) {
        return (x - Vector3D(20, 20, 20)).Length() - 8.0;
    });

    UpwindLevelSetSolver3 solver;
    solver.Reinitialize(sdf, 5.0, &temp);

    for (size_t k = 0; k < 50; ++k)
    {
        for (size_t j = 0; j < 30; ++j)
        {
            for (size_t i = 0; i < 40; ++i)
            {
                EXPECT_NEAR(sdf(i, j, k), temp(i, j, k), 0.7)
                    << i << ", " << j << ", " << k;
            }
        }
    }
}

TEST(UpwindLevelSetSolver3, Extrapolate)
{
    CellCenteredScalarGrid3 sdf(40, 30, 50), temp(40, 30, 50);
    CellCenteredScalarGrid3 field(40, 30, 50);

    sdf.Fill([](const Vector3D& x) {
        return (x - Vector3D(20, 20, 20)).Length() - 8.0;
    });
    field.Fill(5.0);

    UpwindLevelSetSolver3 solver;
    solver.Extrapolate(field, sdf, 5.0, &temp);

    for (size_t k = 0; k < 50; ++k)
    {
        for (size_t j = 0; j < 30; ++j)
        {
            for (size_t i = 0; i < 40; ++i)
            {
                EXPECT_DOUBLE_EQ(5.0, temp(i, j, k))
                    << i << ", " << j << ", " << k;
            }
        }
    }
}

TEST(ENOLevelSetSolver2, Reinitialize)
{
    CellCenteredScalarGrid2 sdf(40, 30), temp(40, 30);

    sdf.Fill([](const Vector2D& x) {
        return (x - Vector2D(20, 20)).Length() - 8.0;
    });

    ENOLevelSetSolver2 solver;
    solver.Reinitialize(sdf, 5.0, &temp);

    for (size_t j = 0; j < 30; ++j)
    {
        for (size_t i = 0; i < 40; ++i)
        {
            EXPECT_NEAR(sdf(i, j), temp(i, j), 0.2);
        }
    }
}

TEST(ENOLevelSetSolver2, Extrapolate)
{
    CellCenteredScalarGrid2 sdf(40, 30), temp(40, 30);
    CellCenteredScalarGrid2 field(40, 30);

    sdf.Fill([](const Vector2D& x) {
        return (x - Vector2D(20, 20)).Length() - 8.0;
    });
    field.Fill(5.0);

    ENOLevelSetSolver2 solver;
    solver.Extrapolate(field, sdf, 5.0, &temp);

    for (size_t j = 0; j < 30; ++j)
    {
        for (size_t i = 0; i < 40; ++i)
        {
            EXPECT_DOUBLE_EQ(5.0, temp(i, j));
        }
    }
}

TEST(ENOLevelSetSolver3, Reinitialize)
{
    CellCenteredScalarGrid3 sdf(40, 30, 50), temp(40, 30, 50);

    sdf.Fill([](const Vector3D& x) {
        return (x - Vector3D(20, 20, 20)).Length() - 8.0;
    });

    ENOLevelSetSolver3 solver;
    solver.Reinitialize(sdf, 5.0, &temp);

    for (size_t k = 0; k < 50; ++k)
    {
        for (size_t j = 0; j < 30; ++j)
        {
            for (size_t i = 0; i < 40; ++i)
            {
                EXPECT_NEAR(sdf(i, j, k), temp(i, j, k), 0.5)
                    << i << ", " << j << ", " << k;
                ;
            }
        }
    }
}

TEST(ENOLevelSetSolver3, Extrapolate)
{
    CellCenteredScalarGrid3 sdf(40, 30, 50), temp(40, 30, 50);
    CellCenteredScalarGrid3 field(40, 30, 50);

    sdf.Fill([](const Vector3D& x) {
        return (x - Vector3D(20, 20, 20)).Length() - 8.0;
    });
    field.Fill(5.0);

    ENOLevelSetSolver3 solver;
    solver.Extrapolate(field, sdf, 5.0, &temp);

    for (size_t k = 0; k < 50; ++k)
    {
        for (size_t j = 0; j < 30; ++j)
        {
            for (size_t i = 0; i < 40; ++i)
            {
                EXPECT_DOUBLE_EQ(5.0, temp(i, j, k))
                    << i << ", " << j << ", " << k;
                ;
            }
        }
    }
}

TEST(FMMLevelSetSolver2, Reinitialize)
{
    CellCenteredScalarGrid2 sdf(40, 30), temp(40, 30);

    sdf.Fill([](const Vector2D& x) {
        return (x - Vector2D(20, 20)).Length() - 8.0;
    });

    FMMLevelSetSolver2 solver;
    solver.Reinitialize(sdf, 5.0, &temp);

    for (size_t j = 0; j < 30; ++j)
    {
        for (size_t i = 0; i < 40; ++i)
        {
            EXPECT_NEAR(sdf(i, j), temp(i, j), 0.6);
        }
    }
}

TEST(FMMLevelSetSolver2, Extrapolate)
{
    CellCenteredScalarGrid2 sdf(40, 30), temp(40, 30);
    CellCenteredScalarGrid2 field(40, 30);

    sdf.Fill([](const Vector2D& x) {
        return (x - Vector2D(20, 20)).Length() - 8.0;
    });
    field.Fill(5.0);

    FMMLevelSetSolver2 solver;
    solver.Extrapolate(field, sdf, 5.0, &temp);

    for (size_t j = 0; j < 30; ++j)
    {
        for (size_t i = 0; i < 40; ++i)
        {
            EXPECT_DOUBLE_EQ(5.0, temp(i, j));
        }
    }
}

TEST(FMMLevelSetSolver3, Reinitialize)
{
    CellCenteredScalarGrid3 sdf(40, 30, 50), temp(40, 30, 50);

    sdf.Fill([](const Vector3D& x) {
        return (x - Vector3D(20, 20, 20)).Length() - 8.0;
    });

    FMMLevelSetSolver3 solver;
    solver.Reinitialize(sdf, 5.0, &temp);

    for (size_t k = 0; k < 50; ++k)
    {
        for (size_t j = 0; j < 30; ++j)
        {
            for (size_t i = 0; i < 40; ++i)
            {
                EXPECT_NEAR(sdf(i, j, k), temp(i, j, k), 0.9)
                    << i << ", " << j << ", " << k;
                ;
            }
        }
    }
}

TEST(FMMLevelSetSolver3, Extrapolate)
{
    CellCenteredScalarGrid3 sdf(40, 30, 50), temp(40, 30, 50);
    CellCenteredScalarGrid3 field(40, 30, 50);

    sdf.Fill([](const Vector3D& x) {
        return (x - Vector3D(20, 20, 20)).Length() - 8.0;
    });
    field.Fill(5.0);

    FMMLevelSetSolver3 solver;
    solver.Extrapolate(field, sdf, 5.0, &temp);

    for (size_t k = 0; k < 50; ++k)
    {
        for (size_t j = 0; j < 30; ++j)
        {
            for (size_t i = 0; i < 40; ++i)
            {
                EXPECT_DOUBLE_EQ(5.0, temp(i, j, k))
                    << i << ", " << j << ", " << k;
                ;
            }
        }
    }
}