/*************************************************************************
> File Name: ScreenRenderable.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Renderable object for rendering framebuffer texture to screeen quad.
> Created Time: 2020/05/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Renderable/ScreenRenderable.h>
#include <Framework/Renderable/Material.h>
#include <Framework/Buffer/VertexBuffer.h>
#include <Framework/Buffer/IndexBuffer.h>
#include <Framework/Buffer/InputLayout.h>
#include <Framework/Buffer/Vertex.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/Shader/Shader.h>
#include <Framework/Utils/Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    ScreenRenderable::ScreenRenderable()
    {
        //! Do nothing.
    }

    ScreenRenderable::~ScreenRenderable()
    {
        release();
    }

    void ScreenRenderable::attachScreenTexture(Texture2DPtr texture)
    {
        _screenTexture = texture;
    }

    void ScreenRenderable::onInitializeResource(RendererPtr renderer)
    {
        //! allocate and initialize material instance.
        if (_material == nullptr)
        {
            _material = renderer->createMaterial();
            ShaderPtr shader = renderer->createShaderPreset("screen_shader");
            _material->setShader(shader);
            _material->addTexture(0U, _screenTexture);
        }
        
        //! allocate and initialize input layout instance.
        if (_inputLayout == nullptr)
        {
            _inputLayout = renderer->createInputLayout();
            //! create vertex buffer from the positions and colors.
            Array1<float> vertices {
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f
            };
            VertexBufferPtr buffer = renderer->createVertexBuffer(vertices.ConstAccessor(), 
                                                                  static_cast<size_t>(vertices.size() / size_t(5)),  
                                                                  VertexFormat::Position3TexCoord2);
            _inputLayout->attachVertexBuffer(renderer, _material, buffer);
        }

        //! Set the primitive type as Point.
        _primitiveType = PrimitiveType::TriangleStrip;
    }

    void ScreenRenderable::onRender(RendererPtr renderer)
    {
        renderer->bindMaterial(_material);
        renderer->bindInputLayout(_inputLayout);
        renderer->setPrimitiveType(_primitiveType);
        renderer->draw(_inputLayout);
        renderer->unbindInputLayout(_inputLayout);
        renderer->unbindMaterial(_material);
    }

    void ScreenRenderable::onRelease()
    {
        //! Do nothing.
    }

} 
}