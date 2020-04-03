#include "pch.hpp"

#include <Core/Grid/FaceCenteredGrid3.hpp>

using namespace CubbyFlow;

TEST(FaceCenteredGrid3, Constructors)
{
    // Default constructors
    FaceCenteredGrid3 grid1;
    EXPECT_EQ(0u, grid1.Resolution().x);
    EXPECT_EQ(0u, grid1.Resolution().y);
    EXPECT_EQ(0u, grid1.Resolution().z);
    EXPECT_DOUBLE_EQ(1.0, grid1.GridSpacing().x);
    EXPECT_DOUBLE_EQ(1.0, grid1.GridSpacing().y);
    EXPECT_DOUBLE_EQ(1.0, grid1.GridSpacing().z);
    EXPECT_DOUBLE_EQ(0.0, grid1.Origin().x);
    EXPECT_DOUBLE_EQ(0.0, grid1.Origin().y);
    EXPECT_DOUBLE_EQ(0.0, grid1.Origin().z);
    EXPECT_EQ(0u, grid1.GetUSize().x);
    EXPECT_EQ(0u, grid1.GetUSize().y);
    EXPECT_EQ(0u, grid1.GetUSize().z);
    EXPECT_EQ(0u, grid1.GetVSize().x);
    EXPECT_EQ(0u, grid1.GetVSize().y);
    EXPECT_EQ(0u, grid1.GetVSize().z);
    EXPECT_EQ(0u, grid1.GetWSize().x);
    EXPECT_EQ(0u, grid1.GetWSize().y);
    EXPECT_EQ(0u, grid1.GetWSize().z);
    EXPECT_DOUBLE_EQ(0.0, grid1.GetUOrigin().x);
    EXPECT_DOUBLE_EQ(0.5, grid1.GetUOrigin().y);
    EXPECT_DOUBLE_EQ(0.5, grid1.GetUOrigin().z);
    EXPECT_DOUBLE_EQ(0.5, grid1.GetVOrigin().x);
    EXPECT_DOUBLE_EQ(0.0, grid1.GetVOrigin().y);
    EXPECT_DOUBLE_EQ(0.5, grid1.GetVOrigin().z);
    EXPECT_DOUBLE_EQ(0.5, grid1.GetWOrigin().x);
    EXPECT_DOUBLE_EQ(0.5, grid1.GetWOrigin().y);
    EXPECT_DOUBLE_EQ(0.0, grid1.GetWOrigin().z);

    // Constructor with params
    FaceCenteredGrid3 grid2(5, 4, 3, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0,
                            9.0);
    EXPECT_EQ(5u, grid2.Resolution().x);
    EXPECT_EQ(4u, grid2.Resolution().y);
    EXPECT_EQ(3u, grid2.Resolution().z);
    EXPECT_DOUBLE_EQ(1.0, grid2.GridSpacing().x);
    EXPECT_DOUBLE_EQ(2.0, grid2.GridSpacing().y);
    EXPECT_DOUBLE_EQ(3.0, grid2.GridSpacing().z);
    EXPECT_DOUBLE_EQ(4.0, grid2.Origin().x);
    EXPECT_DOUBLE_EQ(5.0, grid2.Origin().y);
    EXPECT_DOUBLE_EQ(6.0, grid2.Origin().z);
    EXPECT_EQ(6u, grid2.GetUSize().x);
    EXPECT_EQ(4u, grid2.GetUSize().y);
    EXPECT_EQ(3u, grid2.GetUSize().z);
    EXPECT_EQ(5u, grid2.GetVSize().x);
    EXPECT_EQ(5u, grid2.GetVSize().y);
    EXPECT_EQ(3u, grid2.GetVSize().z);
    EXPECT_EQ(5u, grid2.GetWSize().x);
    EXPECT_EQ(4u, grid2.GetWSize().y);
    EXPECT_EQ(4u, grid2.GetWSize().z);
    EXPECT_DOUBLE_EQ(4.0, grid2.GetUOrigin().x);
    EXPECT_DOUBLE_EQ(6.0, grid2.GetUOrigin().y);
    EXPECT_DOUBLE_EQ(7.5, grid2.GetUOrigin().z);
    EXPECT_DOUBLE_EQ(4.5, grid2.GetVOrigin().x);
    EXPECT_DOUBLE_EQ(5.0, grid2.GetVOrigin().y);
    EXPECT_DOUBLE_EQ(7.5, grid2.GetVOrigin().z);
    EXPECT_DOUBLE_EQ(4.5, grid2.GetWOrigin().x);
    EXPECT_DOUBLE_EQ(6.0, grid2.GetWOrigin().y);
    EXPECT_DOUBLE_EQ(6.0, grid2.GetWOrigin().z);
    grid2.ForEachUIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(7.0, grid2.GetU(i, j, k));
    });
    grid2.ForEachVIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(8.0, grid2.GetV(i, j, k));
    });
    grid2.ForEachWIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(9.0, grid2.GetW(i, j, k));
    });

    // Copy constructor
    FaceCenteredGrid3 grid3(grid2);
    EXPECT_EQ(5u, grid3.Resolution().x);
    EXPECT_EQ(4u, grid3.Resolution().y);
    EXPECT_EQ(3u, grid3.Resolution().z);
    EXPECT_DOUBLE_EQ(1.0, grid3.GridSpacing().x);
    EXPECT_DOUBLE_EQ(2.0, grid3.GridSpacing().y);
    EXPECT_DOUBLE_EQ(3.0, grid3.GridSpacing().z);
    EXPECT_DOUBLE_EQ(4.0, grid3.Origin().x);
    EXPECT_DOUBLE_EQ(5.0, grid3.Origin().y);
    EXPECT_DOUBLE_EQ(6.0, grid3.Origin().z);
    EXPECT_EQ(6u, grid3.GetUSize().x);
    EXPECT_EQ(4u, grid3.GetUSize().y);
    EXPECT_EQ(3u, grid3.GetUSize().z);
    EXPECT_EQ(5u, grid3.GetVSize().x);
    EXPECT_EQ(5u, grid3.GetVSize().y);
    EXPECT_EQ(3u, grid3.GetVSize().z);
    EXPECT_EQ(5u, grid3.GetWSize().x);
    EXPECT_EQ(4u, grid3.GetWSize().y);
    EXPECT_EQ(4u, grid3.GetWSize().z);
    EXPECT_DOUBLE_EQ(4.0, grid3.GetUOrigin().x);
    EXPECT_DOUBLE_EQ(6.0, grid3.GetUOrigin().y);
    EXPECT_DOUBLE_EQ(7.5, grid3.GetUOrigin().z);
    EXPECT_DOUBLE_EQ(4.5, grid3.GetVOrigin().x);
    EXPECT_DOUBLE_EQ(5.0, grid3.GetVOrigin().y);
    EXPECT_DOUBLE_EQ(7.5, grid3.GetVOrigin().z);
    EXPECT_DOUBLE_EQ(4.5, grid3.GetWOrigin().x);
    EXPECT_DOUBLE_EQ(6.0, grid3.GetWOrigin().y);
    EXPECT_DOUBLE_EQ(6.0, grid3.GetWOrigin().z);
    grid3.ForEachUIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(7.0, grid3.GetU(i, j, k));
    });
    grid3.ForEachVIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(8.0, grid3.GetV(i, j, k));
    });
    grid3.ForEachWIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(9.0, grid3.GetW(i, j, k));
    });
}

