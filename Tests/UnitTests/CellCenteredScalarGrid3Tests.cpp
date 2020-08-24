#include "pch.hpp"

#include <Core/Grid/CellCenteredScalarGrid3.hpp>

using namespace CubbyFlow;

TEST(CellCenteredScalarGrid3, Constructors)
{
    // Default constructors
    CellCenteredScalarGrid3 grid1;
    EXPECT_EQ(0u, grid1.Resolution().x);
    EXPECT_EQ(0u, grid1.Resolution().y);
    EXPECT_EQ(0u, grid1.Resolution().z);
    EXPECT_DOUBLE_EQ(1.0, grid1.GridSpacing().x);
    EXPECT_DOUBLE_EQ(1.0, grid1.GridSpacing().y);
    EXPECT_DOUBLE_EQ(1.0, grid1.GridSpacing().z);
    EXPECT_DOUBLE_EQ(0.0, grid1.Origin().x);
    EXPECT_DOUBLE_EQ(0.0, grid1.Origin().y);
    EXPECT_DOUBLE_EQ(0.0, grid1.Origin().z);
    EXPECT_EQ(0u, grid1.GetDataSize().x);
    EXPECT_EQ(0u, grid1.GetDataSize().y);
    EXPECT_EQ(0u, grid1.GetDataSize().z);
    EXPECT_DOUBLE_EQ(0.5, grid1.GetDataOrigin().x);
    EXPECT_DOUBLE_EQ(0.5, grid1.GetDataOrigin().y);
    EXPECT_DOUBLE_EQ(0.5, grid1.GetDataOrigin().z);

    // Constructor with params
    CellCenteredScalarGrid3 grid2(5, 4, 3, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);
    EXPECT_EQ(5u, grid2.Resolution().x);
    EXPECT_EQ(4u, grid2.Resolution().y);
    EXPECT_EQ(3u, grid2.Resolution().z);
    EXPECT_DOUBLE_EQ(1.0, grid2.GridSpacing().x);
    EXPECT_DOUBLE_EQ(2.0, grid2.GridSpacing().y);
    EXPECT_DOUBLE_EQ(3.0, grid2.GridSpacing().z);
    EXPECT_DOUBLE_EQ(4.0, grid2.Origin().x);
    EXPECT_DOUBLE_EQ(5.0, grid2.Origin().y);
    EXPECT_DOUBLE_EQ(6.0, grid2.Origin().z);
    EXPECT_EQ(5u, grid2.GetDataSize().x);
    EXPECT_EQ(4u, grid2.GetDataSize().y);
    EXPECT_EQ(3u, grid2.GetDataSize().z);
    EXPECT_DOUBLE_EQ(4.5, grid2.GetDataOrigin().x);
    EXPECT_DOUBLE_EQ(6.0, grid2.GetDataOrigin().y);
    EXPECT_DOUBLE_EQ(7.5, grid2.GetDataOrigin().z);
    grid2.ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(7.0, grid2(i, j, k));
    });

    // Copy constructor
    CellCenteredScalarGrid3 grid3(grid2);
    EXPECT_EQ(5u, grid3.Resolution().x);
    EXPECT_EQ(4u, grid3.Resolution().y);
    EXPECT_EQ(3u, grid3.Resolution().z);
    EXPECT_DOUBLE_EQ(1.0, grid3.GridSpacing().x);
    EXPECT_DOUBLE_EQ(2.0, grid3.GridSpacing().y);
    EXPECT_DOUBLE_EQ(3.0, grid3.GridSpacing().z);
    EXPECT_DOUBLE_EQ(4.0, grid3.Origin().x);
    EXPECT_DOUBLE_EQ(5.0, grid3.Origin().y);
    EXPECT_DOUBLE_EQ(6.0, grid3.Origin().z);
    EXPECT_EQ(5u, grid3.GetDataSize().x);
    EXPECT_EQ(4u, grid3.GetDataSize().y);
    EXPECT_EQ(3u, grid3.GetDataSize().z);
    EXPECT_DOUBLE_EQ(4.5, grid3.GetDataOrigin().x);
    EXPECT_DOUBLE_EQ(6.0, grid3.GetDataOrigin().y);
    EXPECT_DOUBLE_EQ(7.5, grid3.GetDataOrigin().z);
    grid3.ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(7.0, grid3(i, j, k));
    });
}

