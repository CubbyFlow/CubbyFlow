/*************************************************************************
> File Name: GL3VertexArrayObject.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: VertexArrayObject class implemented by modern opengl
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifdef CUBBYFLOW_USE_GL

#include <GL3/Buffer/GL3VertexArrayObject.h>
#include <GL3/Renderer/GL3Renderer.h>
#include <GL3/Utils/GL3Common.h>
#include <GL3/Utils/GL3Debugging.h>
#include <GL3/Shader/GL3Shader.h>
#include <Framework/Renderable/Material.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/Utils/Common.h>
#include <glad/glad.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    GL3VertexArrayObject::GL3VertexArrayObject()
    {
        //! Do nothing
    }

    GL3VertexArrayObject::~GL3VertexArrayObject()
    {
        release();
    }

    void GL3VertexArrayObject::initialize(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
        glGenVertexArrays(1, &_vertexArrayID);
    }

    void GL3VertexArrayObject::onBind(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
        glBindVertexArray(_vertexArrayID);
    }
        
    void GL3VertexArrayObject::onUnbind(RendererPtr renderer) 
    {
        UNUSED_VARIABLE(renderer);
        glBindVertexArray(0U);
    }

    void GL3VertexArrayObject::onRelease() 
    {
        //!if (static_cast<int>(_vertexArrayID))
        //!    glDeleteVertexArrays(1, &_vertexArrayID);
    }
} 
}

#endif