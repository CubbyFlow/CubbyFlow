/*************************************************************************
> File Name: PathTracer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Path tracing simulation sdf result.
> Created Time: 2020/12/27
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_PATH_TRACER
#define CUBBYFLOW_PATH_TRACER

#include <Vox/Core/App.hpp>
#include <Vox/Utils/GLTypes.hpp>
#include <memory>

namespace Vox {
    class GeometryCacheManager;
    class FluidBuffer;
    class PostProcessing;
    class Texture;
    class Program;
    class FrameBuffer;
    class S3TextureCompression;
};

class PathTracer : public Vox::App
{
public:
    //! Default Constructor
    PathTracer();
    //! Default Destructor
    ~PathTracer();
    
    //! Intiailize sample application.
    bool Initialize(const Vox::Path& scenePath) override;
    void DrawFrame() override;
    const char* GetWindowTitle() override { return "Particle Viewer"; };

protected:
 private:
    std::unique_ptr<Vox::FluidBuffer> _buffer;
    std::shared_ptr<Vox::GeometryCacheManager> _cacheMgr;
    std::unique_ptr<Vox::PostProcessing> _postProcessing;
    std::unique_ptr<Vox::S3TextureCompression> _compressor;
    std::shared_ptr<Vox::FrameBuffer> _mainPass;
    std::shared_ptr<Vox::Program> _meshShader;
    std::shared_ptr<Vox::Texture> _screenTexture;
};

#endif