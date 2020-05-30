/*************************************************************************
> File Name: GL3Renderer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Renderer class implemented with Modern OpenGL
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GLRENDERER_H
#define CUBBYFLOW_GLRENDERER_H

#include <Framework/Renderer/Renderer.h>
#include <string>
#include <memory>

#ifdef CUBBYFLOW_USE_GL
#include <GL3/Utils/GL3Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Renderer interface implemeneted by Modern opengl(exactly above opengl3.3)
    //!
    class GL3Renderer final : public Renderer
    {
    public:
        //! Default constructor.
        GL3Renderer();

        //! Default destructor.
        ~GL3Renderer();

        //! let inputlayout draw it's vertices using this renderer instance.
        void draw(InputLayoutPtr inputLayout) override;

        //! Initialize and fetch gl commands.
        int initializeGL() override;

        //! get current frame image as array of floating point data.
        ArrayAccessor1<unsigned char> getCurrentFramebuffer(Size2 size) const override;

        //! Create Inputlayout pointer with default constructor.
        InputLayoutPtr createInputLayout() override;

        //! Create VertexBuffer pointer with given parameters.
        //!
        //! \param inputLayout Input Layout instance which will contain generated vertex buffer.
        //! \param material material which contains shader 
        //! \param data vertices data.
        //! \param numberOfVertices number of vertex in the data.
        //! \param format format of the input vertex
        //! \return new vertex buffer instance
        VertexBufferPtr createVertexBuffer(const ConstArrayAccessor1<float>& data, size_t numberOfVertices, VertexFormat format) override;

        //! Create VertexBuffer pointer with given parameters.
        //!
        //! \param inputLayout Input Layout instance which will contain generated index buffer.
        //! \param material material which contains shader 
        //! \param data indices data.
        //! \param numberOfIndices number of vertex in the data.
        //! \return new index buffer instance
        IndexBufferPtr createIndexBuffer(const ConstArrayAccessor1<unsigned int>& data, size_t numberOfIndices) override;

        //! Create Shader Program from presets.
        //! \param shader preset name
        //! \return new shader pointer
        ShaderPtr createShaderPreset(const std::string& shaderName) override;
        
        //! Create Framebuffer object 
        //! \param viewportSize viewport size of the framebuffer
        //! \return new framebuffer object
        FramebufferPtr createFramebuffer(Size2 viewportSize) override;

        //! Set viewport of the current window
        //! \param x left bottom x position
        //! \param y left bottom y position
        //! \param width width of the viewport
        //! \param height height of the viewport.
        void setViewport(int x, int y, size_t width, size_t height) override;

        //! Create the 2d texture from the image data.
        //! \param param texture parameter which will be used for creating or binding texture.
        //! \param size width x height of the given image data.
        //! \param data first pixel pointer of the image bytes array.
        //! \return return genearted 2d texture.
        Texture2DPtr createTexture2D(const TextureParams& param, Size2 size, void* data) override;

        //! Create the 3d texture from the image data.
        //! \param param texture parameter which will be used for creating or binding texture.
        //! \param size width x height x depth of the given image data.
        //! \param data first pixel pointer of the image bytes array.
        //! \return return genearted 3d texture.
        Texture3DPtr createTexture3D(const TextureParams& param, Size3 size, void* data) override;
    protected:
        void onRenderBegin() override;
        void onRenderEnd() override;
        void onSetRenderState() override;
    private:
    };

    using GL3RendererPtr = std::shared_ptr<GL3Renderer>;
} 
}

#endif

#endif 