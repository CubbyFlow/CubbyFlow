/*************************************************************************
> File Name: App.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The application class for representating application side cpu tasks.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/App.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Camera/PerspectiveCamera.hpp>
#include <Vox/Camera/FlycamController.hpp>
#include <Vox/Scene/VoxScene.hpp>
#include <Vox/Utils/SequentialFrameCapture.hpp>
#include <Core/Utils/Macros.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace CubbyFlow;

namespace Vox {

    App::App()
    {
        //! Do nothing
    }

    App::App(const int width, const int height)
        : _windowSize(width, height)
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
        std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();

        //! Set render status of the current context with default values.
        ctx->SetRenderStatus(FrameContext::RenderStatus());
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

        //! Initialize renderer scene from file.
        _scene = Vox::VoxScene::CreateScene(scenePath);
        ctx->BindSceneToContext(_scene);

        //! Get camera instance from the scene.
        _camera = _scene->GetSceneObject<Vox::PerspectiveCamera>("mainCam");
        _camera->SetAspectRatio(static_cast<float>(_windowSize.x) / _windowSize.y);
        _camera->UpdateMatrix();

        //! Create camera controller.
        _camController = std::make_shared<Vox::FlycamController>(_camera);

        //! Initialize frame recorder.
        _frameCapture.reset(new SequentialFrameCapture());
        
        Vox::App::PushFrameContextToQueue(ctx);

        return true;
    }

    void App::UpdateFrame(double dt)
    {
        _camController->UpdateCamera(dt);
    }

    void App::BeginFrame(std::shared_ptr<FrameContext>& ctx)
    {
        UNUSED_VARIABLE(ctx);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);
    }

    void App::EndFrame(std::shared_ptr<FrameContext>& ctx)
    {
        UNUSED_VARIABLE(ctx);
        //! On screen draw stuffs like GUI
    }

    void App::SetFPSLimit(const float limit)
    {
        _fpsLimit = limit;
    }

    float App::GetFPSLimit() const
    {
        return _fpsLimit;
    }

    void App::SetWindowSize(int width, int height)
    {
        _windowSize = CubbyFlow::Point2I(width, height);
        _camera->SetAspectRatio(static_cast<float>(width) / height);
        OnSetWindowSize(width, height);
    }

    void App::SetKey(int key, int scancode, int action, int mods)
    {
        _camController->SetKey(key, scancode, action, mods);
        OnSetKey(key, scancode, action, mods);
    }

    void App::SetMouseButton(int button, int action, int mods)
    {
        _camController->SetMouseButton(button, action, mods);
        OnSetMouseButton(button, action, mods);
    }

    void App::SetMouseCursorPos(double x, double y)
    {
        _camController->SetMouseCursorPos(x, y);
        OnSetMouseCursorPos(x, y);
    }

    void App::SetMouseScroll(double deltaX, double deltaY)
    {
        _camController->SetMouseScroll(deltaX, deltaY);
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
