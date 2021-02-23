#include "gtest/gtest.h"
#include <Vox/Utils/FileSystem.hpp>
#include <Vox/Scene/GeometryCache.hpp>

#include "RenderTestsUtils.hpp"

using namespace CubbyFlow;

TEST(GeometryCache, LoadObj)
{
    Vox::GeometryCache cache;
    Vox::Path path = Vox::FileSystem::FindPath("sphere.obj");
    cache.LoadCache(path, 0);
}