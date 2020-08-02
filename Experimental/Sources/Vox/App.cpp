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
#include <glad/glad.h>

using namespace CubbyFlow;

namespace Vox {

    App::App()
    {
    }

    App::~App()
    {
    }

    CubbyFlow::Vector2I App::GetWindowSize()
    {
        return _windowSize;
    }
    
    bool App::Initialize()
    {
        _camera.SetPerspectiveElements(45.0f, static_cast<float>(_windowSize.x) / _windowSize.y, 0.01f, 10000.0f);
        _camera.SetViewElements(CubbyFlow::Vector3F(0.0f, 0.0f, 0.0f), CubbyFlow::Vector3F(0.0f, 0.0f, 1.0f));
        _camera.UpdateMatrix();

        return true;
    }

    void App::BeginFrame(std::shared_ptr<FrameContext>& ctx)
    {
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);
    }

    void App::EndFrame(std::shared_ptr<FrameContext>& ctx)
    {
        //! On screen draw stuffs like GUI
    }

    void App::SetWindowSize(Vector2I size)
    {
        _windowSize = size;
        _camera.SetPerspectiveElements(45.0f, static_cast<float>(_windowSize.x) / _windowSize.y, 0.01f, 10000.0f);
        _camera.SetViewElements(CubbyFlow::Vector3F(0.0f, 0.0f, 0.0f), CubbyFlow::Vector3F(0.0f, 0.0f, 1.0f));
        _camera.UpdateMatrix();
    }

    void App::SetBackgroundColor(Vector4F color)  
    {
        _bgColor = color;
    }
};
