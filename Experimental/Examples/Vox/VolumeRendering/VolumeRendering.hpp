/*************************************************************************
> File Name: VolumeRendering.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Particle Viewer
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOLUME_RENDERING
#define CUBBYFLOW_VOLUME_RENDERING

#include <Vox/App.hpp>
#include <Vox/GLTypes.hpp>
#include <memory>

namespace Vox {
    class PostProcessing;
    class RenderableObject;
    class Texture;
    class FrameBuffer;
    class Program;
};

class VolumeRendering : public Vox::App
{
public:
    //! Default Constructor
    VolumeRendering();
    //! Default Destructor
    ~VolumeRendering();
    
    //! Intiailize sample application.
    bool Initialize(const Vox::Path& scenePath) override;
    void DrawFrame() override;
    const char* GetWindowTitle() override { return "Volume Renderer"; };

protected:
private:
    std::unique_ptr<Vox::PostProcessing> _postProcessing;
    std::shared_ptr<Vox::RenderableObject> _cubeRenderable;
    std::shared_ptr<Vox::Program> _rayCastingShader;
    std::shared_ptr<Vox::Program> _rayDataShader;
    std::shared_ptr<Vox::FrameBuffer> _rayDataPass;
    std::shared_ptr<Vox::FrameBuffer> _mainPass;
    std::shared_ptr<Vox::Texture> _screenTexture;
    std::shared_ptr<Vox::Texture> _volumeSDF;
    std::shared_ptr<Vox::Texture> _volumeFrontFace;
    std::shared_ptr<Vox::Texture> _volumeBackFace;
    GLuint _frontface, _backface;
};

#endif