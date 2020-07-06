//
// pch.cpp
// Include the standard header and generate the precompiled header.
//

#include "pch.h"
#include <Framework/Utils/Common.h>

#ifdef CUBBYFLOW_USE_GL
#include <GLFW/glfw3.h>

TestWindow::TestWindow(const std::string& title, int width, int height)
    : GL3Window(title, width, height)
{
    //! Do nothing.
}

TestWindow::~TestWindow()
{
    //! Do nothing.
}

void TestWindow::setShutdownTimer(int numFrames)
{
    _shutdownTimer = numFrames;
}

void TestWindow::onWindowResized(int width, int height)
{
    UNUSED_VARIABLE(width);
    UNUSED_VARIABLE(height);
}

void TestWindow::onKey(int key, int scancode, int action, int mods)
{
    UNUSED_VARIABLE(key);
    UNUSED_VARIABLE(scancode);
    UNUSED_VARIABLE(action);
    UNUSED_VARIABLE(mods);
}

void TestWindow::onMouseButton(int button, int action, int mods)
{
    UNUSED_VARIABLE(button);
    UNUSED_VARIABLE(action);
    UNUSED_VARIABLE(mods);
}

void TestWindow::onMouseCursorPos(double x, double y)
{
    UNUSED_VARIABLE(x);
    UNUSED_VARIABLE(y);
}

void TestWindow::onMouseScroll(double deltaX, double deltaY)
{
    UNUSED_VARIABLE(deltaX);
    UNUSED_VARIABLE(deltaY);
}

void TestWindow::onRenderScene()
{
    if (_shutdownTimer == 0)     glfwSetWindowShouldClose(_glfwWindow, GLFW_TRUE);
    else if (_shutdownTimer > 0) --_shutdownTimer;
}

void TestWindow::onUpdateScene()
{
    //! Do nothing.
}

#endif