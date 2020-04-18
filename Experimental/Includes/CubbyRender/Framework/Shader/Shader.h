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

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Buffer/Vertex.h>
#include <Framework/Shader/ShaderParameters.h>
#include <unordered_map>
#include <string>

namespace CubbyFlow {
namespace CubbyRender {

    using ShaderMap = std::unordered_map<std::string, std::string>;

    //!
    //! \brief Abstract base class for Shader object.
    //!
    class Shader
    {
    public:
        //! Default constructor.
        Shader();

        //! Default destructor.
        virtual ~Shader();

        //! load shader with shader preset name.
        int load(const std::string& shaderName);

        //! load shader with shader map.
        int load(VertexFormat format, const ShaderMap& fileMap);

        //! get a format of the vertex buffer in the program.
        VertexFormat getInputVertexFormat() const;

        //! get immutable shader parameters which will be sent to program.
        const ShaderParameters& getParameters() const;

        //! get mutable shader parameters which will be send to program.
        ShaderParameters& getParameters();

        //! set shader parameters which will be sent to program.
        void setParameters(const ShaderParameters& parameters);

        void bind(RendererPtr renderer);

        void unbind(RendererPtr renderer);

        void destroy();

    protected:
        //! implementation of shader load
        virtual int onLoad(const ShaderMap& shaderMap) = 0;

        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) = 0;

        //! implementation of destry method
        virtual void onDestroy() = 0;

        //! Input format of the vertex shader.
        VertexFormat _format = VertexFormat::Position3Normal3;
        ShaderParameters _parameters;
    private:
    };

    using ShaderPtr = std::shared_ptr<Shader>;
} 
}

#endif