TEST(CellCenteredScalarGrid3, Swap)
{
    CellCenteredScalarGrid3 grid1(5, 4, 3, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);
    CellCenteredScalarGrid3 grid2(3, 8, 5, 2.0, 3.0, 1.0, 5.0, 4.0, 7.0, 8.0);
    grid1.Swap(&grid2);

    EXPECT_EQ(3u, grid1.Resolution().x);
    EXPECT_EQ(8u, grid1.Resolution().y);
    EXPECT_EQ(5u, grid1.Resolution().z);
    EXPECT_DOUBLE_EQ(2.0, grid1.GridSpacing().x);
    EXPECT_DOUBLE_EQ(3.0, grid1.GridSpacing().y);
    EXPECT_DOUBLE_EQ(1.0, grid1.GridSpacing().z);
    EXPECT_DOUBLE_EQ(5.0, grid1.Origin().x);
    EXPECT_DOUBLE_EQ(4.0, grid1.Origin().y);
    EXPECT_DOUBLE_EQ(7.0, grid1.Origin().z);
    EXPECT_EQ(3u, grid1.GetDataSize().x);
    EXPECT_EQ(8u, grid1.GetDataSize().y);
    EXPECT_EQ(5u, grid1.GetDataSize().z);
    EXPECT_DOUBLE_EQ(6.0, grid1.GetDataOrigin().x);
    EXPECT_DOUBLE_EQ(5.5, grid1.GetDataOrigin().y);
    EXPECT_DOUBLE_EQ(7.5, grid1.GetDataOrigin().z);
    grid1.ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(8.0, grid1(i, j, k));
    });

    EXPECT_EQ(5u, grid2.Resolution().x);
    EXPECT_EQ(4u, grid2.Resolution().y);
    EXPECT_EQ(3u, grid2.Resolution().z);
    EXPECT_DOUBLE_EQ(1.0, grid2.GridSpacing().x);
    EXPECT_DOUBLE_EQ(2.0, grid2.GridSpacing().y);
    EXPECT_DOUBLE_EQ(3.0, grid2.GridSpacing().z);
    EXPECT_DOUBLE_EQ(4.0, grid2.Origin().x);
    EXPECT_DOUBLE_EQ(5.0, grid2.Origin().y);
    EXPECT_DOUBLE_EQ(6.0, grid2.Origin().z);
    EXPECT_EQ(5u, grid2.GetDataSize().x);
    EXPECT_EQ(4u, grid2.GetDataSize().y);
    EXPECT_EQ(3u, grid2.GetDataSize().z);
    EXPECT_DOUBLE_EQ(4.5, grid2.GetDataOrigin().x);
    EXPECT_DOUBLE_EQ(6.0, grid2.GetDataOrigin().y);
    EXPECT_DOUBLE_EQ(7.5, grid2.GetDataOrigin().z);
    grid2.ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(7.0, grid2(i, j, k));
    });
}

TEST(CellCenteredScalarGrid3, Set)
{
    CellCenteredScalarGrid3 grid1(5, 4, 3, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);
    CellCenteredScalarGrid3 grid2(3, 8, 5, 2.0, 3.0, 1.0, 5.0, 4.0, 7.0, 8.0);
    grid1.Set(grid2);

    EXPECT_EQ(3u, grid1.Resolution().x);
    EXPECT_EQ(8u, grid1.Resolution().y);
    EXPECT_EQ(5u, grid1.Resolution().z);
    EXPECT_DOUBLE_EQ(2.0, grid1.GridSpacing().x);
    EXPECT_DOUBLE_EQ(3.0, grid1.GridSpacing().y);
    EXPECT_DOUBLE_EQ(1.0, grid1.GridSpacing().z);
    EXPECT_DOUBLE_EQ(5.0, grid1.Origin().x);
    EXPECT_DOUBLE_EQ(4.0, grid1.Origin().y);
    EXPECT_DOUBLE_EQ(7.0, grid1.Origin().z);
    EXPECT_EQ(3u, grid1.GetDataSize().x);
    EXPECT_EQ(8u, grid1.GetDataSize().y);
    EXPECT_EQ(5u, grid1.GetDataSize().z);
    EXPECT_DOUBLE_EQ(6.0, grid1.GetDataOrigin().x);
    EXPECT_DOUBLE_EQ(5.5, grid1.GetDataOrigin().y);
    EXPECT_DOUBLE_EQ(7.5, grid1.GetDataOrigin().z);
    grid1.ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(8.0, grid1(i, j, k));
    });
}

