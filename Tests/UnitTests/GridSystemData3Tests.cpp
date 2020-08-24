#include "pch.hpp"

#include <Core/Grid/CellCenteredScalarGrid3.hpp>
#include <Core/Grid/CellCenteredVectorGrid3.hpp>
#include <Core/Grid/GridSystemData3.hpp>
#include <Core/Grid/VertexCenteredScalarGrid3.hpp>
#include <Core/Grid/VertexCenteredVectorGrid3.hpp>

using namespace CubbyFlow;

TEST(GridSystemData3, Constructors)
{
    GridSystemData3 grids1;
    EXPECT_EQ(0u, grids1.GetResolution().x);
    EXPECT_EQ(0u, grids1.GetResolution().y);
    EXPECT_EQ(0u, grids1.GetResolution().z);
    EXPECT_EQ(1.0, grids1.GetGridSpacing().x);
    EXPECT_EQ(1.0, grids1.GetGridSpacing().y);
    EXPECT_EQ(1.0, grids1.GetGridSpacing().z);
    EXPECT_EQ(0.0, grids1.GetOrigin().x);
    EXPECT_EQ(0.0, grids1.GetOrigin().y);
    EXPECT_EQ(0.0, grids1.GetOrigin().z);

    GridSystemData3 grids2({ 32, 64, 48 }, { 1.0, 2.0, 3.0 },
                           { -5.0, 4.5, 10.0 });
    EXPECT_EQ(32u, grids2.GetResolution().x);
    EXPECT_EQ(64u, grids2.GetResolution().y);
    EXPECT_EQ(48u, grids2.GetResolution().z);
    EXPECT_EQ(1.0, grids2.GetGridSpacing().x);
    EXPECT_EQ(2.0, grids2.GetGridSpacing().y);
    EXPECT_EQ(3.0, grids2.GetGridSpacing().z);
    EXPECT_EQ(-5.0, grids2.GetOrigin().x);
    EXPECT_EQ(4.5, grids2.GetOrigin().y);
    EXPECT_EQ(10.0, grids2.GetOrigin().z);

    GridSystemData3 grids3(grids2);

    EXPECT_EQ(32u, grids3.GetResolution().x);
    EXPECT_EQ(64u, grids3.GetResolution().y);
    EXPECT_EQ(48u, grids3.GetResolution().z);
    EXPECT_EQ(1.0, grids3.GetGridSpacing().x);
    EXPECT_EQ(2.0, grids3.GetGridSpacing().y);
    EXPECT_EQ(3.0, grids3.GetGridSpacing().z);
    EXPECT_EQ(-5.0, grids3.GetOrigin().x);
    EXPECT_EQ(4.5, grids3.GetOrigin().y);
    EXPECT_EQ(10.0, grids3.GetOrigin().z);

    EXPECT_TRUE(grids2.GetVelocity() != grids3.GetVelocity());
}

