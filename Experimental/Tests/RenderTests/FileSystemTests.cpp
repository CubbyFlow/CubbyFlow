#include "gtest/gtest.h"
#include <Vox/FileSystem.hpp>

#include "RenderTestsUtils.hpp"

using namespace CubbyFlow;

TEST(FileSystem, FindFile)
{
    Vox::Path path = Vox::FileSystem::FindPath("sphere.obj");
    EXPECT_EQ(false, path.IsNullPath());
}