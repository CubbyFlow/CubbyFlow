/*************************************************************************
> File Name: Prerequisites.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Forward declaration of frequently used classes
> Created Time: 2020/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_PREREQUISITES_H
#define CUBBYFLOW_PREREQUISITES_H

#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    class Application;
    class Buffer;
    class IndexBuffer;
    class Material;
    class Renderer;
    class Renderable;
    class PointsRenderable;
    class Shader;
    class Texture;
    class Texture2D;
    class Texture3D;
    class VertexBuffer;
    class Window;
    class InputLayout;
    class Simulation;
    class Camera;
    class CameraController;

    using InputLayoutPtr     = std::shared_ptr<InputLayout>;
    using ApplicationPtr     = std::shared_ptr<Application>;
    using BufferPtr          = std::shared_ptr<Buffer>;
    using IndexBufferPtr     = std::shared_ptr<IndexBuffer>;
    using MaterialPtr        = std::shared_ptr<Material>;
    using RendererPtr        = std::shared_ptr<Renderer>;
    using PointsRenderablePtr= std::shared_ptr<PointsRenderable>;
    using ConstRendererPtr   = std::shared_ptr<const Renderer>;
    using RenderablePtr      = std::shared_ptr<Renderable>;
    using ShaderPtr          = std::shared_ptr<Shader>;
    using TexturePtr         = std::shared_ptr<Texture>;
    using Texture2DPtr       = std::shared_ptr<Texture2D>;
    using Texture3DPtr       = std::shared_ptr<Texture3D>;
    using VertexBufferPtr    = std::shared_ptr<VertexBuffer>;
    using WindowPtr          = std::shared_ptr<Window>;
    using SimulationPtr      = std::shared_ptr<Simulation>;
    using CameraPtr          = std::shared_ptr<Camera>;
    using CameraControllerPtr= std::shared_ptr<CameraController>;
}
}

#endif