TEST(GridSystemData3, Serialize)
{
    std::vector<uint8_t> buffer;

    GridSystemData3 grids({ 32, 64, 48 }, { 1.0, 2.0, 3.0 },
                          { -5.0, 4.5, 10.0 });

    size_t scalarIdx0 = grids.AddScalarData(
        std::make_shared<CellCenteredScalarGrid3::Builder>());
    size_t vectorIdx0 = grids.AddVectorData(
        std::make_shared<CellCenteredVectorGrid3::Builder>());
    size_t scalarIdx1 = grids.AddAdvectableScalarData(
        std::make_shared<VertexCenteredScalarGrid3::Builder>());
    size_t vectorIdx1 = grids.AddAdvectableVectorData(
        std::make_shared<VertexCenteredVectorGrid3::Builder>());

    auto scalar0 = grids.GetScalarDataAt(scalarIdx0);
    auto vector0 = grids.GetVectorDataAt(vectorIdx0);
    auto scalar1 = grids.GetAdvectableScalarDataAt(scalarIdx1);
    auto vector1 = grids.GetAdvectableVectorDataAt(vectorIdx1);

    scalar0->Fill([](const Vector3D& pt) { return pt.Length(); });

    vector0->Fill([](const Vector3D& pt) { return pt; });

    scalar1->Fill(
        [](const Vector3D& pt) { return (pt - Vector3D(1, 2, 3)).Length(); });

    vector1->Fill([](const Vector3D& pt) { return pt - Vector3D(1, 2, 3); });

    grids.Serialize(&buffer);

    GridSystemData3 grids2;
    grids2.Deserialize(buffer);

    EXPECT_EQ(32u, grids2.GetResolution().x);
    EXPECT_EQ(64u, grids2.GetResolution().y);
    EXPECT_EQ(48u, grids2.GetResolution().z);
    EXPECT_EQ(1.0, grids2.GetGridSpacing().x);
    EXPECT_EQ(2.0, grids2.GetGridSpacing().y);
    EXPECT_EQ(3.0, grids2.GetGridSpacing().z);
    EXPECT_EQ(-5.0, grids2.GetOrigin().x);
    EXPECT_EQ(4.5, grids2.GetOrigin().y);
    EXPECT_EQ(10.0, grids2.GetOrigin().z);

    EXPECT_EQ(1u, grids2.GetNumberOfScalarData());
    EXPECT_EQ(1u, grids2.GetNumberOfVectorData());
    EXPECT_EQ(1u, grids2.GetNumberOfAdvectableScalarData());
    EXPECT_EQ(2u, grids2.GetNumberOfAdvectableVectorData());

    auto scalar0_2 = grids2.GetScalarDataAt(scalarIdx0);
    EXPECT_TRUE(std::dynamic_pointer_cast<CellCenteredScalarGrid3>(scalar0_2) !=
                nullptr);
    EXPECT_EQ(scalar0->Resolution(), scalar0_2->Resolution());
    EXPECT_EQ(scalar0->GridSpacing(), scalar0_2->GridSpacing());
    EXPECT_EQ(scalar0->Origin(), scalar0_2->Origin());
    EXPECT_EQ(scalar0->GetDataSize(), scalar0_2->GetDataSize());
    EXPECT_EQ(scalar0->GetDataOrigin(), scalar0_2->GetDataOrigin());
    scalar0->ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_EQ((*scalar0)(i, j, k), (*scalar0_2)(i, j, k));
    });

    auto vector0_2 = grids2.GetVectorDataAt(vectorIdx0);
    auto cell_vector0 =
        std::dynamic_pointer_cast<CellCenteredVectorGrid3>(vector0);
    auto cell_vector0_2 =
        std::dynamic_pointer_cast<CellCenteredVectorGrid3>(vector0_2);
    EXPECT_TRUE(cell_vector0_2 != nullptr);
    EXPECT_EQ(vector0->Resolution(), vector0_2->Resolution());
    EXPECT_EQ(vector0->GridSpacing(), vector0_2->GridSpacing());
    EXPECT_EQ(vector0->Origin(), vector0_2->Origin());
    EXPECT_EQ(cell_vector0->GetDataSize(), cell_vector0_2->GetDataSize());
    EXPECT_EQ(cell_vector0->GetDataOrigin(), cell_vector0_2->GetDataOrigin());
    cell_vector0->ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_EQ((*cell_vector0)(i, j, k), (*cell_vector0_2)(i, j, k));
    });

    auto scalar1_2 = grids2.GetAdvectableScalarDataAt(scalarIdx1);
    EXPECT_TRUE(std::dynamic_pointer_cast<VertexCenteredScalarGrid3>(
                    scalar1_2) != nullptr);
    EXPECT_EQ(scalar1->Resolution(), scalar1_2->Resolution());
    EXPECT_EQ(scalar1->GridSpacing(), scalar1_2->GridSpacing());
    EXPECT_EQ(scalar1->Origin(), scalar1_2->Origin());
    EXPECT_EQ(scalar1->GetDataSize(), scalar1_2->GetDataSize());
    EXPECT_EQ(scalar1->GetDataOrigin(), scalar1_2->GetDataOrigin());
    scalar1->ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_EQ((*scalar1)(i, j, k), (*scalar1_2)(i, j, k));
    });

    auto vector1_2 = grids2.GetAdvectableVectorDataAt(vectorIdx1);
    auto vert_vector1 =
        std::dynamic_pointer_cast<VertexCenteredVectorGrid3>(vector1);
    auto vert_vector1_2 =
        std::dynamic_pointer_cast<VertexCenteredVectorGrid3>(vector1_2);
    EXPECT_TRUE(vert_vector1_2 != nullptr);
    EXPECT_EQ(vector1->Resolution(), vector1_2->Resolution());
    EXPECT_EQ(vector1->GridSpacing(), vector1_2->GridSpacing());
    EXPECT_EQ(vector1->Origin(), vector1_2->Origin());
    EXPECT_EQ(vert_vector1->GetDataSize(), vert_vector1_2->GetDataSize());
    EXPECT_EQ(vert_vector1->GetDataOrigin(), vert_vector1_2->GetDataOrigin());
    vert_vector1->ForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_EQ((*vert_vector1)(i, j, k), (*vert_vector1_2)(i, j, k));
    });

    auto velocity = grids.GetVelocity();
    auto velocity2 = grids2.GetVelocity();
    EXPECT_EQ(velocity->Resolution(), velocity2->Resolution());
    EXPECT_EQ(velocity->GridSpacing(), velocity2->GridSpacing());
    EXPECT_EQ(velocity->Origin(), velocity2->Origin());
    EXPECT_EQ(velocity->GetUSize(), velocity2->GetUSize());
    EXPECT_EQ(velocity->GetVSize(), velocity2->GetVSize());
    EXPECT_EQ(velocity->GetWSize(), velocity2->GetWSize());
    EXPECT_EQ(velocity->GetUOrigin(), velocity2->GetUOrigin());
    EXPECT_EQ(velocity->GetVOrigin(), velocity2->GetVOrigin());
    EXPECT_EQ(velocity->GetWOrigin(), velocity2->GetWOrigin());
    velocity->ForEachUIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_EQ(velocity->GetU(i, j, k), velocity2->GetU(i, j, k));
    });
    velocity->ForEachVIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_EQ(velocity->GetV(i, j, k), velocity2->GetV(i, j, k));
    });
    velocity->ForEachWIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_EQ(velocity->GetW(i, j, k), velocity2->GetW(i, j, k));
    });
}