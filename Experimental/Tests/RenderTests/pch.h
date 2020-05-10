//
// pch.h
// Header for standard system include files.
//
#pragma once

#include "gtest/gtest.h"
#include <GL3/Window/GL3Window.h>
#include <memory>

using namespace CubbyFlow;
using namespace CubbyRender;

class TestWindow final : public GL3Window
{
    using super_t = GL3Window;
public: 
    //! Default Constructor.
    TestWindow(const std::string& title, int width, int height);
    //! Default Destructor.
    ~TestWindow();
    
    //! Action implementation when window is resized.
    void onWindowResized(int width, int height) override;
    //! Action implementation when any key is pressed or released
    void onKey(int key, int scancode, int action, int mods) override;
    //! Action implementation when any mouse button is pressed or released.
    void onMouseButton(int button, int action, int mods) override;
    //! Action implementation when cursor is moved
    void onMouseCursorPos(double x, double y) override;
    //! Action implementation when mouse scroll is moved
    void onMouseScroll(double deltaX, double deltaY) override;
protected:
    void onRenderScene() override;
    void onUpdateScene() override;
private:
};
using TestWindowPtr = std::shared_ptr<TestWindow>;