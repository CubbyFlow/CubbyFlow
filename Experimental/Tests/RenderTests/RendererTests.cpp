#include "gtest/gtest.h"
#include <Vox/Renderer.hpp>
#include <Vox/FileSystem.hpp>
#include "TestApp.hpp"
#include "RenderTestsUtils.hpp"

using namespace CubbyFlow;

TEST(Renderer, InitializeWindow)
{
    Vox::Renderer::Initialize();
    Vox::Renderer::Terminate();
}

TEST(Renderer, RunApp)
{
    Vox::Renderer::Initialize();
    Vox::Renderer::RunApp(std::make_shared<TestApp>(3), "TestScene.json");
    Vox::Renderer::Terminate();
}