TEST(FaceCenteredGrid3, Fill)
{
    FaceCenteredGrid3 grid(5, 4, 6, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                           0.0);
    grid.Fill(Vector3D(42.0, 27.0, 31.0));

    for (size_t k = 0; k < grid.GetUSize().z; ++k)
    {
        for (size_t j = 0; j < grid.GetUSize().y; ++j)
        {
            for (size_t i = 0; i < grid.GetUSize().x; ++i)
            {
                EXPECT_DOUBLE_EQ(42.0, grid.GetU(i, j, k));
            }
        }
    }

    for (size_t k = 0; k < grid.GetVSize().z; ++k)
    {
        for (size_t j = 0; j < grid.GetVSize().y; ++j)
        {
            for (size_t i = 0; i < grid.GetVSize().x; ++i)
            {
                EXPECT_DOUBLE_EQ(27.0, grid.GetV(i, j, k));
            }
        }
    }

    for (size_t k = 0; k < grid.GetWSize().z; ++k)
    {
        for (size_t j = 0; j < grid.GetWSize().y; ++j)
        {
            for (size_t i = 0; i < grid.GetWSize().x; ++i)
            {
                EXPECT_DOUBLE_EQ(31.0, grid.GetW(i, j, k));
            }
        }
    }

    auto func = [](const Vector3D& x) { return x; };
    grid.Fill(func);

    for (size_t k = 0; k < grid.GetUSize().z; ++k)
    {
        for (size_t j = 0; j < grid.GetUSize().y; ++j)
        {
            for (size_t i = 0; i < grid.GetUSize().x; ++i)
            {
                EXPECT_DOUBLE_EQ(static_cast<double>(i), grid.GetU(i, j, k));
            }
        }
    }

    for (size_t k = 0; k < grid.GetVSize().z; ++k)
    {
        for (size_t j = 0; j < grid.GetVSize().y; ++j)
        {
            for (size_t i = 0; i < grid.GetVSize().x; ++i)
            {
                EXPECT_DOUBLE_EQ(static_cast<double>(j), grid.GetV(i, j, k));
            }
        }
    }

    for (size_t k = 0; k < grid.GetWSize().z; ++k)
    {
        for (size_t j = 0; j < grid.GetWSize().y; ++j)
        {
            for (size_t i = 0; i < grid.GetWSize().x; ++i)
            {
                EXPECT_DOUBLE_EQ(static_cast<double>(k), grid.GetW(i, j, k));
            }
        }
    }
}

