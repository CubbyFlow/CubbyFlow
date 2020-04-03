/*************************************************************************
> File Name: GL3IndexBuffer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: VertexBuffer class implemented with Modern OpenGL
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL3INDEXBUFFER_H
#define CUBBYFLOW_GL3INDEXBUFFER_H

#ifdef CUBBYFLOW_USE_GL

#include <Framework/Buffer/IndexBuffer.h>
#include <GL3/Utils/GL3Common.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief VertexBuffer implemeneted by Modern opengl(exactly above opengl3.3)
    //!
    class GL3IndexBuffer final : public IndexBuffer
    {
    public:
        //! Default constructor.
        GL3IndexBuffer();

        //! Default destructor.
        ~GL3IndexBuffer();

        //! Update 
        void updateBuffer(RendererPtr renderer, const ConstArrayAccessor1<unsigned int>& data) override;

    protected:
        //! implementation of bind method
        void onBind(RendererPtr renderer) override;
        
        //! implementation of unbind method
        void onUnbind(RendererPtr renderer) override;

        //! implementation of destry method
        void onDestroy() override;

        //! Allocate gpu 
        void onAllocateBuffer(RendererPtr renderer, const ConstArrayAccessor1<unsigned int>& data) override;

        void onBindState(RendererPtr renderer) override;

    private:
        GLuint _indexBufferID;
    };

    using GL3IndexBufferPtr = std::shared_ptr<GL3IndexBuffer>;
} 
}

#endif

#endif 