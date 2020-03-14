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

#include <memory>
#include <Core/Point/Point2.h>
#include <Core/Vector/Vector2.h>
#include <Framework/Renderer.h>
#include <string>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Abstract base class for Window.
    //!
    class Window
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
        virtual Point2I getFramebufferSize() const = 0;

        //! Returns the window size.
        virtual Point2I getWindowSize() const = 0;

        //! Returns framebuffer / window size ratio.
        virtual Vector2F displayScalingFactor() const;

        //! Request to render given number of frames to the renderer.
        virtual void requestRender(unsigned int numFrames);

        //! Sets swap interval.
        virtual void setSwapInterval(int interval);

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
        
    protected:
        RendererPtr _renderer;
        std::string _title;
        int _swapInterval = 0;
        bool _isUpdateEnabled = false;
    private:

    };

    using WindowPtr = std::shared_ptr<Window>;
} 
}

#endif