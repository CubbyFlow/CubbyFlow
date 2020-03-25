/*************************************************************************
> File Name: main.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Visualize CubbyFlow examples with OpenGL3.
> Created Time: 2020/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <../ClaraUtils.h>

#include <Core/Utils/Logging.h>
#include <Framework/Common.h>
#include <Clara/include/clara.hpp>
#include <pystring/pystring.h>

#ifdef CUBBYFLOW_WINDOWS
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include <Framework/Media/ScreenRecorder.h>

#include <Framework/GL3/GL3Application.h>
#include <Framework/GL3/GL3Shader.h>
#include <Framework/GL3/GL3Renderer.h>
#include <Framework/Application.h>
#include <Framework/Window.h>
#include <Framework/Shader.h>
#include <Framework/Material.h>
#include <Core/Array/Array1.h>
#include <Core/Size/Size3.h>

#include <memory>
#include <fstream>
#include <iostream>
#include <string>

#define APP_NAME "GL3Examples"

using namespace CubbyFlow;
using namespace CubbyRender;

void RunExample1(ApplicationPtr application, int resX, int resY, int numberOfFrames, ScreenRecorderPtr recorder)
{
    auto window = application->createWindow("SPH Simulation", resX, resY);
    ShaderPtr simpleShader = std::make_shared<GL3Shader>("simple_shader");
    MaterialPtr simpleMaterial = std::make_shared<Material>(simpleShader);

    Array1<float> vertices = {
        -1.0f, +1.0f, +1.0f, // 0
        +1.0f, +0.0f, +0.0f,
        +1.0f, +1.0f, +1.0f, // 1
        +0.0f, +1.0f, +0.0f,
        +1.0f, +1.0f, -1.0f, // 2
        +0.0f, +0.0f, +1.0f,
        -1.0f, +1.0f, -1.0f, // 3
        +1.0f, +1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f, // 4
        +1.0f, +0.0f, +1.0f,
        +1.0f, +1.0f, -1.0f, // 5
        +0.0f, +0.5f, +0.2f,
        +1.0f, -1.0f, -1.0f, // 6
        +0.8f, +0.6f, +0.4f,
        -1.0f, -1.0f, -1.0f, // 7
        +0.3f, +1.0f, +0.5f,
        +1.0f, +1.0f, -1.0f, // 8
        +0.2f, +0.5f, +0.2f,
        +1.0f, +1.0f, +1.0f, // 9
        +0.9f, +0.3f, +0.7f,
        +1.0f, -1.0f, +1.0f, // 10
        +0.3f, +0.7f, +1.0f,
        +1.0f, -1.0f, -1.0f, // 11
        +0.5f, +0.7f, +0.5f,
        -1.0f, +1.0f, +1.0f, // 12
        +0.7f, +0.8f, +0.2f,
        -1.0f, +1.0f, -1.0f, // 13
        +0.5f, +0.7f, +0.3f,
        -1.0f, -1.0f, -1.0f, // 14
        +0.4f, +0.7f, +0.7f,
        -1.0f, -1.0f, +1.0f, // 15
        +0.2f, +0.5f, +1.0f,
        +1.0f, +1.0f, +1.0f, // 16
        +0.6f, +1.0f, +0.7f,
        -1.0f, +1.0f, +1.0f, // 17
        +0.6f, +0.4f, +0.8f,
        -1.0f, -1.0f, +1.0f, // 18
        +0.2f, +0.8f, +0.7f,
        +1.0f, -1.0f, +1.0f, // 19
        +0.2f, +0.7f, +1.0f,
        +1.0f, -1.0f, -1.0f, // 20
        +0.8f, +0.3f, +0.7f,
        -1.0f, -1.0f, -1.0f, // 21
        +0.8f, +0.9f, +0.5f,
        -1.0f, -1.0f, +1.0f, // 22
        +0.5f, +0.8f, +0.5f,
        +1.0f, -1.0f, +1.0f, // 23
        +0.9f, +1.0f, +0.2f
    };

    window->getRenderer()->createVertexBuffer(simpleMaterial, vertices.ConstAccessor(), 48, VertexFormat::Position3, false);
    application->run(numberOfFrames, recorder);
    application->terminate();
}

void RunExample2(ApplicationPtr application, int resX, int resY, int numberOfFrames, ScreenRecorderPtr recorder)
{
    auto window = application->createWindow("PCISPH Simulation", resX, resY);
    UNUSED_VARIABLE(window);
    application->run(numberOfFrames, recorder);
    application->terminate();
}

int main(int argc, char* argv[])
{
    bool showHelp = false;
    int numberOfFrames = 120;
    int exampleNum = 1;
    int resX = 800;
    int resY = 600;
    std::string logFileName = APP_NAME ".log";
    std::string outputDir = APP_NAME "_output";
    std::string format = "null";

    // Parsing
    auto parser =
        clara::Help(showHelp) |
        clara::Opt(resX, "resX")
        ["-x"]["--resx"]
        ("grid resolution in x-axis (default is 800)") |
        clara::Opt(resY, "resY")
        ["-y"]["--resy"]
        ("grid resolution in y-axis (default is 600)") |
        clara::Opt(numberOfFrames, "numberOfFrames")
        ["-f"]["--frames"]
        ("total number of frames (default is 100)") |
        clara::Opt(logFileName, "logFileName")
        ["-l"]["--log"]
        ("log file name (default is " APP_NAME ".log)") |
        clara::Opt(exampleNum, "exampleNum")
        ["-e"]["--example"]
        ("example number (between 1 and 5, default is 1)") |
        clara::Opt(outputDir, "outputDir")
        ["-o"]["--output"]
        ("output directory name (default is " APP_NAME "_output)") |
        clara::Opt(format, "format")
        ["-m"]["--format"]
        ("simulation output format (tga or null. default is null)");

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

    ApplicationPtr      application { nullptr };
    ScreenRecorderPtr   recorder    { nullptr };

    application = std::make_shared<GL3Application>();
    if (application->initialize())
    {
        std::cerr << "Initialize application failed.";
        return -1;
    }
    
    if (format != "null") 
        recorder = std::make_shared<ScreenRecorder>();
    
    switch (exampleNum)
    {
    case 1:
        RunExample1(application, resX, resY, numberOfFrames, recorder);
        break;
    case 2:
        RunExample2(application, resX, resY, numberOfFrames, recorder);
        break;
    default:
        std::cout << ToString(parser) << '\n';
        exit(EXIT_FAILURE);
    }

    if (format == "tga")
    {
        std::cout << "Save recording result as tga...";
        recorder->saveAllFrames(outputDir);
        std::cout << "complete" << '\n';
    }
    
    return EXIT_SUCCESS;
}