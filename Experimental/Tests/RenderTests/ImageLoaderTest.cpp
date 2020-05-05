#include "pch.h"

#include <Framework/Utils/ImageLoader.h>

#include "RenderTestsUtils.h"

using namespace CubbyFlow;
using namespace CubbyRender;

TEST(ImageLoader, LoadImage)
{
    ImageLoader loader;
    bool result = loader.loadImage(RESOURCES_DIR "textures/metal.png");
    EXPECT_EQ(true, result);
}
