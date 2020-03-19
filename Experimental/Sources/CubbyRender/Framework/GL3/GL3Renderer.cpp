/*************************************************************************
> File Name: GLRenderer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: abstract Renderer class implemented with Modern OpenGL
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifdef CUBBYFLOW_USE_GL

#include <Framework/GL3/GL3Renderer.h>
#include <Framework/GL3/GL3VertexBuffer.h>
#include <Framework/GL3/GL3Shader.h>
#include <Framework/GL3/GL3Renderer.h>
#include <Framework/GL3/GL3Common.h>
#include <Framework/Common.h>
#include <Core/Utils/Logging.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace CubbyFlow {
namespace CubbyRender {
    
        GL3Renderer::GL3Renderer()
        {
            //! Do nothing
        }

        GL3Renderer::~GL3Renderer()
        {
            //! Do nothing
        }

        int GL3Renderer::initializeGL()
        {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
            {
                CUBBYFLOW_ERROR << "Failed to initialize OpenGL";
                return -1;                
            }
            
            return 0;
        }

        VertexBufferPtr GL3Renderer::createVertexBuffer(MaterialPtr material, const float* vertices, size_t numberOfVertices, VertexFormat format, bool storeData) 
        {
            VertexBufferPtr vertexBuffer = std::make_shared<GL3VertexBuffer>(numberOfVertices, format);
            //! Use shared_from_this for wrap this pointer with shared_ptr 
            //! Reference : http://embeddedartistry.com/blog/2017/01/11/stdshared_ptr-and-shared_from_this/
            vertexBuffer->allocateResource(shared_from_this(), material, vertices, storeData);
            return vertexBuffer;
        }

        ShaderPtr GL3Renderer::createShaderPreset(const std::string& shaderName) 
        {
            ShaderPtr shader = std::make_shared<GL3Shader>();
            shader->load(shaderName);
            return shader;
        }

} 
}

#endif 