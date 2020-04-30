/*************************************************************************
> File Name: GL3VertexArrayObject.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Input layout which store vertex buffers and index buffers.  
            Same as VAO(Vertex Array Object) in OpenGL.
> Created Time: 2020/04/01
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL3VERTEXARRAYOBJECT_H
#define CUBBYFLOW_GL3VERTEXARRAYOBJECT_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Buffer/InputLayout.h>
#include <Framework/Buffer/Vertex.h>
#include <vector>
#include <memory>

#ifdef CUBBYFLOW_USE_GL

#include <GL3/Utils/GL3Common.h>

namespace CubbyFlow {
namespace CubbyRender {

    //!
    //! \brief GPU Vertex shader input layout
    //!
    class GL3VertexArrayObject : public InputLayout
    {
    public:
        //! Default constructor.
        GL3VertexArrayObject();

        //! Default destructor.
        virtual ~GL3VertexArrayObject();

        //! Initialize the input layout.
        void initialize(RendererPtr renderer) override;

    protected:
        //! implementation of bind method
        void onBind(RendererPtr renderer) override;
        
        //! implementation of unbind method
        void onUnbind(RendererPtr renderer) override;

        //! implementation of destroy method
        void onRelease() override;

    private:
        GLuint _vertexArrayID;
    };

    using GL3VertexArrayObjectPtr = std::shared_ptr<GL3VertexArrayObject>;
} 
}

#endif
#endif