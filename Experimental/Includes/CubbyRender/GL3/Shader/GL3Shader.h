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

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Shader/Shader.h>
#include <GL3/Utils/GL3Common.h>
#include <string>
#include <unordered_map>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Shader implemeneted by Modern opengl(exactly above opengl3.3)
    //!
    class GL3Shader final : public Shader
    {
    public:
        //! Default constructor.
        GL3Shader();

        //! Construct with shader preset name (for example, shadow_shader, etc..)
        GL3Shader(const std::string& shaderName);

        //! Construct with shader shader map ( for example, fileMap["VertexShader"] = "shader/vs.glsl")
        GL3Shader(VertexFormat format, const ShaderMap& fileMap);

        //! Default destructor.
        virtual ~GL3Shader();

        //! return shader program id
        GLuint getProgramID() const;

        //! return shader attribute location id.
        //!
        //! if given parameter does not exist in _locationCache, cache it.
        //! because of communication between cpu and gpu cause bottleneck,
        //! use unordered_map for caching.
        //!
        //! \param name of the shader attribute want to know
        //! \return shader attribute location.
        GLint getAttribLocation(const std::string& name);

        //! return shader uniform location id.
        //!
        //! if given parameter does not exist in _locationCache, cache it.
        //! because of communication between cpu and gpu cause bottleneck,
        //! use unordered_map for caching.
        //!
        //! \param name of the shader uniform want to know
        //! \return shader uniform location.
        GLint getUniformLocation(const std::string& name);
    protected:
        //! implementation of shader load
        int onLoad(const ShaderMap& shaderMap) override;

        //! implementation of bind method
        void onBind(RendererPtr renderer) override;
        
        //! implementation of unbind method
        void onUnbind(RendererPtr renderer) override;

        //! implementation of destroy method
        void onDestroy() override;

        void sendParametersToGPU();

    private:
        void printShaderLog();

        std::unordered_map<std::string, GLuint> _locationCache;
        GLuint _programID = 0U;
    };

    using GL3ShaderPtr = std::shared_ptr<GL3Shader>;
} 
}

#endif

#endif 
