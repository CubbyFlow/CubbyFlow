#include "pch.h"

#include <Framework/Application/Application.h>
#include <Framework/Window/Window.h>
#include <Framework/Buffer/VertexBuffer.h>
#include <Framework/Buffer/Vertex.h>
#include <Framework/Renderer/Renderer.h>
#include <GL3/Application/GL3Application.h>
#include <GL3/Buffer/GL3VertexBuffer.h>
#include <GL3/Utils/GL3Debugging.h>
#include <Core/Array/Array1.h>

using namespace CubbyFlow;
using namespace CubbyRender;

#ifdef CUBBYFLOW_USE_GL

TEST(GL3IndexBuffer, Constructor)
{
	ApplicationPtr application = std::make_shared<GL3Application>();
    int result = application->initialize();
    
    WindowPtr window = application->createMainWindow("Test Window", 600, 400);
    RendererPtr gl = window->getRenderer();
    
    Array1<unsigned int> indices = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    gl->createIndexBuffer(indices.ConstAccessor(), 6);
    EXPECT_EQ(0, CUBBYFLOW_CHECK_GLERROR());
}

#endif