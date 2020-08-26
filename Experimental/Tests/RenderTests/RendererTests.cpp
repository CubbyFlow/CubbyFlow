#include "gtest/gtest.h"
#include <Vox/Device.hpp>
#include <Vox/Renderer.hpp>
#include <Vox/FileSystem.hpp>
#include "TestApp.hpp"
#include "RenderTestsUtils.hpp"

using namespace CubbyFlow;

TEST(Renderer, InitializeWindow)
{
    Vox::Device::Initialize();
    Vox::Device::Terminate();
}

TEST(Renderer, RunApp)
{
    Vox::Device::Initialize();
    Vox::Device::RunApp(std::make_shared<TestApp>(3), "TestScene.json");
    Vox::Device::Terminate();
}