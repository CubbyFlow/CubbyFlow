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
    class Entry;
    class IndexBuffer;
    class Material;
    class Renderer;
    class Renderable;
    class Shader;
    class ShaderParameter;
    class Texture;
    class VertexBuffer;
    class Window;
    
    using ApplicationPtr     = std::shared_ptr<Application>;
    using BufferPtr          = std::shared_ptr<Buffer>;
    using EntryPtr           = std::shared_ptr<Entry>;
    using IndexBufferPtr     = std::shared_ptr<IndexBuffer>;
    using MaterialPtr        = std::shared_ptr<Material>;
    using RendererPtr        = std::shared_ptr<Renderer>;
    using ConstRendererPtr   = std::shared_ptr<const Renderer>;
    using RenderablePtr      = std::shared_ptr<Renderable>;
    using ShaderPtr          = std::shared_ptr<Shader>;
    using ShaderParameterPtr = std::shared_ptr<ShaderParameter>;
    using TexturePtr         = std::shared_ptr<Texture>;
    using VertexBufferPtr    = std::shared_ptr<VertexBuffer>;
    using WindowPtr          = std::shared_ptr<Window>;
}
}

#endif