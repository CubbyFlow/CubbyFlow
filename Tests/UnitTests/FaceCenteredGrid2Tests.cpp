#include "pch.hpp"

#include <Core/Grid/FaceCenteredGrid2.hpp>

using namespace CubbyFlow;

TEST(FaceCenteredGrid2, Constructors)
{
    // Default constructors
    FaceCenteredGrid2 grid1;
    EXPECT_EQ(0u, grid1.Resolution().x);
    EXPECT_EQ(0u, grid1.Resolution().y);
    EXPECT_DOUBLE_EQ(1.0, grid1.GridSpacing().x);
    EXPECT_DOUBLE_EQ(1.0, grid1.GridSpacing().y);
    EXPECT_DOUBLE_EQ(0.0, grid1.Origin().x);
    EXPECT_DOUBLE_EQ(0.0, grid1.Origin().y);
    EXPECT_EQ(0u, grid1.GetUSize().x);
    EXPECT_EQ(0u, grid1.GetUSize().y);
    EXPECT_EQ(0u, grid1.GetVSize().x);
    EXPECT_EQ(0u, grid1.GetVSize().y);
    EXPECT_DOUBLE_EQ(0.0, grid1.GetUOrigin().x);
    EXPECT_DOUBLE_EQ(0.5, grid1.GetUOrigin().y);
    EXPECT_DOUBLE_EQ(0.5, grid1.GetVOrigin().x);
    EXPECT_DOUBLE_EQ(0.0, grid1.GetVOrigin().y);

    // Constructor with params
    FaceCenteredGrid2 grid2(5, 4, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
    EXPECT_EQ(5u, grid2.Resolution().x);
    EXPECT_EQ(4u, grid2.Resolution().y);
    EXPECT_DOUBLE_EQ(1.0, grid2.GridSpacing().x);
    EXPECT_DOUBLE_EQ(2.0, grid2.GridSpacing().y);
    EXPECT_DOUBLE_EQ(3.0, grid2.Origin().x);
    EXPECT_DOUBLE_EQ(4.0, grid2.Origin().y);
    EXPECT_EQ(6u, grid2.GetUSize().x);
    EXPECT_EQ(4u, grid2.GetUSize().y);
    EXPECT_EQ(5u, grid2.GetVSize().x);
    EXPECT_EQ(5u, grid2.GetVSize().y);
    EXPECT_DOUBLE_EQ(3.0, grid2.GetUOrigin().x);
    EXPECT_DOUBLE_EQ(5.0, grid2.GetUOrigin().y);
    EXPECT_DOUBLE_EQ(3.5, grid2.GetVOrigin().x);
    EXPECT_DOUBLE_EQ(4.0, grid2.GetVOrigin().y);
    grid2.ForEachUIndex(
        [&](size_t i, size_t j) { EXPECT_DOUBLE_EQ(5.0, grid2.GetU(i, j)); });
    grid2.ForEachVIndex(
        [&](size_t i, size_t j) { EXPECT_DOUBLE_EQ(6.0, grid2.GetV(i, j)); });

    // Copy constructor
    FaceCenteredGrid2 grid3(grid2);
    EXPECT_EQ(5u, grid3.Resolution().x);
    EXPECT_EQ(4u, grid3.Resolution().y);
    EXPECT_DOUBLE_EQ(1.0, grid3.GridSpacing().x);
    EXPECT_DOUBLE_EQ(2.0, grid3.GridSpacing().y);
    EXPECT_DOUBLE_EQ(3.0, grid3.Origin().x);
    EXPECT_DOUBLE_EQ(4.0, grid3.Origin().y);
    EXPECT_EQ(6u, grid3.GetUSize().x);
    EXPECT_EQ(4u, grid3.GetUSize().y);
    EXPECT_EQ(5u, grid3.GetVSize().x);
    EXPECT_EQ(5u, grid3.GetVSize().y);
    EXPECT_DOUBLE_EQ(3.0, grid3.GetUOrigin().x);
    EXPECT_DOUBLE_EQ(5.0, grid3.GetUOrigin().y);
    EXPECT_DOUBLE_EQ(3.5, grid3.GetVOrigin().x);
    EXPECT_DOUBLE_EQ(4.0, grid3.GetVOrigin().y);
    grid3.ForEachUIndex(
        [&](size_t i, size_t j) { EXPECT_DOUBLE_EQ(5.0, grid3.GetU(i, j)); });
    grid3.ForEachVIndex(
        [&](size_t i, size_t j) { EXPECT_DOUBLE_EQ(6.0, grid3.GetV(i, j)); });
}

TEST(FaceCenteredGrid2, Fill)
{
    FaceCenteredGrid2 grid(5, 4, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0);
    grid.Fill(Vector2D(42.0, 27.0));

    for (size_t j = 0; j < grid.GetUSize().y; ++j)
    {
        for (size_t i = 0; i < grid.GetUSize().x; ++i)
        {
            EXPECT_DOUBLE_EQ(42.0, grid.GetU(i, j));
        }
    }

    for (size_t j = 0; j < grid.GetVSize().y; ++j)
    {
        for (size_t i = 0; i < grid.GetVSize().x; ++i)
        {
            EXPECT_DOUBLE_EQ(27.0, grid.GetV(i, j));
        }
    }

    auto func = [](const Vector2D& x) { return x; };
    grid.Fill(func);

    for (size_t j = 0; j < grid.GetUSize().y; ++j)
    {
        for (size_t i = 0; i < grid.GetUSize().x; ++i)
        {
            EXPECT_DOUBLE_EQ(static_cast<double>(i), grid.GetU(i, j));
        }
    }

    for (size_t j = 0; j < grid.GetVSize().y; ++j)
    {
        for (size_t i = 0; i < grid.GetVSize().x; ++i)
        {
            EXPECT_DOUBLE_EQ(static_cast<double>(j), grid.GetV(i, j));
        }
    }
}

TEST(FaceCenteredGrid2, DivergenceAtCellCenter)
{
    FaceCenteredGrid2 grid(5, 8, 2.0, 3.0);

    grid.Fill(Vector2D(1.0, -2.0));

    for (size_t j = 0; j < grid.Resolution().y; ++j)
    {
        for (size_t i = 0; i < grid.Resolution().x; ++i)
        {
            EXPECT_DOUBLE_EQ(0.0, grid.DivergenceAtCellCenter(i, j));
        }
    }

    grid.Fill([](const Vector2D& x) { return x; });

    for (size_t j = 0; j < grid.Resolution().y; ++j)
    {
        for (size_t i = 0; i < grid.Resolution().x; ++i)
        {
            EXPECT_NEAR(2.0, grid.DivergenceAtCellCenter(i, j), 1e-6);
        }
    }
}

TEST(FaceCenteredGrid2, CurlAtCellCenter)
{
    FaceCenteredGrid2 grid(5, 8, 2.0, 3.0);

    grid.Fill(Vector2D(1.0, -2.0));

    for (size_t j = 0; j < grid.Resolution().y; ++j)
    {
        for (size_t i = 0; i < grid.Resolution().x; ++i)
        {
            EXPECT_DOUBLE_EQ(0.0, grid.CurlAtCellCenter(i, j));
        }
    }

    grid.Fill([](const Vector2D& x) { return Vector2D(-x.y, x.x); });

    for (size_t j = 1; j < grid.Resolution().y - 1; ++j)
    {
        for (size_t i = 1; i < grid.Resolution().x - 1; ++i)
        {
            EXPECT_NEAR(2.0, grid.CurlAtCellCenter(i, j), 1e-6);
        }
    }
}

TEST(FaceCenteredGrid2, ValueAtCellCenter)
{
    FaceCenteredGrid2 grid(5, 8, 2.0, 3.0);
    grid.Fill([&](const Vector2D& x) {
        return Vector2D(3.0 * x.y + 1.0, 5.0 * x.x + 7.0);
    });

    auto pos = grid.CellCenterPosition();
    grid.ForEachCellIndex([&](size_t i, size_t j) {
        Vector2D val = grid.ValueAtCellCenter(i, j);
        Vector2D x = pos(i, j);
        Vector2D expected = Vector2D(3.0 * x.y + 1.0, 5.0 * x.x + 7.0);
        EXPECT_NEAR(expected.x, val.x, 1e-6);
        EXPECT_NEAR(expected.y, val.y, 1e-6);
    });
}

TEST(FaceCenteredGrid2, Sample)
{
    FaceCenteredGrid2 grid(5, 8, 2.0, 3.0);
    grid.Fill([&](const Vector2D& x) {
        return Vector2D(3.0 * x.y + 1.0, 5.0 * x.x + 7.0);
    });

    auto pos = grid.CellCenterPosition();
    grid.ForEachCellIndex([&](size_t i, size_t j) {
        Vector2D x = pos(i, j);
        Vector2D val = grid.Sample(x);
        Vector2D expected = Vector2D(3.0 * x.y + 1.0, 5.0 * x.x + 7.0);
        EXPECT_NEAR(expected.x, val.x, 1e-6);
        EXPECT_NEAR(expected.y, val.y, 1e-6);
    });
}

TEST(FaceCenteredGrid2, Builder)
{
    {
        auto builder = FaceCenteredGrid2::GetBuilder();

        auto grid = builder.Build(Size2(5, 2), Vector2D(2.0, 4.0),
                                  Vector2D(-1.0, 2.0), Vector2D(3.0, 5.0));
        EXPECT_EQ(Size2(5, 2), grid->Resolution());
        EXPECT_EQ(Vector2D(2.0, 4.0), grid->GridSpacing());
        EXPECT_EQ(Vector2D(-1.0, 2.0), grid->Origin());

        auto faceCenteredGrid =
            std::dynamic_pointer_cast<FaceCenteredGrid2>(grid);
        EXPECT_TRUE(faceCenteredGrid != nullptr);

        faceCenteredGrid->ForEachUIndex(
            [&faceCenteredGrid](size_t i, size_t j) {
                EXPECT_DOUBLE_EQ(3.0, faceCenteredGrid->GetU(i, j));
            });
        faceCenteredGrid->ForEachVIndex(
            [&faceCenteredGrid](size_t i, size_t j) {
                EXPECT_DOUBLE_EQ(5.0, faceCenteredGrid->GetV(i, j));
            });
    }

    {
        auto grid = FaceCenteredGrid2::Builder()
                        .WithResolution(5, 2)
                        .WithGridSpacing(2, 4)
                        .WithOrigin(-1, 2)
                        .WithInitialValue(3, 5)
                        .Build();

        EXPECT_EQ(Size2(5, 2), grid.Resolution());
        EXPECT_EQ(Vector2D(2.0, 4.0), grid.GridSpacing());
        EXPECT_EQ(Vector2D(-1.0, 2.0), grid.Origin());

        grid.ForEachUIndex([&](size_t i, size_t j) {
            EXPECT_DOUBLE_EQ(3.0, grid.GetU(i, j));
        });
        grid.ForEachVIndex([&](size_t i, size_t j) {
            EXPECT_DOUBLE_EQ(5.0, grid.GetV(i, j));
        });
    }
}

TEST(FaceCenteredGrid2, Serialization)
{
    FaceCenteredGrid2 grid1(5, 4, 1.0, 2.0, -5.0, 3.0);
    grid1.Fill([&](const Vector2D& pt) { return Vector2D(pt.x, pt.y); });

    // Serialize to in-memory stream
    std::vector<uint8_t> buffer1;
    grid1.Serialize(&buffer1);

    // Deserialize to non-zero array
    FaceCenteredGrid2 grid2(1, 2, 0.5, 1.0, 0.5, 2.0);
    grid2.Deserialize(buffer1);
    EXPECT_EQ(5u, grid2.Resolution().x);
    EXPECT_EQ(4u, grid2.Resolution().y);
    EXPECT_DOUBLE_EQ(-5.0, grid2.Origin().x);
    EXPECT_DOUBLE_EQ(3.0, grid2.Origin().y);
    EXPECT_DOUBLE_EQ(1.0, grid2.GridSpacing().x);
    EXPECT_DOUBLE_EQ(2.0, grid2.GridSpacing().y);
    EXPECT_DOUBLE_EQ(-5.0, grid2.BoundingBox().lowerCorner.x);
    EXPECT_DOUBLE_EQ(3.0, grid2.BoundingBox().lowerCorner.y);
    EXPECT_DOUBLE_EQ(0.0, grid2.BoundingBox().upperCorner.x);
    EXPECT_DOUBLE_EQ(11.0, grid2.BoundingBox().upperCorner.y);
    EXPECT_EQ(6u, grid2.GetUSize().x);
    EXPECT_EQ(4u, grid2.GetUSize().y);
    EXPECT_EQ(5u, grid2.GetVSize().x);
    EXPECT_EQ(5u, grid2.GetVSize().y);
    EXPECT_DOUBLE_EQ(-5.0, grid2.GetUOrigin().x);
    EXPECT_DOUBLE_EQ(4.0, grid2.GetUOrigin().y);
    EXPECT_DOUBLE_EQ(-4.5, grid2.GetVOrigin().x);
    EXPECT_DOUBLE_EQ(3.0, grid2.GetVOrigin().y);

    grid1.ForEachUIndex([&](size_t i, size_t j) {
        EXPECT_DOUBLE_EQ(grid1.GetU(i, j), grid2.GetU(i, j));
    });

    grid1.ForEachVIndex([&](size_t i, size_t j) {
        EXPECT_DOUBLE_EQ(grid1.GetV(i, j), grid2.GetV(i, j));
    });

    // Serialize zero-sized array
    FaceCenteredGrid2 grid3;
    std::vector<uint8_t> buffer2;
    grid3.Serialize(&buffer2);

    // Deserialize to non-zero array
    grid2.Deserialize(buffer2);
    EXPECT_EQ(0u, grid2.Resolution().x);
    EXPECT_EQ(0u, grid2.Resolution().y);
    EXPECT_DOUBLE_EQ(0.0, grid2.Origin().x);
    EXPECT_DOUBLE_EQ(0.0, grid2.Origin().y);
    EXPECT_DOUBLE_EQ(1.0, grid2.GridSpacing().x);
    EXPECT_DOUBLE_EQ(1.0, grid2.GridSpacing().y);
    EXPECT_EQ(0u, grid2.GetUSize().x);
    EXPECT_EQ(0u, grid2.GetUSize().y);
    EXPECT_EQ(0u, grid2.GetVSize().x);
    EXPECT_EQ(0u, grid2.GetVSize().y);
    EXPECT_DOUBLE_EQ(0.0, grid2.GetUOrigin().x);
    EXPECT_DOUBLE_EQ(0.5, grid2.GetUOrigin().y);
    EXPECT_DOUBLE_EQ(0.5, grid2.GetVOrigin().x);
    EXPECT_DOUBLE_EQ(0.0, grid2.GetVOrigin().y);
}