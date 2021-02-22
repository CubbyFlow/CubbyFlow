/*************************************************************************
> File Name: Device.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Device class related to graphics device 
> Created Time: 2020/08/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_DEVICE_HPP
#define CUBBYFLOW_VOX_DEVICE_HPP

#include <Vox/Utils/FileSystem.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <memory>
#include <string>

namespace Vox {

    class App;
    class FrameContext;

    namespace Device {

        //! Initialize GLFW window configuration.
        void Initialize();

        //! Destroy GLFW window configuration.
        void Terminate();

        //! Init GLFW window with information from the given app and scene file.
        //! Run loop to advance the frame of the app.
        void RunApp(const std::shared_ptr<App>& app, const Vox::Path& scenePath);

        //! Check whether if given extensions are supported or not.
        [[nodiscard]] bool CheckExtensionsSupported(const std::initializer_list<const char*>& exts);

        //! Register callback functions to given context.
        void RegisterCallbacks(const std::shared_ptr<FrameContext>& ctx);

        //! Apply only modified render status compared two render status structure.
        void ApplyRenderStatus(const FrameContext::RenderStatus& prevStat, const FrameContext::RenderStatus& newStat);
    };
};

#endif