TEST(CellCenteredScalarGrid3, AssignmentOperator)
{
    CellCenteredScalarGrid3 grid1;
    CellCenteredScalarGrid3 grid2(3, 8, 5, 2.0, 3.0, 1.0, 5.0, 4.0, 7.0, 8.0);
    grid1 = grid2;

    EXPECT_EQ(3u, grid1.Resolution().x);
    EXPECT_EQ(8u, grid1.Resolution().y);
    EXPECT_EQ(5u, grid1.Resolution().z);
    EXPECT_DOUBLE_EQ(2.0, grid1.GridSpacing().x);
    EXPECT_DOUBLE_EQ(3.0, grid1.GridSpacing().y);
    EXPECT_DOUBLE_EQ(1.0, grid1.GridSpacing().z);
    EXPECT_DOUBLE_EQ(5.0, grid1.Origin().x);
    EXPECT_DOUBLE_EQ(4.0, grid1.Origin().y);
    EXPECT_DOUBLE_EQ(7.0, grid1.Origin().z);
    EXPECT_EQ(3u, grid1.GetDataSize().x);
    EXPECT_EQ(8u, grid1.GetDataSize().y);
    EXPECT_EQ(5u, grid1.GetDataSize().z);
    EXPECT_DOUBLE_EQ(6.0, grid1.GetDataOrigin().x);
    EXPECT_DOUBLE_EQ(5.5, grid1.GetDataOrigin().y);
    EXPECT_DOUBLE_EQ(7.5, grid1.GetDataOrigin().z);
    grid1.ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(8.0, grid1(i, j, k));
    });
}

TEST(CellCenteredScalarGrid3, Clone)
{
    CellCenteredScalarGrid3 grid2(3, 8, 5, 2.0, 3.0, 1.0, 5.0, 4.0, 7.0, 8.0);
    auto grid1 = grid2.Clone();

    EXPECT_EQ(3u, grid1->Resolution().x);
    EXPECT_EQ(8u, grid1->Resolution().y);
    EXPECT_EQ(5u, grid1->Resolution().z);
    EXPECT_DOUBLE_EQ(2.0, grid1->GridSpacing().x);
    EXPECT_DOUBLE_EQ(3.0, grid1->GridSpacing().y);
    EXPECT_DOUBLE_EQ(1.0, grid1->GridSpacing().z);
    EXPECT_DOUBLE_EQ(5.0, grid1->Origin().x);
    EXPECT_DOUBLE_EQ(4.0, grid1->Origin().y);
    EXPECT_DOUBLE_EQ(7.0, grid1->Origin().z);
    EXPECT_EQ(3u, grid1->GetDataSize().x);
    EXPECT_EQ(8u, grid1->GetDataSize().y);
    EXPECT_EQ(5u, grid1->GetDataSize().z);
    EXPECT_DOUBLE_EQ(6.0, grid1->GetDataOrigin().x);
    EXPECT_DOUBLE_EQ(5.5, grid1->GetDataOrigin().y);
    EXPECT_DOUBLE_EQ(7.5, grid1->GetDataOrigin().z);
    grid1->ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(8.0, (*grid1)(i, j, k));
    });
}

