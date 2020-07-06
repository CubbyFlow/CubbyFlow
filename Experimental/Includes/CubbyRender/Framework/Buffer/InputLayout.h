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
#include <Core/Array/ArrayAccessor1.h>
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

        //! attach vertex buffer to this input layout.
        //! attached vertex buffers' format must not be overlapped.
        //! also number of vertices in the given buffer must matched with original ones.
        void attachVertexBuffer(RendererPtr renderer, MaterialPtr material, VertexBufferPtr vertexBuffer);
        
        //! attach index buffer to this input layout
        void attachIndexBuffer(RendererPtr renderer, IndexBufferPtr indexBuffer);

        //! update vertex buffer 
        void updateVertexBuffer(RendererPtr renderer, const ConstArrayAccessor1<float>& data, VertexFormat format);
        
        //! update index buffer;
        void updateIndexBuffer(RendererPtr renderer, const ConstArrayAccessor1<unsigned int>& data);

        //! Check index buffer is attached or not.
        bool isIndexBufferAttached() const;
    
        //! bind input layout to the renderer
        void bind(RendererPtr renderer);

        //! unbind input layout from the renderer
        void unbind(RendererPtr renderer);

        //! release resources what input layout held.
        void release();

        //! return number of vertices from vertex buffer 
        size_t getNumberOfVertices() const;

        //! return number of indices from index buffer.
        size_t getNumberOfIndices() const;

        //! The implementation of the initialize Input layout .
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