/*************************************************************************
> File Name: VoxRenderer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Scene file based fluid renderer
> Created Time: 2020/08/11
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_RENDERER
#define CUBBYFLOW_VOX_RENDERER

#include <Vox/Core/App.hpp>
#include <Vox/Utils/GLTypes.hpp>
#include <Core/Array/Array1.hpp>
#include <memory>

namespace Vox {
    class FluidRenderable;
    class StaticRenderable;
    class PostProcessing;
    class Texture;
    class FrameBuffer;
    class PointLight;
};

class VoxRenderer : public Vox::App
{
public:
    //! Default Constructor
    VoxRenderer();
    //! Constructor with window size
    VoxRenderer(const int width, const int height);
    //! Default Destructor
    ~VoxRenderer();
    
    //! Intiailize sample application.
    bool Initialize(const Vox::Path& scenePath) override;
    //! Update the renderer frame. such as uniform variables upating.
    void UpdateFrame(double dt) override;
    //! Draw the VoxRenderer frame.
    void DrawFrame() override;
    //! Returns the window title as const char pointer
    const char* GetWindowTitle() override { return "CubbyFlow Fluid Renderer"; };

protected:
    void OnSetKey(int key, int scancode, int action, int mods) override;
private:
    CubbyFlow::Array1<std::shared_ptr<Vox::FluidRenderable>> _fluidAnims; // fluid animation buffer instance
    CubbyFlow::Array1<std::shared_ptr<Vox::StaticRenderable>> _obstacles; // static renderable object which act as obstacle in simul
    std::shared_ptr<Vox::PointLight> _pointLight; // point light objects manager
    std::unique_ptr<Vox::PostProcessing> _postProcessing; // post-processing class which do gamma-correction in this case
    std::shared_ptr<Vox::FrameBuffer> _mainPass; // main render target
    std::shared_ptr<Vox::Texture> _screenTexture; // color attachment of the main render target
    std::shared_ptr<Vox::StaticRenderable> _skybox; // skybox instance for rendering cubemap.
    bool _animStopped = false;
    GLenum _polygonMode;
};

#endif