TEST(FaceCenteredGrid3, DivergenceAtCellCenter)
{
    FaceCenteredGrid3 grid(5, 8, 6);

    grid.Fill(Vector3D(1.0, -2.0, 3.0));

    for (size_t k = 0; k < grid.Resolution().z; ++k)
    {
        for (size_t j = 0; j < grid.Resolution().y; ++j)
        {
            for (size_t i = 0; i < grid.Resolution().x; ++i)
            {
                EXPECT_DOUBLE_EQ(0.0, grid.DivergenceAtCellCenter(i, j, k));
            }
        }
    }

    grid.Fill([](const Vector3D& x) { return x; });

    for (size_t k = 0; k < grid.Resolution().z; ++k)
    {
        for (size_t j = 0; j < grid.Resolution().y; ++j)
        {
            for (size_t i = 0; i < grid.Resolution().x; ++i)
            {
                EXPECT_DOUBLE_EQ(3.0, grid.DivergenceAtCellCenter(i, j, k));
            }
        }
    }
}

TEST(FaceCenteredGrid3, CurlAtCellCenter)
{
    FaceCenteredGrid3 grid(5, 8, 6, 2.0, 3.0, 1.5);

    grid.Fill(Vector3D(1.0, -2.0, 3.0));

    for (size_t k = 0; k < grid.Resolution().z; ++k)
    {
        for (size_t j = 0; j < grid.Resolution().y; ++j)
        {
            for (size_t i = 0; i < grid.Resolution().x; ++i)
            {
                Vector3D curl = grid.CurlAtCellCenter(i, j, k);
                EXPECT_DOUBLE_EQ(0.0, curl.x);
                EXPECT_DOUBLE_EQ(0.0, curl.y);
                EXPECT_DOUBLE_EQ(0.0, curl.z);
            }
        }
    }

    grid.Fill([](const Vector3D& x) { return Vector3D(x.y, x.z, x.x); });

    for (size_t k = 1; k < grid.Resolution().z - 1; ++k)
    {
        for (size_t j = 1; j < grid.Resolution().y - 1; ++j)
        {
            for (size_t i = 1; i < grid.Resolution().x - 1; ++i)
            {
                Vector3D curl = grid.CurlAtCellCenter(i, j, k);
                EXPECT_DOUBLE_EQ(-1.0, curl.x);
                EXPECT_DOUBLE_EQ(-1.0, curl.y);
                EXPECT_DOUBLE_EQ(-1.0, curl.z);
            }
        }
    }
}

TEST(FaceCenteredGrid3, ValueAtCellCenter)
{
    FaceCenteredGrid3 grid(5, 8, 6, 2.0, 3.0, 1.5);
    grid.Fill([&](const Vector3D& x) {
        return Vector3D(3.0 * x.y + 1.0, 5.0 * x.z + 7.0, -1.0 * x.x - 9.0);
    });

    auto pos = grid.CellCenterPosition();
    grid.ForEachCellIndex([&](size_t i, size_t j, size_t k) {
        Vector3D val = grid.ValueAtCellCenter(i, j, k);
        Vector3D x = pos(i, j, k);
        Vector3D expected =
            Vector3D(3.0 * x.y + 1.0, 5.0 * x.z + 7.0, -1.0 * x.x - 9.0);
        EXPECT_NEAR(expected.x, val.x, 1e-6);
        EXPECT_NEAR(expected.y, val.y, 1e-6);
        EXPECT_NEAR(expected.z, val.z, 1e-6);
    });
}