TEST(CellCenteredScalarGrid3, Builder)
{
    {
        auto grid1 = CellCenteredScalarGrid3::GetBuilder().Build(
            { 3, 8, 5 }, { 2.0, 3.0, 1.0 }, { 5.0, 4.0, 7.0 }, 8.0);

        auto grid2 = std::dynamic_pointer_cast<CellCenteredScalarGrid3>(grid1);
        EXPECT_TRUE(grid2 != nullptr);

        EXPECT_EQ(3u, grid1->Resolution().x);
        EXPECT_EQ(8u, grid1->Resolution().y);
        EXPECT_EQ(5u, grid1->Resolution().z);
        EXPECT_DOUBLE_EQ(2.0, grid1->GridSpacing().x);
        EXPECT_DOUBLE_EQ(3.0, grid1->GridSpacing().y);
        EXPECT_DOUBLE_EQ(1.0, grid1->GridSpacing().z);
        EXPECT_DOUBLE_EQ(5.0, grid1->Origin().x);
        EXPECT_DOUBLE_EQ(4.0, grid1->Origin().y);
        EXPECT_DOUBLE_EQ(7.0, grid1->Origin().z);
        EXPECT_EQ(3u, grid1->GetDataSize().x);
        EXPECT_EQ(8u, grid1->GetDataSize().y);
        EXPECT_EQ(5u, grid1->GetDataSize().z);
        EXPECT_DOUBLE_EQ(6.0, grid1->GetDataOrigin().x);
        EXPECT_DOUBLE_EQ(5.5, grid1->GetDataOrigin().y);
        EXPECT_DOUBLE_EQ(7.5, grid1->GetDataOrigin().z);
        grid1->ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
            EXPECT_DOUBLE_EQ(8.0, (*grid1)(i, j, k));
        });
    }

    {
        auto grid1 = CellCenteredScalarGrid3::GetBuilder()
                         .WithResolution(3, 8, 5)
                         .WithGridSpacing(2, 3, 1)
                         .WithOrigin(5, 4, 7)
                         .WithInitialValue(8.0)
                         .Build();

        EXPECT_EQ(3u, grid1.Resolution().x);
        EXPECT_EQ(8u, grid1.Resolution().y);
        EXPECT_EQ(5u, grid1.Resolution().z);
        EXPECT_DOUBLE_EQ(2.0, grid1.GridSpacing().x);
        EXPECT_DOUBLE_EQ(3.0, grid1.GridSpacing().y);
        EXPECT_DOUBLE_EQ(1.0, grid1.GridSpacing().z);
        EXPECT_DOUBLE_EQ(5.0, grid1.Origin().x);
        EXPECT_DOUBLE_EQ(4.0, grid1.Origin().y);
        EXPECT_DOUBLE_EQ(7.0, grid1.Origin().z);
        EXPECT_EQ(3u, grid1.GetDataSize().x);
        EXPECT_EQ(8u, grid1.GetDataSize().y);
        EXPECT_EQ(5u, grid1.GetDataSize().z);
        EXPECT_DOUBLE_EQ(6.0, grid1.GetDataOrigin().x);
        EXPECT_DOUBLE_EQ(5.5, grid1.GetDataOrigin().y);
        EXPECT_DOUBLE_EQ(7.5, grid1.GetDataOrigin().z);
        grid1.ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
            EXPECT_DOUBLE_EQ(8.0, grid1(i, j, k));
        });
    }
}

TEST(CellCenteredScalarGrid3, Fill)
{
    CellCenteredScalarGrid3 grid(5, 4, 6, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0);
    grid.Fill(42.0);

    for (size_t k = 0; k < grid.GetDataSize().z; ++k)
    {
        for (size_t j = 0; j < grid.GetDataSize().y; ++j)
        {
            for (size_t i = 0; i < grid.GetDataSize().x; ++i)
            {
                EXPECT_DOUBLE_EQ(42.0, grid(i, j, k));
            }
        }
    }

    auto func = [](const Vector3D& x) { return x.Sum(); };
    grid.Fill(func);

    for (size_t k = 0; k < grid.GetDataSize().z; ++k)
    {
        for (size_t j = 0; j < grid.GetDataSize().y; ++j)
        {
            for (size_t i = 0; i < grid.GetDataSize().x; ++i)
            {
                EXPECT_DOUBLE_EQ(static_cast<double>(i + j + k) + 1.5,
                                 grid(i, j, k));
            }
        }
    }
}

TEST(CellCenteredScalarGrid3, GradientAtDataPoint)
{
    CellCenteredScalarGrid3 grid(5, 8, 6, 2.0, 3.0, 1.5);

    grid.Fill(1.0);

    for (size_t k = 0; k < grid.Resolution().z; ++k)
    {
        for (size_t j = 0; j < grid.Resolution().y; ++j)
        {
            for (size_t i = 0; i < grid.Resolution().x; ++i)
            {
                Vector3D grad = grid.GradientAtDataPoint(i, j, k);
                EXPECT_DOUBLE_EQ(0.0, grad.x);
                EXPECT_DOUBLE_EQ(0.0, grad.y);
                EXPECT_DOUBLE_EQ(0.0, grad.z);
            }
        }
    }

    grid.Fill([](const Vector3D& x) { return x.x + 2.0 * x.y - 3.0 * x.z; });

    for (size_t k = 1; k < grid.Resolution().z - 1; ++k)
    {
        for (size_t j = 1; j < grid.Resolution().y - 1; ++j)
        {
            for (size_t i = 1; i < grid.Resolution().x - 1; ++i)
            {
                Vector3D grad = grid.GradientAtDataPoint(i, j, k);
                EXPECT_DOUBLE_EQ(1.0, grad.x);
                EXPECT_DOUBLE_EQ(2.0, grad.y);
                EXPECT_DOUBLE_EQ(-3.0, grad.z);
            }
        }
    }
}

