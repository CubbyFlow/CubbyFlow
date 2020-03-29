/*************************************************************************
> File Name: Window.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Window.
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_WINDOW_H
#define CUBBYFLOW_WINDOW_H

#include <Framework/Utils/Prerequisites.h>
#include <Core/Size/Size2.h>
#include <Core/Vector/Vector2.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    class Window;
    using WindowPtr = std::shared_ptr<Window>;

    //!
    //! \brief Abstract base class for Window.
    //!
    class Window : public std::enable_shared_from_this<Window>
    {
    public: 
        using UpdateClosure = std::function<void(WindowPtr)>;
        //! Default Constructor.
        Window();

        //! Construct with title and window size.
        Window(const std::string& title, int width, int height);

        //! Default Destructor.
        virtual ~Window();

        //! Returns the framebuffer size.
        //! Note that the framebuffer size can be different from the window size,
        //! especially on a Retina display (2x the window size).
        virtual Size2 getFramebufferSize() const = 0;

        //! Returns the window size.
        virtual Size2 getWindowSize() const = 0;

        //! Returns framebuffer / window size ratio.
        virtual Vector2F displayScalingFactor() const;

        //! Request to render given number of frames to the renderer.
        virtual void requestRender(unsigned int numFrames);

        //! Sets swap interval.
        virtual void setSwapInterval(int interval);

        //! Action implementation when window is resized.
        virtual void onWindowResized(int width, int height) = 0;

        //! Action implementation when window is moved.
        virtual void onWindowMoved(int width, int height) = 0;

        //! Action implementation when any event is happend on "KEY"
        virtual void onKey(int key, int scancode, int action, int mods) = 0;

        //! Returns renderer.
        const RendererPtr& renderer() const;

        //! Returns true if update is enabled.
        bool isUpdateEnabled() const;

        //! Enables/disables update.
        void setIsUpdateEnabled(bool enabled);

        //! Get title of the window.
        std::string getTitle();

        //! Set renderer pointer
        void setRenderer(RendererPtr renderer);
        
        //! Get Renderer pointer
        RendererPtr getRenderer();

        //! Add update closure
        void addUpdateClosure(const UpdateClosure& closure);

        //! Update window (this routine include scene update, view update, etc..)
        void update();

        //! call render function through renderer.
        void render();
    protected:
        //! Implementation of the update routine.
        virtual void onUpdate() = 0;

        RendererPtr _renderer;
        std::vector<UpdateClosure> _updateClosures;
        std::string _title;
        Size2 _windowSize;
        int _swapInterval = 0;
        bool _isUpdateEnabled = false;

    private:

    };

} 
}

#endif
