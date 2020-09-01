/*************************************************************************
> File Name: ObjViewer.cpp
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
    class PostProcessing;
    class FrameBuffer;
    class Texture;
    class Program;
    class RenderableObject;
};

class ObjViewer : public Vox::App
{
public:
    //! Default Constructor
    ObjViewer();
    //! Default Destructor
    ~ObjViewer();
    
    //! Intiailize sample application.
    bool Initialize(const Vox::Path& scenePath) override;
    void DrawFrame() override;
    const char* GetWindowTitle() override { return "Obj Viewer"; };

protected:
private:
    std::shared_ptr<Vox::RenderableObject> _renderable;
    std::unique_ptr<Vox::PostProcessing> _postProcessing;
    std::shared_ptr<Vox::FrameBuffer> _mainPass;
    std::shared_ptr<Vox::Texture> _screenTexture;
    std::shared_ptr<Vox::Program> _meshShader;
};

#endif