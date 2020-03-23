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
#include <Framework/RenderOptions.h>
#include <Core/Utils/Logging.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    GLenum convertPrimitiveType(PrimitiveType _type)
    {
        GLenum mode = GL_LINE_STRIP;
        switch(_type)
        {
            case PrimitiveType::Point         :
            {
                mode = GL_POINTS;
                break;
            }
            case PrimitiveType::Line          :
            {
                mode = GL_LINES;
                break;
            }
            case PrimitiveType::LineStrip     :
            {
                mode = GL_LINE_STRIP;
                break;
            }
            case PrimitiveType::Triangle      :
            {
                mode = GL_TRIANGLES;
                break;
            }
            case PrimitiveType::TriangleStrip :
            {
                mode = GL_TRIANGLE_STRIP;
                break;
            }
        }
        return mode;
    }

    GLenum convertBlendFactor(RenderState::BlendFactor blendFactor)
    {
        GLenum factor = GL_ZERO;
        switch(blendFactor)
        {
            case RenderState::BlendFactor::Zero:
            {
                factor = GL_ZERO;
                break;
            }
            case RenderState::BlendFactor::One:
            {
                factor = GL_ONE;
                break;
            }
            case RenderState::BlendFactor::SrcColor:
            {
                factor = GL_SRC_COLOR;
                break;
            }
            case RenderState::BlendFactor::OneMinusSrcColor:
            {
                factor = GL_ONE_MINUS_SRC_COLOR;
                break;
            }
            case RenderState::BlendFactor::SrcAlpha:
            {
                factor = GL_SRC_ALPHA;
                break;
            }
            case RenderState::BlendFactor::OneMinusSrcAlpha:
            {
                factor = GL_ONE_MINUS_SRC_ALPHA;
                break;
            }
            case RenderState::BlendFactor::DestAlpha:
            {
                factor = GL_DST_ALPHA;
                break;
            }
            case RenderState::BlendFactor::OneMinusDestAlpha:
            {
                factor = GL_ONE_MINUS_DST_ALPHA;
                break;
            }
            case RenderState::BlendFactor::DestColor:
            {
                factor = GL_DST_COLOR;
                break;
            }
            case RenderState::BlendFactor::OneMinusDestColor:
            {
                factor = GL_ONE_MINUS_DST_COLOR;
                break;
            }
        }
        return factor;   
    }

    GL3Renderer::GL3Renderer()
    {
        //! Do nothing
    }

    GL3Renderer::~GL3Renderer()
    {
        //! Do nothing
    }

    void GL3Renderer::getCurrentFrame(ArrayAccessor2<Vector3B> pixels)
    {
        Size2 size = pixels.size();
        glReadPixels( 0, 0, static_cast<GLint>(size.x), static_cast<GLint>(size.y), GL_RGB, GL_UNSIGNED_BYTE, &(pixels(0, 0).x));
    }

    void GL3Renderer::draw(size_t numberOfVertices) 
    {
        if (numberOfVertices)
        {
            GLsizei count = static_cast<GLsizei>(numberOfVertices);
            glDrawArrays(convertPrimitiveType(_primitiveType), 0, count);
        }
    }

    void GL3Renderer::drawIndices(size_t numberOfElements) 
    {
        if (numberOfElements)
        {
            GLsizei count = static_cast<GLsizei>(numberOfElements);
            glDrawElements(convertPrimitiveType(_primitiveType), count, GL_UNSIGNED_INT, NULL);
        }
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

    VertexBufferPtr GL3Renderer::createVertexBuffer(MaterialPtr material, const ConstArrayAccessor1<float>& data, size_t numberOfVertices, VertexFormat format, bool storeData) 
    {
        VertexBufferPtr vertexBuffer = std::make_shared<GL3VertexBuffer>(format);
        //! Use shared_from_this for wrap this pointer with shared_ptr 
        //! Reference : http://embeddedartistry.com/blog/2017/01/11/stdshared_ptr-and-shared_from_this/
        vertexBuffer->allocateBuffer(shared_from_this(), material, data, numberOfVertices, storeData);
        return vertexBuffer;
    }

    ShaderPtr GL3Renderer::createShaderPreset(const std::string& shaderName) 
    {
        ShaderPtr shader = std::make_shared<GL3Shader>();
        shader->load(shaderName);
        return shader;
    }

    void GL3Renderer::onRenderBegin() 
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(_backgroundColor[0], _backgroundColor[1],
                     _backgroundColor[2], _backgroundColor[3]);
    }

    void GL3Renderer::onRenderEnd() 
    {
        //! Do nothing.
    }

    void GL3Renderer::onSetRenderState()
    {
        switch (_renderState.cullMode) {
            case RenderState::CullMode::None:
            {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_NONE);
                break;
            }
            case RenderState::CullMode::Front:
            {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
                break;
            }
            case RenderState::CullMode::Back:
            {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
                break;
            }
        }

        if (_renderState.isFrontFaceClockWise) 
        {
            glFrontFace(GL_CW);
        } 
        else
        {
            glFrontFace(GL_CCW);
        }

        if (_renderState.isBlendEnabled) 
        {
            glEnable(GL_BLEND);
            glBlendFunc(convertBlendFactor(_renderState.sourceBlendFactor),
                        convertBlendFactor(_renderState.destinationBlendFactor));
        } 
        else 
        {
            glDisable(GL_BLEND);
        }

        if (_renderState.isDepthTestEnabled) 
        {
            glEnable(GL_DEPTH_TEST);
        } 
        else 
        {
            glDisable(GL_DEPTH_TEST);
        }
    }
} 
}

#endif 