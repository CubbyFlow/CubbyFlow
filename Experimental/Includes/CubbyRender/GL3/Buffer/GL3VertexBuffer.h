/*************************************************************************
> File Name: GL3VertexBuffer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: VertexBuffer class implemented with Modern OpenGL
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL3VERTEXBUFFER_H
#define CUBBYFLOW_GL3VERTEXBUFFER_H

#include <Framework/Buffer/VertexBuffer.h>
#include <memory>

#ifdef CUBBYFLOW_USE_GL

#include <GL3/Utils/GL3Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief VertexBuffer implemeneted by Modern opengl(exactly above opengl3.3)
    //!
    class GL3VertexBuffer final : public VertexBuffer
    {
    public:
        //! Default constructor.
        GL3VertexBuffer();

        //! Default destructor.
        virtual ~GL3VertexBuffer();

        //! Update the buffer contents
        void updateBuffer(RendererPtr renderer, const ConstArrayAccessor1<float>& data) override;

    protected:
        //! implementation of bind method
        void onBind(RendererPtr renderer) override;
        
        //! implementation of unbind method
        void onUnbind(RendererPtr renderer) override;

        //! implementation of destroy method
        void onDestroy() override;

        //! Allocate gpu 
        void onAllocateBuffer(RendererPtr renderer, const ConstArrayAccessor1<float>& data) override;

        //! implementation of bind state method.
        void onBindState(RendererPtr renderer, MaterialPtr material) override;

    private:
        GLuint _vertexBufferID = 0U;
    };

    using GL3VertexBufferPtr = std::shared_ptr<GL3VertexBuffer>;
} 
}

#endif

#endif 