/*************************************************************************
> File Name: FluidSimulator.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: PIC Simulation Sample example.
> Created Time: 2020/05/09
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_FLUID_SIMULATOR_H
#define CUBBYFLOW_FLUID_SIMULATOR_H

#include <Framework/Buffer/Framebuffer.h>
#include <Framework/Texture/Texture.h>
#include <Framework/Texture/Texture2D.h>
#include <Framework/Window/Docker.h>
#include <Framework/Application/Application.h>
#ifdef CUBBYFLOW_USE_GL

#include "../Utils/ClaraUtils.h"
#include "../Utils/main.h"
#include <GL3/Window/GL3Window.h>
#include <GL3/Buffer/GL3Framebuffer.h>
#include <GL3/Renderer/GL3Renderer.h>
#include <GL3/Application/GL3Application.h>
#include <GLFW/glfw3.h>

#include <Media/ScreenRecorder.h>

#include <memory>
#include <fstream>
#include <iostream>
#include <string>

#ifdef CUBBYFLOW_WINDOWS
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include "SimulationHelper.h"
#include "PICSimulation.h"
#include "SPHSimulation.h"

#define APP_NAME "FluidSimulator"

using namespace CubbyFlow;
using namespace CubbyRender;

//! 실시간으로 다 돌리기보다는 한 프레임 한 프레임 정지해가며 관찰한다는 느낌으로

class SampleWindow final : public GL3Window
{
    using super_t = GL3Window;
public: 
    //! Default constructor.
    SampleWindow() = default;
    //! Default Constructor.
    SampleWindow(const std::string& title, int width, int height);
    //! Default Destructor.
    ~SampleWindow();
    
    bool initialize();
    //! Action implementation when window is resized.
    void onWindowResized(int width, int height) override;
    //! Action implementation when any key is pressed or released
    void onKey(int key, int scancode, int action, int mods) override;
    //! Action implementation when any mouse button is pressed or released.
    void onMouseButton(int button, int action, int mods) override;
    //! Action implementation when cursor is moved
    void onMouseCursorPos(double x, double y) override;
    //! Action implementation when mouse scroll is moved
    void onMouseScroll(double deltaX, double deltaY) override;
protected:
    void onRenderScene() override;
    void onUpdateScene() override;
private:
    bool addDocker(SimulationPtr simulation);
    SimulationHelper _simHelper;
};
using SampleWindowPtr = std::shared_ptr<SampleWindow>;

ApplicationPtr application;
SampleWindowPtr sample;

SampleWindow::SampleWindow(const std::string& title, int width, int height)
    : GL3Window(title, width, height)
{
    //! Do nothing.
}
SampleWindow::~SampleWindow()
{
    //! Do nothing.
}

bool SampleWindow::initialize()
{
    if (!_simHelper.initialize(_glfwWindow))
        return false;
    if (!addDocker(std::make_shared<SPHSimulation>()))
        return false;
    if (!addDocker(std::make_shared<PICSimulation>()))
        return false;
    
    return true;
}

bool SampleWindow::addDocker(SimulationPtr simulation)
{
    FramebufferPtr framebuffer = _renderer->createFramebuffer(_windowSize);

    TextureParams textureParam;
    textureParam.format = ImageFormat::RGBA;
    textureParam.internalFormat = ImageInternalFormat::RGBA8;
    textureParam.type = DataType::UNSIGNED_BYTE;
    textureParam.samplingMode = ImageSamplingMode::LINEAR;
    textureParam.wrapMethod = ImageWrapMethod::REPEAT;

    Texture2DPtr colorTexture = _renderer->createTexture2D(textureParam, _windowSize, nullptr);
    framebuffer->attachColorTexture(_renderer, colorTexture);
    
    textureParam.format = ImageFormat::DEPTH_COMPONENT;
    textureParam.internalFormat = ImageInternalFormat::DEPTH_COMPONENT16;
    textureParam.type = DataType::FLOAT;
    textureParam.samplingMode = ImageSamplingMode::NEAREST;
    textureParam.wrapMethod = ImageWrapMethod::CLAMP_TO_EDGE;
    Texture2DPtr depthTexture = _renderer->createTexture2D(textureParam, _windowSize, nullptr);
    framebuffer->attachDepthTexture(_renderer, depthTexture);

    //! Configure framebuffer with several attachments.
    if (!framebuffer->configure(_renderer))
        return false;

    DockerPtr docker = std::make_shared<Docker>(simulation->name());
    docker->setFramebuffer(framebuffer);
    docker->registerSimulation(_renderer, simulation);

    _dockers.push_back(docker);
    _simHelper.addSimulationTexture(colorTexture); //! connect simulation result texture pointer 
    return true;
}

void SampleWindow::onWindowResized(int width, int height)
{
    UNUSED_VARIABLE(width);
    UNUSED_VARIABLE(height);
}

void SampleWindow::onKey(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(_glfwWindow, GLFW_TRUE);
    }
    UNUSED_VARIABLE(scancode);
    UNUSED_VARIABLE(mods);
}

void SampleWindow::onMouseButton(int button, int action, int mods)
{
    UNUSED_VARIABLE(button);
    UNUSED_VARIABLE(action);
    UNUSED_VARIABLE(mods);
}   

void SampleWindow::onMouseCursorPos(double x, double y)
{
    UNUSED_VARIABLE(x);
    UNUSED_VARIABLE(y);
}   

void SampleWindow::onMouseScroll(double deltaX, double deltaY)
{
    UNUSED_VARIABLE(deltaX);
    UNUSED_VARIABLE(deltaY);
}

void SampleWindow::onRenderScene()
{
    _simHelper.beginRender();
    //! Sample Window Rendering Here.
    _simHelper.render();
    _simHelper.endRender();
}

void SampleWindow::onUpdateScene()
{
    //! Do nothing
}

int sampleMain(int argc, const char** argv)
{
    bool showHelp = false;
    int resX = 800;
    int resY = 600;
    double fps = 60.0;
    std::string logFileName = APP_NAME ".log";
    std::string outputDir = APP_NAME "_output";

    // Parsing
    auto parser =
        clara::Help(showHelp) |
        clara::Opt(resX, "resX")
        ["-x"]["--resx"]
        ("grid resolution in x-axis (default is 800)") |
        clara::Opt(resY, "resY")
        ["-y"]["--resy"]
        ("grid resolution in y-axis (default is 600)") |
        clara::Opt(fps, "fps")
        ["-f"]["--fps"]
        ("frame per second for simulation (default is 60)") |
        clara::Opt(logFileName, "logFileName")
        ["-l"]["--log"]
        ("log file name (default is " APP_NAME ".log)") |
        clara::Opt(outputDir, "outputDir")
        ["-o"]["--output"]
        ("output directory name (default is " APP_NAME "_output)");

    auto result = parser.parse(clara::Args(argc, argv));
    if (!result)
    {
        std::cerr << "Error in command line: " << result.errorMessage() << '\n';
        exit(EXIT_FAILURE);
    }

    if (showHelp)
    {
        std::cout << ToString(parser) << '\n';
        exit(EXIT_SUCCESS);
    }

#ifdef CUBBYFLOW_WINDOWS
    _mkdir(outputDir.c_str());
#else
    mkdir(outputDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif

    std::ofstream logFile(logFileName.c_str());
    if (logFile)
    {
        Logging::SetAllStream(&logFile);
    }

    application = std::make_shared<GL3Application>();
    if (application->initialize())
    {
        std::cerr << "Initialize application failed." << std::endl;
        return -1;
    }
    
    sample = std::make_shared<SampleWindow>(APP_NAME, resX, resY);
    if (sample->initialize() == false)
    {
        std::cerr << "Sample Window Initialize Failed" << std::endl;
        return -1;
    }
    
    int exitCode = 0;
    application->setMainWindow(sample);

#ifdef CUBBYFLOW_RECORDING
    auto recorder = std::make_shared<ScreenRecorder>();
    recorder->setWorkingDirectory(APP_NAME "_output");

    exitCode = application->run([&](Size2 dim, const ArrayAccessor1<unsigned char>& frame){
        return recorder->EncodeFrame(dim, frame);
    });
#else
    exitCode = application->run(nullptr);
#endif

    //! release application and sample window .
    sample.reset();
    application.reset();
    return exitCode;
}

#endif
#endif 