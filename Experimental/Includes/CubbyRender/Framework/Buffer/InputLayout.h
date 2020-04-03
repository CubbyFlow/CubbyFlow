/*************************************************************************
> File Name: InputLayout.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Input layout which store vertex buffers and index buffers.  
            Same as VAO(Vertex Array Object) in OpenGL.
> Created Time: 2020/04/01
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_INPUTLAYOUT_H
#define CUBBYFLOW_INPUTLAYOUT_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Buffer/Buffer.h>
#include <Framework/Buffer/Vertex.h>
#include <vector>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {

    //!
    //! \brief GPU Vertex shader input layout
    //!
    class InputLayout
    {
    public:
        //! Default constructor.
        InputLayout();

        //! Default destructor.
        virtual ~InputLayout();
        
        //! Get input vertex format.
        VertexFormat getDrawFormat() const;

        //! attach vertex buffer to this input layout
        void attachVertexBuffer(RendererPtr renderer, MaterialPtr material, VertexBufferPtr vertexBuffer);

        //! attach index buffer to this input layout
        void attachIndexBuffer(RendererPtr renderer, IndexBufferPtr indexBuffer);

        void bind(RendererPtr renderer);

        void unbind(RendererPtr renderer);

        void release();

        //! Initialize Input layout .
        virtual void initialize(RendererPtr renderer) = 0;

    protected:
        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) = 0;

        //! implementation of destroy method
        virtual void onRelease() = 0;

        std::vector<VertexBufferPtr> _vertexBuffers;
        IndexBufferPtr _indexBuffer = nullptr; 
        VertexFormat _drawFormat = VertexFormat::None;
    private:
    };

    using InputLayoutPtr = std::shared_ptr<InputLayout>;
} 
}

#endif