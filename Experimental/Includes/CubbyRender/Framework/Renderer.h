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
#include <Framework/Vertex.h>
#include <Framework/RenderOptions.h>
#include <Core/Array/ArrayAccessor1.h>
#include <Core/Array/ArrayAccessor2.h>
#include <Core/Vector/Vector4.h>
#include <Core/Size/Size2.h>
#include <vector>
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
    class Renderer : public std::enable_shared_from_this<Renderer>
    {
    public:
        //! Default constructor.
        Renderer();

        //! Default destructor.
        virtual ~Renderer();

        //! Initialize and fetch gl commands.
        virtual int initializeGL() = 0;

        void render();

        virtual void draw(size_t numberOfVertices) = 0;

        virtual void drawIndices(size_t numberOfElements) = 0;

        void setRenderState(const RenderState& renderState);

        void setPrimitiveType(PrimitiveType type);

        void addRenderable(RenderablePtr renderable);

        void clearRenderables();

        void setBackgroundColor(Vector4F color);

        virtual ConstArrayAccessor2<Vector4UB> getCurrentFrame(Size2 size) = 0;

        //! Create VertexBuffer pointer with given parameters.
        //!
        //! \param vertices vertex data
        //! \param numberOfVertices number of point.
        //! \return new vertex buffer instance
        virtual VertexBufferPtr createVertexBuffer(MaterialPtr material, const ConstArrayAccessor1<float>& data, size_t numberOfVertices, VertexFormat format, bool storeData) = 0;

        //! Create Shader Program from presets.
        //! \param shader preset name
        //! \return new shader pointer
        virtual ShaderPtr createShaderPreset(const std::string& shaderName) = 0;

        //! CreateShader
    protected:
        virtual void onRenderBegin() = 0;
        virtual void onRenderEnd() = 0;
        virtual void onSetRenderState() = 0;

        std::vector<RenderablePtr> _renderables;
        RenderState _renderState;
        PrimitiveType _primitiveType;
        Vector4F _backgroundColor = Vector4F(1.0f, 0.0f, 0.0f, 1.0f);
    private:
    };

    using RendererPtr = std::shared_ptr<Renderer>;
} 
}

#endif