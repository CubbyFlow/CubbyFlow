#include "TestApp.hpp"
#include <Vox/Utils/FileSystem.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <glad/glad.h>

TestApp::TestApp(size_t shutdownTimer)
    : _shutdownTimer(shutdownTimer)
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
    static size_t count = 0;

    std::shared_ptr<Vox::FrameContext> ctx = Vox::App::PopFrameContextFromQueue();
    ctx->MakeContextCurrent();

    if (count++ == _shutdownTimer)
        ctx->SetWindowContextShouldClose(true);

    Vox::App::PushFrameContextToQueue(ctx);
}