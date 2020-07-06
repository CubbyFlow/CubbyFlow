/*************************************************************************
> File Name: GLRenderer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: abstract Renderer class implemented with Modern OpenGL
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Renderer/RenderOptions.h>
#include <Framework/Texture/Texture.h>
#include <Framework/Texture/Texture2D.h>
#include <Framework/Texture/Texture3D.h>
#include <Framework/Utils/Common.h>
#include <Core/Array/Array2.h>
#include <Core/Vector/Vector4.h>
#include <Core/Utils/Logging.h>

#ifdef CUBBYFLOW_USE_GL

#include <GL3/Renderer/GL3Renderer.h>
#include <GL3/Buffer/GL3Framebuffer.h>
#include <GL3/Buffer/GL3VertexBuffer.h>
#include <GL3/Buffer/GL3IndexBuffer.h>
#include <GL3/Buffer/GL3VertexArrayObject.h>
#include <GL3/Shader/GL3Shader.h>
#include <GL3/Utils/GL3Common.h>
#include <GL3/Texture/GL3Texture.h>
#include <GL3/Texture/GL3Texture2D.h>
#include <GL3/Texture/GL3Texture3D.h>
#include <GLFW/glfw3.h>

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
            default:
            {
                CUBBYFLOW_ERROR << "Unknown PrimitiveType";
                abort();
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
            default:
            {
                CUBBYFLOW_ERROR << "Unknown PrimitiveType";
                abort();
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

    ArrayAccessor1<unsigned char> GL3Renderer::getCurrentFramebuffer(Size2 size) const
    {
        constexpr size_t kNumChannels = 4;
        size_t width = size[0], height = size[1];
        size_t numElements = width * height * kNumChannels;
        static Array1<unsigned char> pixels ( numElements );

        if (pixels.size() != numElements)
            pixels.Resize(numElements);

        glReadPixels( 0, 0, static_cast<GLint>(width), static_cast<GLint>(height), GL_RGBA, GL_UNSIGNED_BYTE, &(pixels[0]));
        
        return pixels.Accessor();
    }

    InputLayoutPtr GL3Renderer::createInputLayout()
    {
        InputLayoutPtr inputLayout = std::make_shared<GL3VertexArrayObject>();
        inputLayout->initialize(shared_from_this());
        return inputLayout;
    }

    void GL3Renderer::draw(InputLayoutPtr inputLayout)
    {
        if (inputLayout->isIndexBufferAttached())
        {
            GLsizei count = static_cast<GLsizei>(inputLayout->getNumberOfIndices());
            glDrawElements(convertPrimitiveType(_primitiveType), count, GL_UNSIGNED_INT, NULL);
        }
        else
        {
            GLsizei count = static_cast<GLsizei>(inputLayout->getNumberOfVertices());
            glDrawArrays(convertPrimitiveType(_primitiveType), 0, count);
        }
    }

    int GL3Renderer::initializeGL()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
        {
            CUBBYFLOW_ERROR << "Failed to initialize OpenGL";
            return -1;                
        }
        onSetRenderState();
        return 0;
    }

    VertexBufferPtr GL3Renderer::createVertexBuffer(const ConstArrayAccessor1<float>& data, size_t numberOfVertices, VertexFormat format) 
    {
        VertexBufferPtr vertexBuffer = std::make_shared<GL3VertexBuffer>();
        vertexBuffer->setInputVertexFormat(format);
        vertexBuffer->allocateBuffer(shared_from_this(), data, numberOfVertices);
        return vertexBuffer;
    }

    IndexBufferPtr GL3Renderer::createIndexBuffer(const ConstArrayAccessor1<unsigned int>& data, size_t numberOfIndices)
    {
        IndexBufferPtr indexBuffer = std::make_shared<GL3IndexBuffer>();
        indexBuffer->allocateBuffer(shared_from_this(), data, numberOfIndices);
        return indexBuffer;
    }

    ShaderPtr GL3Renderer::createShaderPreset(const std::string& shaderName) 
    {
        ShaderPtr shader = std::make_shared<GL3Shader>();
        shader->load(shaderName);
        return shader;
    }

    FramebufferPtr GL3Renderer::createFramebuffer(Size2 viewportSize)
    {
        FramebufferPtr framebuffer = std::make_shared<GL3Framebuffer>(viewportSize);
        framebuffer->allocateBuffer(shared_from_this());
        return framebuffer;
    }

    void GL3Renderer::setViewport(int x, int y, size_t width, size_t height)
    {
        glViewport(x, y, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
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
                glDisable(GL_CULL_FACE);
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
            glFrontFace(GL_CW);
        else
            glFrontFace(GL_CCW);

        if (_renderState.isBlendEnabled) 
        {
            glEnable(GL_BLEND);
            glBlendFunc(convertBlendFactor(_renderState.sourceBlendFactor),
                        convertBlendFactor(_renderState.destinationBlendFactor));
        } 
        else 
            glDisable(GL_BLEND);

        if (_renderState.isDepthTestEnabled) 
            glEnable(GL_DEPTH_TEST);
        else 
            glDisable(GL_DEPTH_TEST);
        
        glPointSize(3.0f);

    }

    Texture2DPtr GL3Renderer::createTexture2D(const TextureParams& param, Size2 size, void* data)
    {
        Texture2DPtr texture = std::make_shared<GL3Texture2D>();
        texture->allocateTexture(shared_from_this(), size, data);
        texture->setTextureParams(param);
        return texture;
    }

    Texture3DPtr GL3Renderer::createTexture3D(const TextureParams& param, Size3 size, void* data)
    {
        Texture3DPtr texture = std::make_shared<GL3Texture3D>();
        texture->allocateTexture(shared_from_this(), size, data);
        texture->setTextureParams(param);
        return texture;
    }

} 
}

#endif 