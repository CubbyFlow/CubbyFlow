#include "gtest/gtest.h"
#include <Vox/GeometryCache.hpp>

#include "RenderTestsUtils.hpp"

using namespace CubbyFlow;

TEST(GeometryCache, LoadObj)
{
    Vox::GeometryCache cache;
    cache.LoadCache(RESOURCES_DIR "sphere.obj", 0);
}