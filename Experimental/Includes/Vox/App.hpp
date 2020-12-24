/*************************************************************************
> File Name: App.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The application class for representating application side cpu tasks.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_APP_HPP
#define CUBBYFLOW_VOX_APP_HPP

#include <Vox/FileSystem.hpp>
#include <Core/Geometry/Point2.hpp>
#include <Core/Vector/Vector4.hpp>
#include <memory>
#include <queue>

namespace Vox {

    class FrameContext;
    class VoxScene;
    class PerspectiveCamera;
    class CameraController;
    class SequentialFrameCapture;
    
    /**
     *  CPU side tasks collection wrapper class. 
     */
    class App
    {
    public:
        //! Default Constructor.
        App();
        //! Default Destructor.
        virtual ~App();

        //! Return the window screen size.
        CubbyFlow::Point2I GetWindowSize();
        //! Initialize the base application.
        virtual bool Initialize(const Vox::Path& scenePath);
        //! Update the physical step of the frame.
        virtual void UpdateFrame() {};
        //! OpenGL Commands or other tasks collection for 
        //! preparing call DrawFrame.
        void BeginFrame(std::shared_ptr<FrameContext>& ctx);
        //! Actual Drawing Call
        virtual void DrawFrame() {};
        //! End tasks after drawing call.
        void EndFrame(std::shared_ptr<FrameContext>& ctx);

        //! Return window title.
        virtual const char* GetWindowTitle() { return nullptr; };

        //! Pop frame context from top of the stack.
        std::shared_ptr<Vox::FrameContext> PopFrameContextFromQueue()
        {
            if (_ctxQueue.empty())
            {
                return nullptr;
            }
            else
            {
                const auto& ctx = _ctxQueue.front();
                _ctxQueue.pop();
                return ctx;
            }
        }
        //! Push frame context to the stack.
        inline void PushFrameContextToQueue(std::shared_ptr<Vox::FrameContext> ctx)
        {
            _ctxQueue.push(ctx);
        }

        //! Set Window screen size.
        void SetWindowSize(int width, int height);
        void SetKey(int key, int scancode, int action, int mods);
        void SetMouseButton(int button, int action, int mods);
        void SetMouseCursorPos(double x, double y);
        void SetMouseScroll(double deltaX, double deltaY);
        //! Set screen background color
        void SetBackgroundColor(CubbyFlow::Vector4F color);
    protected:
        virtual void OnSetWindowSize(int width, int height);
        virtual void OnSetKey(int key, int scancode, int action, int mods);
        virtual void OnSetMouseButton(int button, int action, int mods);
        virtual void OnSetMouseCursorPos(double x, double y);
        virtual void OnSetMouseScroll(double deltaX, double deltaY);

        std::shared_ptr<Vox::VoxScene> _scene;
        std::shared_ptr<Vox::PerspectiveCamera> _camera;
        std::shared_ptr<Vox::CameraController> _camController;
        std::unique_ptr<SequentialFrameCapture> _frameCapture;
        CubbyFlow::Point2I _windowSize { 1200, 900 }; //! window size
        CubbyFlow::Vector4F _bgColor { 0.344f, 0.388f, 0.388f, 1.0f}; //! background color
    private:
        std::queue<std::shared_ptr<FrameContext>> _ctxQueue;
    };

};

#endif