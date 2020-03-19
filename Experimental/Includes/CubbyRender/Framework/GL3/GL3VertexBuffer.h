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

#ifdef CUBBYFLOW_USE_GL

#include <Framework/VertexBuffer.h>
#include <Framework/GL3/GL3Common.h>
#include <memory>

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

        GL3VertexBuffer(size_t numberOfElements, VertexFormat format = VertexFormat::Position3);

        //! Default destructor.
        ~GL3VertexBuffer();
    protected:
        //! implementation of bind method
        void onBind(RendererPtr renderer) override;
        
        //! implementation of unbind method
        void onUnbind(RendererPtr renderer) override;

        //! implementation of destry method
        void onDestroy(RendererPtr renderer) override;

        //! Allocate gpu 
        void onAllocateResource(RendererPtr renderer, MaterialPtr material, const float* data, bool storeData) override;

        //! Update 
        void onUpdateResource(RendererPtr renderer, MaterialPtr material, const float* data, bool storeData) override;
    private:
        GLuint _vertexArrayID;
        GLuint _vertexBufferID;
    };

    using GL3VertexBufferPtr = std::shared_ptr<GL3VertexBuffer>;
} 
}

#endif

#endif 