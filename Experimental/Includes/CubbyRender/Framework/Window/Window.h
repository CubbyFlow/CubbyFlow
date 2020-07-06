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
#include <Core/Array/ArrayAccessor1.h>
#include <Core/Size/Size2.h>
#include <Core/Vector/Vector2.h>
#include <string>
#include <vector>
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

        //! Action implementation when window is resized.
        virtual void onWindowResized(int width, int height) = 0;

        //! Action implementation when any key is pressed or released
        virtual void onKey(int key, int scancode, int action, int mods) = 0;

        //! Action implementation when any mouse button is pressed or released.
        virtual void onMouseButton(int button, int action, int mods) = 0;

        //! Action implementation when cursor is moved
        virtual void onMouseCursorPos(double x, double y) = 0;

        //! Action implementation when mouse scroll is moved
        virtual void onMouseScroll(double deltaX, double deltaY) = 0;

        //! Returns framebuffer / window size ratio.
        Vector2F displayScalingFactor() const;

        //! Sets swap interval.
        virtual void setSwapInterval(int interval);

        //! Returns renderer.
        const RendererPtr& getRenderer() const;

        //! Get Renderer pointer
        RendererPtr getRenderer();

        //! Returns true if update is enabled.
        bool isUpdateEnabled() const;

        //! Enables/disables update.
        void setIsUpdateEnabled(bool enabled);

        //! Get title of the window.
        std::string getTitle();

        //! Set renderer pointer
        void setRenderer(RendererPtr renderer);
        
        //! Set viewport of the current window
        //! \param x left bottom x position
        //! \param y left bottom y position
        //! \param width width of the viewport
        //! \param height height of the viewport.
        void setViewport(int x, int y, size_t width, size_t height);
        
        //! call render function through renderer.
        void renderScene();

        //! update window
        void updateScene();

        //! Return Number of requested render frames.
        unsigned int getNumRequestedRenderFrames() const;

        //! Return reference of the number of requested render frames variable.
        unsigned int& getNumRequestedRenderFrames();

        //! Return current screen pixel array.
        ArrayAccessor1<unsigned char> getCurrentScreen(Size2 size) const;

    protected:
        //! Implementation of the window update;
        virtual void onUpdateScene() = 0;     

        //! Implementation of the render;
        virtual void onRenderScene() = 0;     

        std::vector<DockerPtr> _dockers;
        RendererPtr _renderer;
        std::string _title;
        Size2 _windowSize;
        int _swapInterval = 0;
        bool _isUpdateEnabled = false;
    private:
    };

} 
}

#endif
