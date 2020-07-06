#include "pch.h"

#include <Framework/Application/Application.h>
#include <Framework/Window/Window.h>
#include <GL3/Application/GL3Application.h>
#include <GL3/Utils/GL3Debugging.h>

using namespace CubbyFlow;
using namespace CubbyRender;

#ifdef CUBBYFLOW_USE_GL

TEST(GL3Application, Constructor)
{
	ApplicationPtr application = std::make_shared<GL3Application>();
}

TEST(GL3Application, Initialize)
{
	ApplicationPtr application = std::make_shared<GL3Application>();
    EXPECT_EQ(0, application->initialize());
}

TEST(GL3Application, CreateMainWindow)
{
	ApplicationPtr application = std::make_shared<GL3Application>();
    EXPECT_EQ(0, application->initialize());
    
    TestWindowPtr window = std::make_shared<TestWindow>("TestWindow", 600, 400);
    application->setMainWindow(window);
    EXPECT_EQ(0, CUBBYFLOW_CHECK_GLERROR());
}

TEST(GL3Application, Run)
{
	ApplicationPtr application = std::make_shared<GL3Application>();
    EXPECT_EQ(0, application->initialize());
    
    TestWindowPtr window = std::make_shared<TestWindow>("TestWindow", 600, 400);
    window->setShutdownTimer(1); //! After render one frame, window will be shutdown.
    application->setMainWindow(window);
    EXPECT_EQ(0, application->run());
    EXPECT_EQ(0, CUBBYFLOW_CHECK_GLERROR());
}

#endif