TEST(CellCenteredScalarGrid3, LaplacianAtAtDataPoint)
{
    CellCenteredScalarGrid3 grid(5, 8, 6, 2.0, 3.0, 1.5);

    grid.Fill(1.0);

    for (size_t k = 0; k < grid.Resolution().z; ++k)
    {
        for (size_t j = 0; j < grid.Resolution().y; ++j)
        {
            for (size_t i = 0; i < grid.Resolution().x; ++i)
            {
                EXPECT_DOUBLE_EQ(0.0, grid.LaplacianAtDataPoint(i, j, k));
            }
        }
    }

    grid.Fill([](const Vector3D& x) {
        return Square(x.x) + 2.0 * Square(x.y) - 4.0 * Square(x.z);
    });

    for (size_t k = 1; k < grid.Resolution().z - 1; ++k)
    {
        for (size_t j = 1; j < grid.Resolution().y - 1; ++j)
        {
            for (size_t i = 1; i < grid.Resolution().x - 1; ++i)
            {
                EXPECT_DOUBLE_EQ(-2.0, grid.LaplacianAtDataPoint(i, j, k));
            }
        }
    }
}

TEST(CellCenteredScalarGrid3, Serialization)
{
    CellCenteredScalarGrid3 grid1(5, 4, 3, 1.0, 2.0, 3.0, -5.0, 3.0, 1.0);
    grid1.Fill([&](const Vector3D& pt) { return pt.x + pt.y + pt.z; });

    // Serialize to in-memory stream
    std::vector<uint8_t> buffer1;
    grid1.Serialize(&buffer1);

    // Deserialize to non-zero array
    CellCenteredScalarGrid3 grid2(1, 2, 4, 0.5, 1.0, 2.0, 0.5, 2.0, -3.0);
    grid2.Deserialize(buffer1);
    EXPECT_EQ(5u, grid2.Resolution().x);
    EXPECT_EQ(4u, grid2.Resolution().y);
    EXPECT_EQ(3u, grid2.Resolution().z);
    EXPECT_DOUBLE_EQ(1.0, grid2.GridSpacing().x);
    EXPECT_DOUBLE_EQ(2.0, grid2.GridSpacing().y);
    EXPECT_DOUBLE_EQ(3.0, grid2.GridSpacing().z);
    EXPECT_DOUBLE_EQ(-5.0, grid2.Origin().x);
    EXPECT_DOUBLE_EQ(3.0, grid2.Origin().y);
    EXPECT_DOUBLE_EQ(1.0, grid2.Origin().z);
    EXPECT_DOUBLE_EQ(-5.0, grid2.BoundingBox().lowerCorner.x);
    EXPECT_DOUBLE_EQ(3.0, grid2.BoundingBox().lowerCorner.y);
    EXPECT_DOUBLE_EQ(1.0, grid2.BoundingBox().lowerCorner.z);
    EXPECT_DOUBLE_EQ(0.0, grid2.BoundingBox().upperCorner.x);
    EXPECT_DOUBLE_EQ(11.0, grid2.BoundingBox().upperCorner.y);
    EXPECT_DOUBLE_EQ(10.0, grid2.BoundingBox().upperCorner.z);

    grid1.ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(grid1(i, j, k), grid2(i, j, k));
    });

    // Serialize zero-sized array
    CellCenteredScalarGrid3 grid3;
    std::vector<uint8_t> buffer2;
    grid3.Serialize(&buffer2);

    // Deserialize to non-zero array
    grid2.Deserialize(buffer2);
    EXPECT_EQ(0u, grid2.Resolution().x);
    EXPECT_EQ(0u, grid2.Resolution().y);
    EXPECT_EQ(0u, grid2.Resolution().z);
    EXPECT_DOUBLE_EQ(0.0, grid2.Origin().x);
    EXPECT_DOUBLE_EQ(0.0, grid2.Origin().y);
    EXPECT_DOUBLE_EQ(0.0, grid2.Origin().z);
    EXPECT_DOUBLE_EQ(1.0, grid2.GridSpacing().x);
    EXPECT_DOUBLE_EQ(1.0, grid2.GridSpacing().y);
    EXPECT_DOUBLE_EQ(1.0, grid2.GridSpacing().z);
}