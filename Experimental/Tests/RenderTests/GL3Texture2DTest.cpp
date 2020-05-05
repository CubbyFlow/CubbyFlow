#include "pch.h"

#include <Framework/Application/Application.h>
#include <Framework/Window/Window.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/Utils/ImageLoader.h>
#include <GL3/Application/GL3Application.h>
#include <GL3/Utils/GL3Debugging.h>
#include <GL3/Texture/GL3Texture2D.h>
#include <Core/Array/Array1.h>

using namespace CubbyFlow;
using namespace CubbyRender;

#ifdef CUBBYFLOW_USE_GL

TEST(GL3Texture2D, Constructor)
{
	ApplicationPtr application = std::make_shared<GL3Application>();
    application->initialize();
    
    WindowPtr window = application->createMainWindow("Test Window", 600, 400);
    RendererPtr gl = window->getRenderer();

    ImageLoader loader;
    bool result = loader.loadImage(RESOURCES_DIR "textures/metal.png");
    EXPECT_EQ(true, result);

    Texture2DPtr texture = gl->createTexture2D(loader.getImageAccessor(), TextureSamplingMode::kNearest);

    EXPECT_EQ(0, CUBBYFLOW_CHECK_GLERROR());
}

#endif