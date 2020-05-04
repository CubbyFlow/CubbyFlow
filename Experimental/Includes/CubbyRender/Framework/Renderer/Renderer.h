/*************************************************************************
> File Name: Renderer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Renderer object.
> Created Time: 2020/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_RENDERER_H
#define CUBBYFLOW_RENDERER_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Buffer/Vertex.h>
#include <Framework/Renderer/RenderOptions.h>
#include <Framework/View/Camera.h>
#include <Framework/Texture/Texture.h>
#include <Core/Array/ArrayAccessor1.h>
#include <Core/Array/ArrayAccessor2.h>
#include <Core/Array/ArrayAccessor3.h>
#include <Core/Vector/Vector4.h>
#include <Core/Size/Size2.h>
#include <vector>
#include <string>

namespace CubbyFlow {
namespace CubbyRender {

    //!
    //! \brief Abstract base class for Renderer object.
    //!
    //! This class contains basic interface for renderer. 
    //! The wrapper class of each Graphics API must overrides 
    //! this renderer class.
    //!
    class Renderer : public std::enable_shared_from_this<Renderer>
    {
    public:
        //! Default constructor.
        Renderer();

        //! Default destructor.
        virtual ~Renderer();

        //! Initialize and fetch gl commands.
        virtual int initializeGL() = 0;

        //! call all render method of renderable instances.
        void render();

        //! let inputlayout draw it's vertices using this renderer instance.
        virtual void draw(InputLayoutPtr inputLayout) = 0;

        //! set state of the rendering(culling, etc..)
        void setRenderState(const RenderState& renderState);

        //! set primitive type for rendering geometry. (point, triangle-strip, etc..)
        void setPrimitiveType(PrimitiveType type);

        //! add new renderable instance.
        void addRenderable(RenderablePtr renderable);

        //! clear all renderable instances registered.
        void clearRenderables();

        //! set background color of the window screen.
        void setBackgroundColor(Vector4F color);

        //! get current frame image as array of floating point data.
        virtual ArrayAccessor1<unsigned char> getCurrentFramebuffer(Size2 size) const = 0;

        //! Create Inputlayout pointer with default constructor.
        virtual InputLayoutPtr createInputLayout() = 0;

        //! Create VertexBuffer pointer with given parameters.
        //!
        //! \param material material which contains shader 
        //! \param data vertices data.
        //! \param numberOfVertices number of vertex in the data.
        //! \param format format of the input vertex
        //! \param storeData store vertex data into vertex buffer or not.
        //! \return new vertex buffer instance
        virtual VertexBufferPtr createVertexBuffer(const ConstArrayAccessor1<float>& data, size_t numberOfVertices, VertexFormat format) = 0;

        //! Create VertexBuffer pointer with given parameters.
        //!
        //! \param material material which contains shader 
        //! \param data indices data.
        //! \param numberOfIndices number of vertex in the data.
        //! \param storeData store index data into index buffer or not.
        //! \return new index buffer instance
        virtual IndexBufferPtr createIndexBuffer(const ConstArrayAccessor1<unsigned int>& data, size_t numberOfIndices) = 0;

        //! Create Shader Program from presets.
        //! \param shader preset name
        //! \return new shader pointer
        virtual ShaderPtr createShaderPreset(const std::string& shaderName) = 0;

        //! Set viewport of the current window
        //! \param x left top x position
        //! \param y left top y position
        //! \param width width of the viewport
        //! \param height height of the viewport.
        virtual void setViewport(int x, int y, size_t width, size_t height) = 0;

        //! Create Material
        //! \return new material pointer
        MaterialPtr createMaterial() const;

        //! Bind material (shader and textures) to this renderer.
        void bindMaterial(MaterialPtr material);

        //! Unbind material (shader and textures) from this renderer.
        void unbindMaterial(MaterialPtr material);

        //! Bind inputLayout (vertex buffer and index buffer) to this renderer.
        void bindInputLayout(InputLayoutPtr inputLayout);

        //! Unbind inputLayout (vertex buffer and index buffer) from this renderer.
        void unbindInputLayout(InputLayoutPtr inputLayout);

        //! Set the camera of the renderer.
        void setCamera(CameraPtr camera);

        //! Get the camera of the renderer.
        const CameraPtr& getCamera() const;

        //! Create the 2d texture from the image data.
        //! \param data 2d floating-point data of the texture
        //! \param samplingMode texture sampling mode. select kNearest or kLinear
        //! \return return genearted 2d texture with given data and sampling mode
        virtual Texture2DPtr createTexture2D(const ConstArrayAccessor2<Vector4F>& data, TextureSamplingMode samplingMode) = 0;

        //! Create the 2d texture from the image data.
        //! \param data 2d unsigned char data of the texture
        //! \param samplingMode texture sampling mode. select kNearest or kLinear
        //! \return return genearted 2d texture with given data and sampling mode
        virtual Texture2DPtr createTexture2D(const ConstArrayAccessor2<Vector4UB>& data, TextureSamplingMode samplingMode) = 0;

        //! Create the 3d texture from the image data.
        //! \param data 3d floating-point data of the texture
        //! \param samplingMode texture sampling mode. select kNearest or kLinear
        //! \return return genearted 3d texture with given data and sampling mode
        virtual Texture3DPtr createTexture3D(const ConstArrayAccessor3<Vector4F>& data, TextureSamplingMode samplingMode) = 0;

        //! Create the 3d texture from the image data.
        //! \param data 3d unsigned char data of the texture
        //! \param samplingMode texture sampling mode. select kNearest or kLinear
        //! \return return genearted 3d texture with given data and sampling mode
        virtual Texture3DPtr createTexture3D(const ConstArrayAccessor3<Vector4UB>& data, TextureSamplingMode samplingMode) = 0;
    protected:
        virtual void onRenderBegin() = 0;
        virtual void onRenderEnd() = 0;
        virtual void onSetRenderState() = 0;

        std::vector<RenderablePtr> _renderables;
        RenderState _renderState;
        PrimitiveType _primitiveType;
        CameraPtr _camera = nullptr;
        Vector4F _backgroundColor = Vector4F(1.0f, 0.0f, 0.0f, 1.0f);
    private:
    };

    using RendererPtr = std::shared_ptr<Renderer>;
} 
}

#endif