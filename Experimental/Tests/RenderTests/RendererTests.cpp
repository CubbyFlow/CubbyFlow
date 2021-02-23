#include "gtest/gtest.h"
#include <Vox/Core/Device.hpp>
#include <Vox/Core/Renderer.hpp>
#include <Vox/Utils/FileSystem.hpp>
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
    Vox::Device::RunApp(std::make_shared<TestApp>(3), "TestScene.xml");
    Vox::Device::Terminate();
}