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

#include <Core/Size/Size2.h>
#include <Core/Vector/Vector4.h>
#include <memory>

namespace Vox {

    class FrameContext;

    /**
     *  CPU side tasks collection wrapper class. 
     */
    class App
    {
    public:
        App();
        virtual ~App();

        //! Return the window screen size.
        CubbyFlow::Size2 GetWindowSize();

        //! Initialize the base application.
        virtual bool Initialize();
        //! OpenGL Commands or other tasks collection for 
        //! preparing call DrawFrame.
        void BeginFrame(std::shared_ptr<FrameContext>& ctx);
        //! Actual Drawing Call
        virtual void DrawFrame() = 0;
        //! End tasks after drawing call.
        void EndFrame(std::shared_ptr<FrameContext>& ctx);

        virtual const char* GetWindowTitle() = 0;

        //! Set Window screen size.
        void SetWindowSize(CubbyFlow::Size2 size);
        //! Set screen background color
        void SetBackgroundColor(CubbyFlow::Vector4F color);

        const std::shared_ptr<FrameContext>& GetCurrentContext();
    protected:
        std::shared_ptr<FrameContext> _ctx;

        CubbyFlow::Size2 _wndSize    { 1200, 900 }; //! window size
        CubbyFlow::Vector4F _bgColor { 0.2f, 0.6f, 0.2f, 1.0f}; //! background color
    private:
    };

};

#endif