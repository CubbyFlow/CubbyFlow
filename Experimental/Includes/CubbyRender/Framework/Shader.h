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

#include <Framework/Prerequisites.h>
#include <Framework/Object.h>
#include <Framework/Vertex.h>
#include <unordered_map>
#include <string>

namespace CubbyFlow {
namespace CubbyRender {

    using ShaderMap = std::unordered_map<std::string, std::string>;

    //!
    //! \brief Abstract base class for Shader object.
    //!
    class Shader : public Object
    {
    public:
        //! Default constructor.
        Shader();

        //! Default destructor.
        virtual ~Shader();

        //! load shader with shader preset name.
        int load(const std::string& shaderName);

        //! load shader with shader map.
        int load(const ShaderMap& fileMap);

        VertexFormat getInputVertexFormat() const;

    protected:
        //! implementation of shader load
        virtual int onLoad(const ShaderMap& shaderMap) = 0;

        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) = 0;

        //! implementation of destry method
        virtual void onDestroy(RendererPtr renderer) = 0;

        //! Input format of the vertex shader.
        VertexFormat _format = VertexFormat::Position3Normal3;
    private:
    };

    using ShaderPtr = std::shared_ptr<Shader>;
} 
}

#endif