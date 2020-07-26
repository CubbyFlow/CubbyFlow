/*************************************************************************
> File Name: Renderer.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Renderere class managing whole context and states.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_RENDERER_HPP
#define CUBBYFLOW_VOX_RENDERER_HPP

#include <memory>
#include <string>

namespace Vox {

    class App;

    namespace Renderer {
        //! Init GLFW window with information from the given app and OpenGL.
        //! Run loop to advance frame of the app.
        bool RunApp(const std::shared_ptr<App>& app);

        //! Check whether if given extensions are supported or not.
        bool CheckExtensionsSupported(const std::initializer_list<std::string>& exts);
    };

};

#endif