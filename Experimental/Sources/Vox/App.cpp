/*************************************************************************
> File Name: App.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The application class for representating application side cpu tasks.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/App.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/PerspectiveCamera.hpp>
#include <Vox/CameraController.hpp>
#include <Vox/VoxScene.hpp>
#include <Vox/SequentialFrameCapture.hpp>
#include <Core/Utils/Macros.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace CubbyFlow;

namespace Vox {

    App::App()
    {
        //! Do nothing
    }

    App::~App()
    {
        //! Remove All Contained Contexts 
        while (!_ctxQueue.empty())
        {
            _ctxQueue.pop();
        }
    }

    CubbyFlow::Point2I App::GetWindowSize()
    {
        return _windowSize;
    }
    
    bool App::Initialize(const Vox::Path& scenePath)
    {
        _scene = Vox::VoxScene::CreateScene(scenePath);
        _camera = _scene->GetSceneObject<Vox::PerspectiveCamera>("Camera");
        _camera->SetAspectRatio(static_cast<float>(_windowSize.x) / _windowSize.y);
        _camera->UpdateMatrix();

        _camController = std::make_shared<Vox::CameraController>(_camera);

        _frameCapture.reset(new SequentialFrameCapture());

        return true;
    }

    void App::BeginFrame(std::shared_ptr<FrameContext>& ctx)
    {
        UNUSED_VARIABLE(ctx);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);
    }

    void App::EndFrame(std::shared_ptr<FrameContext>& ctx)
    {
        UNUSED_VARIABLE(ctx);
        //! On screen draw stuffs like GUI
    }

    void App::SetWindowSize(int width, int height)
    {
        _windowSize = CubbyFlow::Point2I(width, height);
        _camera->SetAspectRatio(static_cast<float>(width) / height);
        OnSetWindowSize(width, height);
    }

    void App::SetKey(int key, int scancode, int action, int mods)
    {
        OnSetKey(key, scancode, action, mods);
    }

    void App::SetMouseButton(int button, int action, int mods)
    {
        _camController->SetMouseButton(button, action);
        OnSetMouseButton(button, action, mods);
    }

    void App::SetMouseCursorPos(double x, double y)
    {
        _camController->SetMouseCursorPos(x, y);
        OnSetMouseCursorPos(x, y);
    }

    void App::SetMouseScroll(double deltaX, double deltaY)
    {
        OnSetMouseScroll(deltaX, deltaY);
    }

    void App::SetBackgroundColor(Vector4F color)  
    {
        _bgColor = color;
    }

    void App::OnSetWindowSize(int width, int height)
    {
        UNUSED_VARIABLE(width);
        UNUSED_VARIABLE(height);
    }

    void App::OnSetKey(int key, int scancode, int action, int mods)
    {
        UNUSED_VARIABLE(scancode);
        UNUSED_VARIABLE(mods);

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            while (_ctxQueue.empty() == false)
            {
                auto ctx = _ctxQueue.front();
                _ctxQueue.pop();
                ctx->SetWindowContextShouldClose(GLFW_TRUE);
                ctx.reset();
            }
        }
    }

    void App::OnSetMouseButton(int button, int action, int mods)
    {
        UNUSED_VARIABLE(button);
        UNUSED_VARIABLE(action);
        UNUSED_VARIABLE(mods);
    }

    void App::OnSetMouseCursorPos(double x, double y)
    {
        UNUSED_VARIABLE(x);
        UNUSED_VARIABLE(y);
    }

    void App::OnSetMouseScroll(double deltaX, double deltaY)
    {
        UNUSED_VARIABLE(deltaX);
        UNUSED_VARIABLE(deltaY);
    }

};
