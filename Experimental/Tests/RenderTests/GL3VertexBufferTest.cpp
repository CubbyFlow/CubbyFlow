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

TEST(GL3VertexBuffer, Constructor)
{
	ApplicationPtr application = std::make_shared<GL3Application>();
    EXPECT_EQ(0, application->initialize());
    
    TestWindowPtr window = std::make_shared<TestWindow>("TestWindow", 600, 400);
    application->setMainWindow(window);
    RendererPtr gl = application->getMainWindow()->getRenderer();

    Array1<float> vertices = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    gl->createVertexBuffer(vertices.ConstAccessor(), 4, VertexFormat::Position3);
    EXPECT_EQ(0, CUBBYFLOW_CHECK_GLERROR());
}

#endif