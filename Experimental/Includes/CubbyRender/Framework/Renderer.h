/*************************************************************************
> File Name: Renderer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Renderer object.
> Created Time: 2020/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_RENDERER_H
#define CUBBYFLOW_RENDERER_H

#include <Framework/Prerequisites.h>
#include <string>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Abstract base class for Renderer object.
    //!
    //! This class contains basic interface for renderer. 
    //! The wrapper class of each Graphics API must overrides 
    //! this renderer class.
    //!
    class Renderer
    {
    public:
        //! Default constructor.
        Renderer();

        //! Default destructor.
        virtual ~Renderer();

        //! Initialize and fetch gl commands.
        virtual int initializeGL() = 0;

        //! Create VertexBuffer pointer with given parameters.
        //!
        //! \param vertices vertex data
        //! \param numberOfVertices number of point.
        //! \return new vertex buffer instance
        virtual VertexBufferPtr createVertexBuffer(const float* vertices, size_t numberOfVertices) = 0;

        //! Create Shader Program from presets.
        //! \param shader preset name
        //! \return new shader pointer
        virtual ShaderPtr createShaderPreset(const std::string& shaderName) = 0;

        //! CreateShader
    protected:
    private:
    };

    using RendererPtr = std::shared_ptr<Renderer>;
} 
}

#endif