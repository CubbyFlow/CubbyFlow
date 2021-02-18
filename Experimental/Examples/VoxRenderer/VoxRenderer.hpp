/*************************************************************************
> File Name: VoxRenderer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Particle Viewer
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
    class GeometryCacheManager;
    class FluidRenderable;
    class StaticRenderable;
    class PostProcessing;
    class Texture;
    class Program;
    class FrameBuffer;
    class CubeMesh;
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
    void UpdateFrame(double dt) override;
    void DrawFrame() override;
    const char* GetWindowTitle() override { return "CubbyFlow Fluid Renderer"; };

protected:
    void OnSetKey(int key, int scancode, int action, int mods) override;
private:
    std::shared_ptr<Vox::FluidRenderable> _buffer;
    CubbyFlow::Array1<std::shared_ptr<Vox::StaticRenderable>> _obstacles;
    std::unique_ptr<Vox::PostProcessing> _postProcessing;
    std::shared_ptr<Vox::FrameBuffer> _mainPass;
    std::shared_ptr<Vox::Texture> _screenTexture;
    std::shared_ptr<Vox::StaticRenderable> _skybox;
    bool _animStopped = false;
    GLenum _polygonMode;
};

#endif