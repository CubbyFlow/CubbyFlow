#include "TestApp.hpp"
#include <Vox/FileSystem.hpp>
#include <Vox/FrameContext.hpp>
#include <glad/glad.h>

TestApp::TestApp()
{
    //! Do nothing.
}

TestApp::~TestApp()
{
    //! Do nothing.
}

bool TestApp::Initialize(const Vox::Path& scenePath)
{
    if (!App::Initialize(scenePath))
        return false;
    
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();

    Vox::App::PushFrameContextToQueue(ctx);
    return true;
}

void TestApp::DrawFrame() 
{
    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->MakeContextCurrent();

    Vox::App::PushFrameContextToQueue(ctx);
}