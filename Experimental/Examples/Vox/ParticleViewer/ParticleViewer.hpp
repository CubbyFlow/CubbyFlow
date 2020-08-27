/*************************************************************************
> File Name: ParticleViewer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Particle Viewer
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_PARTICLE_VIEWER
#define CUBBYFLOW_PARTICLE_VIEWER

#include <Vox/App.hpp>
#include <Vox/GLTypes.hpp>
#include <memory>

namespace Vox {
    class RoundRobinAsyncBuffer;
    class DebugDraw;
    class PostProcessing;
    class GeometryCacheManager;
};

class ParticleViewer : public Vox::App
{
public:
    //! Default Constructor
    ParticleViewer();
    //! Default Destructor
    ~ParticleViewer();
    
    //! Intiailize sample application.
    bool Initialize(const Vox::Path& scenePath) override;
    void DrawFrame() override;
    const char* GetWindowTitle() override { return "Particle Viewer"; };

protected:
private:
    std::unique_ptr<Vox::RoundRobinAsyncBuffer> _buffer;
    std::unique_ptr<Vox::DebugDraw> _debugDraw;
    std::unique_ptr<Vox::PostProcessing> _postProcessing;
    std::shared_ptr<Vox::GeometryCacheManager> _cacheMgr;
};

#endif