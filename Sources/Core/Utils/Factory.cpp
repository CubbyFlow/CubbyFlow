// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/CellCenteredScalarGrid2.hpp>
#include <Core/Grid/CellCenteredScalarGrid3.hpp>
#include <Core/Grid/CellCenteredVectorGrid2.hpp>
#include <Core/Grid/CellCenteredVectorGrid3.hpp>
#include <Core/Grid/FaceCenteredGrid2.hpp>
#include <Core/Grid/FaceCenteredGrid3.hpp>
#include <Core/Grid/VertexCenteredScalarGrid2.hpp>
#include <Core/Grid/VertexCenteredScalarGrid3.hpp>
#include <Core/Grid/VertexCenteredVectorGrid2.hpp>
#include <Core/Grid/VertexCenteredVectorGrid3.hpp>
#include <Core/Searcher/PointHashGridSearcher2.hpp>
#include <Core/Searcher/PointHashGridSearcher3.hpp>
#include <Core/Searcher/PointKdTreeSearcher2.hpp>
#include <Core/Searcher/PointKdTreeSearcher3.hpp>
#include <Core/Searcher/PointParallelHashGridSearcher2.hpp>
#include <Core/Searcher/PointParallelHashGridSearcher3.hpp>
#include <Core/Searcher/PointSimpleListSearcher2.hpp>
#include <Core/Searcher/PointSimpleListSearcher3.hpp>
#include <Core/Utils/Factory.hpp>

#include <memory>
#include <unordered_map>