TEST(FaceCenteredGrid3, Sample)
{
    FaceCenteredGrid3 grid(5, 8, 6, 2.0, 3.0, 1.5);
    grid.Fill([&](const Vector3D& x) {
        return Vector3D(3.0 * x.y + 1.0, 5.0 * x.z + 7.0, -1.0 * x.x - 9.0);
    });

    auto pos = grid.CellCenterPosition();
    grid.ForEachCellIndex([&](size_t i, size_t j, size_t k) {
        Vector3D x = pos(i, j, k);
        Vector3D val = grid.Sample(x);
        Vector3D expected =
            Vector3D(3.0 * x.y + 1.0, 5.0 * x.z + 7.0, -1.0 * x.x - 9.0);
        EXPECT_NEAR(expected.x, val.x, 1e-6);
        EXPECT_NEAR(expected.y, val.y, 1e-6);
        EXPECT_NEAR(expected.z, val.z, 1e-6);
    });
}

TEST(FaceCenteredGrid3, Builder)
{
    {
        auto builder = FaceCenteredGrid3::Builder();

        auto grid =
            builder.Build(Size3(5, 2, 7), Vector3D(2.0, 4.0, 1.5),
                          Vector3D(-1.0, 2.0, 7.0), Vector3D(3.0, 5.0, -2.0));
        EXPECT_EQ(Size3(5, 2, 7), grid->Resolution());
        EXPECT_EQ(Vector3D(2.0, 4.0, 1.5), grid->GridSpacing());
        EXPECT_EQ(Vector3D(-1.0, 2.0, 7.0), grid->Origin());

        auto faceCenteredGrid =
            std::dynamic_pointer_cast<FaceCenteredGrid3>(grid);
        faceCenteredGrid->ForEachUIndex(
            [&faceCenteredGrid](size_t i, size_t j, size_t k) {
                EXPECT_DOUBLE_EQ(3.0, faceCenteredGrid->GetU(i, j, k));
            });
        faceCenteredGrid->ForEachVIndex(
            [&faceCenteredGrid](size_t i, size_t j, size_t k) {
                EXPECT_DOUBLE_EQ(5.0, faceCenteredGrid->GetV(i, j, k));
            });
        faceCenteredGrid->ForEachWIndex(
            [&faceCenteredGrid](size_t i, size_t j, size_t k) {
                EXPECT_DOUBLE_EQ(-2.0, faceCenteredGrid->GetW(i, j, k));
            });
    }

    {
        auto grid = FaceCenteredGrid3::Builder()
                        .WithResolution(5, 2, 7)
                        .WithGridSpacing(2.0, 4.0, 1.5)
                        .WithOrigin(-1.0, 2.0, 7.0)
                        .WithInitialValue(3.0, 5.0, -2.0)
                        .Build();
        EXPECT_EQ(Size3(5, 2, 7), grid.Resolution());
        EXPECT_EQ(Vector3D(2.0, 4.0, 1.5), grid.GridSpacing());
        EXPECT_EQ(Vector3D(-1.0, 2.0, 7.0), grid.Origin());

        grid.ForEachUIndex([&](size_t i, size_t j, size_t k) {
            EXPECT_DOUBLE_EQ(3.0, grid.GetU(i, j, k));
        });
        grid.ForEachVIndex([&](size_t i, size_t j, size_t k) {
            EXPECT_DOUBLE_EQ(5.0, grid.GetV(i, j, k));
        });
        grid.ForEachWIndex([&](size_t i, size_t j, size_t k) {
            EXPECT_DOUBLE_EQ(-2.0, grid.GetW(i, j, k));
        });
    }
}

TEST(FaceCenteredGrid3, Serialization)
{
    FaceCenteredGrid3 grid1(5, 4, 3, 1.0, 2.0, 3.0, -5.0, 3.0, 1.0);
    grid1.Fill([&](const Vector3D& pt) { return Vector3D(pt.x, pt.y, pt.z); });

    // Serialize to in-memory stream
    std::vector<uint8_t> buffer1;
    grid1.Serialize(&buffer1);

    // Deserialize to non-zero array
    FaceCenteredGrid3 grid2(1, 2, 4, 0.5, 1.0, 2.0, 0.5, 2.0, -3.0);
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

    grid1.ForEachUIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(grid1.GetU(i, j, k), grid2.GetU(i, j, k));
    });
    grid1.ForEachVIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(grid1.GetV(i, j, k), grid2.GetV(i, j, k));
    });
    grid1.ForEachWIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_DOUBLE_EQ(grid1.GetW(i, j, k), grid2.GetW(i, j, k));
    });

    // Serialize zero-sized array
    FaceCenteredGrid3 grid3;
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