// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "gtest/gtest.h"

#include <Core/Array/Array.hpp>
#include <Core/CUDA/CUDAArray.hpp>
#include <Core/CUDA/CUDAPointHashGridSearcher3.hpp>
#include <Core/Searcher/PointParallelHashGridSearcher.hpp>

using namespace CubbyFlow;

namespace
{
struct ForEachCallback
{
    CUBBYFLOW_CUDA_HOST_DEVICE void operator()(size_t i, float4 o, size_t j,
                                               float4 pt)
    {
        (void)i;
        (void)o;

        visited[j] = 1;

        if (j == 1)
        {
            isValid[j] = 0;
            return;
        }

        if (j == 0)
        {
            isValid[j] = points[0] == pt;
        }
        else if (j == 2)
        {
            isValid[j] = points[2] == pt;
        }
    }

    float4* points;
    int* isValid;
    int* visited;
};

}  // namespace

TEST(CUDAPointHashGridSearcher3, Build)
{
    // CPU baseline
    Array1<Vector3D> points = { Vector3D(0, 1, 3), Vector3D(2, 5, 4),
                                Vector3D(-1, 3, 0) };

    PointParallelHashGridSearcher3 searcher({ 4, 4, 4 }, std::sqrt(10.0f));
    searcher.Build(points);

    // GPU
    CUDAArray1<float4> pointsD(3);
    pointsD[0] = make_float4(0, 1, 3, 0);
    pointsD[1] = make_float4(2, 5, 4, 0);
    pointsD[2] = make_float4(-1, 3, 0, 0);

    CUDAPointHashGridSearcher3 searcherD({ 4, 4, 4 }, std::sqrt(10.0f));
    searcherD.Build(pointsD.View());

    // Compare
    EXPECT_EQ(searcher.Keys().Length(), searcherD.Keys().Length());
    EXPECT_EQ(searcher.StartIndexTable().Length(),
              searcherD.StartIndexTable().Length());
    EXPECT_EQ(searcher.EndIndexTable().Length(),
              searcherD.EndIndexTable().Length());
    EXPECT_EQ(searcher.SortedIndices().Length(),
              searcherD.SortedIndices().Length());

    for (size_t i = 0; i < searcher.Keys().Length(); ++i)
    {
        uint32_t valD = searcherD.Keys()[i];
        EXPECT_EQ(searcher.Keys()[i], valD)
            << i << " of " << searcher.Keys().Length();
    }

    for (size_t i = 0; i < searcher.StartIndexTable().Length(); ++i)
    {
        uint32_t valD = searcherD.StartIndexTable()[i];
        if (valD == 0xffffffff)
        {
            EXPECT_EQ(std::numeric_limits<size_t>::max(),
                      searcher.StartIndexTable()[i])
                << i << " of " << searcher.StartIndexTable().Length();
        }
        else
        {
            EXPECT_EQ(searcher.StartIndexTable()[i], valD)
                << i << " of " << searcher.StartIndexTable().Length();
        }
    }

    for (size_t i = 0; i < searcher.EndIndexTable().Length(); ++i)
    {
        uint32_t valD = searcherD.EndIndexTable()[i];
        if (valD == 0xffffffff)
        {
            EXPECT_EQ(std::numeric_limits<size_t>::max(),
                      searcher.EndIndexTable()[i])
                << i << " of " << searcher.EndIndexTable().Length();
        }
        else
        {
            EXPECT_EQ(searcher.EndIndexTable()[i], valD)
                << i << " of " << searcher.EndIndexTable().Length();
        }
    }

    for (size_t i = 0; i < searcher.SortedIndices().Length(); ++i)
    {
        size_t valD = searcherD.SortedIndices()[i];
        EXPECT_EQ(searcher.SortedIndices()[i], valD)
            << i << " of " << searcher.SortedIndices().Length();
    }
}

TEST(CUDAPointHashGridSearcher3, ForEachNearbyPoint)
{
    CUDAArray1<float4> pointsD(3);
    pointsD[0] = make_float4(0, 1, 3, 0);
    pointsD[1] = make_float4(2, 5, 4, 0);
    pointsD[2] = make_float4(-1, 2.9f, 0, 0);

    CUDAArray1<float4> origins(1, make_float4(0, 0, 0, 0));
    CUDAArray1<int> isValid(3, 1);
    CUDAArray1<int> visited(3, 0);

    CUDAPointHashGridSearcher3 searcherD(4, 4, 4, std::sqrt(10.0f));
    searcherD.Build(pointsD.View());

    ForEachCallback func;
    func.points = pointsD.data();
    func.isValid = isValid.data();
    func.visited = visited.data();

    searcherD.ForEachNearbyPoint(origins.View(), std::sqrt(10.0f), func);

    int iv = isValid[0];
    int vd = visited[0];
    EXPECT_EQ(1, iv);
    EXPECT_EQ(1, vd);
    iv = isValid[1];
    vd = visited[1];
    EXPECT_EQ(1, iv);
    EXPECT_EQ(0, vd);
    iv = isValid[2];
    vd = visited[2];
    EXPECT_EQ(1, iv);
    EXPECT_EQ(1, vd);
}