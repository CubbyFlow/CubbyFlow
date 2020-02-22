/*************************************************************************
> File Name: GL3Shader.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: shader class implemented with Modern OpenGL
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL3SHADER_H
#define CUBBYFLOW_GL3SHADER_H

#ifdef CUBBYFLOW_USE_GL

#include <Framework/Prerequisites.h>
#include <Framework/Shader.h>
#include <Framework/GL3/GL3Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Shader implemeneted by Modern opengl(exactly above opengl3.3)
    //!
    class GL3Shader final : public Shader
    {
        //! Default constructor.
        GL3Shader();

        //! Construct with shader preset name (for example, shadow_shader, etc..)
        GL3Shader(const std::string& shaderName);

        //! Construct with shader shader map ( for example, fileMap["VertexShader"] = "shader/vs.glsl")
        GL3Shader(const ShaderMap& fileMap);

        //! Default destructor.
        virtual ~GL3Shader();

    protected:
        //! implementation of shader load
        int onLoad(const ShaderMap& shaderMap) override;

        //! implementation of bind method
        void onBind(RendererPtr renderer) override;
        
        //! implementation of unbind method
        void onUnbind(RendererPtr renderer) override;

        //! implementation of destry method
        void onDestroy() override;
    private:
        void printShaderLog();

        GLuint _programID;
    };

    using GL3ShaderPtr = std::shared_ptr<GL3Shader>;
} 
}

#endif

#endif 