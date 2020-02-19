/*************************************************************************
> File Name: Shader.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Shader object.
> Created Time: 2020/02/19
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_SHADER_H
#define CUBBYFLOW_SHADER_H

#include <memory>
#include <Framework/Object.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    class Renderer;
    using RendererPtr = std::shared_ptr<Renderer>;

    //!
    //! \brief Abstract base class for Shader object.
    //!
    class Shader : public Object
    {
    public:
        //! Default constructor.
        Shader();

        //! Construct with shader name (for example, shadow_shader, etc..)
        Shader(const std::string& shaderName);

        //! Default destructor.
        virtual ~Shader();

        //! Set shader name
        void setShaderName(const std::string& shaderName);

        //! Get shader name
        std::string getShaderName();
    protected:
        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) = 0;

        //! implementation of destry method
        virtual void onDestroy() = 0;

        std::string _shaderName;
    private:
    };

    using ShaderPtr = std::shared_ptr<Shader>;
} 
}

#endif