namespace CubbyFlow
{
std::unordered_map<std::string, ScalarGridBuilder2Ptr> sScalarGrid2Builders;
std::unordered_map<std::string, ScalarGridBuilder3Ptr> sScalarGrid3Builders;

std::unordered_map<std::string, VectorGridBuilder2Ptr> sVectorGrid2Builders;
std::unordered_map<std::string, VectorGridBuilder3Ptr> sVectorGrid3Builders;

std::unordered_map<std::string, PointNeighborSearcherBuilder2Ptr>
    sPointNeighborSearcher2Builders;
std::unordered_map<std::string, PointNeighborSearcherBuilder3Ptr>
    sPointNeighborSearcher3Builders;

#define REGISTER_BUILDER(map, ClassName) \
    map.emplace(#ClassName, std::make_shared<ClassName::Builder>());

#define REGISTER_SCALAR_GRID2_BUILDER(ClassName) \
    REGISTER_BUILDER(sScalarGrid2Builders, ClassName)
#define REGISTER_SCALAR_GRID3_BUILDER(ClassName) \
    REGISTER_BUILDER(sScalarGrid3Builders, ClassName)

#define REGISTER_VECTOR_GRID2_BUILDER(ClassName) \
    REGISTER_BUILDER(sVectorGrid2Builders, ClassName)
#define REGISTER_VECTOR_GRID3_BUILDER(ClassName) \
    REGISTER_BUILDER(sVectorGrid3Builders, ClassName)

#define REGISTER_POINT_NEIGHBOR_SEARCHER2_BUILDER(ClassName) \
    REGISTER_BUILDER(sPointNeighborSearcher2Builders, ClassName)
#define REGISTER_POINT_NEIGHBOR_SEARCHER3_BUILDER(ClassName) \
    REGISTER_BUILDER(sPointNeighborSearcher3Builders, ClassName)

class Registry
{
 public:
    Registry()
    {
        REGISTER_SCALAR_GRID2_BUILDER(CellCenteredScalarGrid2)
        REGISTER_SCALAR_GRID3_BUILDER(CellCenteredScalarGrid3)

        REGISTER_VECTOR_GRID2_BUILDER(CellCenteredVectorGrid2)
        REGISTER_VECTOR_GRID3_BUILDER(CellCenteredVectorGrid3)

        REGISTER_VECTOR_GRID2_BUILDER(FaceCenteredGrid2)
        REGISTER_VECTOR_GRID3_BUILDER(FaceCenteredGrid3)

        REGISTER_SCALAR_GRID2_BUILDER(VertexCenteredScalarGrid2)
        REGISTER_SCALAR_GRID3_BUILDER(VertexCenteredScalarGrid3)

        REGISTER_VECTOR_GRID2_BUILDER(VertexCenteredVectorGrid2)
        REGISTER_VECTOR_GRID3_BUILDER(VertexCenteredVectorGrid3)

        REGISTER_POINT_NEIGHBOR_SEARCHER2_BUILDER(PointHashGridSearcher2)
        REGISTER_POINT_NEIGHBOR_SEARCHER3_BUILDER(PointHashGridSearcher3)

        REGISTER_POINT_NEIGHBOR_SEARCHER2_BUILDER(
            PointParallelHashGridSearcher2)
        REGISTER_POINT_NEIGHBOR_SEARCHER3_BUILDER(
            PointParallelHashGridSearcher3)

        REGISTER_POINT_NEIGHBOR_SEARCHER2_BUILDER(PointSimpleListSearcher2)
        REGISTER_POINT_NEIGHBOR_SEARCHER3_BUILDER(PointSimpleListSearcher3)

        REGISTER_POINT_NEIGHBOR_SEARCHER2_BUILDER(PointKdTreeSearcher2)
        REGISTER_POINT_NEIGHBOR_SEARCHER3_BUILDER(PointKdTreeSearcher3)
    }
};

static Registry sRegistry;

ScalarGrid2Ptr Factory::BuildScalarGrid2(const std::string& name)
{
    const auto result = sScalarGrid2Builders.find(name);
    if (result != sScalarGrid2Builders.end())
    {
        const auto builder = result->second;
        return builder->Build({ 0, 0 }, { 1, 1 }, { 0, 0 }, 0.0);
    }

    return nullptr;
}

ScalarGrid3Ptr Factory::BuildScalarGrid3(const std::string& name)
{
    const auto result = sScalarGrid3Builders.find(name);
    if (result != sScalarGrid3Builders.end())
    {
        const auto builder = result->second;
        return builder->Build({ 0, 0, 0 }, { 1, 1, 1 }, { 0, 0, 0 }, 0.0);
    }

    return nullptr;
}

VectorGrid2Ptr Factory::BuildVectorGrid2(const std::string& name)
{
    const auto result = sVectorGrid2Builders.find(name);
    if (result != sVectorGrid2Builders.end())
    {
        const auto builder = result->second;
        return builder->Build({ 0, 0 }, { 1, 1 }, { 0, 0 }, { 0, 0 });
    }

    return nullptr;
}

VectorGrid3Ptr Factory::BuildVectorGrid3(const std::string& name)
{
    const auto result = sVectorGrid3Builders.find(name);
    if (result != sVectorGrid3Builders.end())
    {
        const auto builder = result->second;
        return builder->Build({ 0, 0, 0 }, { 1, 1, 1 }, { 0, 0, 0 },
                              { 0, 0, 0 });
    }

    return nullptr;
}

PointNeighborSearcher2Ptr Factory::BuildPointNeighborSearcher2(
    const std::string& name)
{
    const auto result = sPointNeighborSearcher2Builders.find(name);
    if (result != sPointNeighborSearcher2Builders.end())
    {
        const auto builder = result->second;
        return builder->BuildPointNeighborSearcher();
    }

    return nullptr;
}

PointNeighborSearcher3Ptr Factory::BuildPointNeighborSearcher3(
    const std::string& name)
{
    const auto result = sPointNeighborSearcher3Builders.find(name);
    if (result != sPointNeighborSearcher3Builders.end())
    {
        const auto builder = result->second;
        return builder->BuildPointNeighborSearcher();
    }

    return nullptr;
}
}  // namespace CubbyFlow