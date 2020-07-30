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

#include <Vox/FileSystem.hpp>
#include <Vox/GLTypes.hpp>
#include <memory>
#include <string>

namespace Vox {

    class App;

    namespace Renderer {
        //! Init GLFW window with information from the given app and OpenGL.
        //! Run loop to advance frame of the app.
        void RunApp(const std::shared_ptr<App>& app);

        //! Check whether if given extensions are supported or not.
        bool CheckExtensionsSupported(const std::initializer_list<const char*>& exts);

        //! Create Texture from given image path.
        GLuint CreateTextureFromFile(const Path& path, bool srgb = false);

        //! Create Render Buffer
        GLuint CreateRenderBuffer(GLenum internalFormat, GLsizei width, GLsizei height);

        //! Create Shader from glsl source data.
        GLuint CreateShaderFromSource(const char* source, GLenum shaderType);

        //! Create Shader with glsl shader file.
        GLuint CreateShaderFromFile(const Path& path, GLenum shaderType);

        //! Create Program with pre-compiled shaders.
        GLuint CreateProgram(GLuint vs, GLuint gs, GLuint fs);
    };

};

#endif