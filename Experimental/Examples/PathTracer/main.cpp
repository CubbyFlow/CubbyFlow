/*************************************************************************
> File Name: main.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Fluid Mesh Renderer
> Created Time: 2020/06/02
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include "ClaraUtils.h"
#include "PathTracer.hpp"

#include <Vox/Core/Device.hpp>
#include <Vox/Core/Renderer.hpp>
#include <Vox/Core/App.hpp>
#include <Vox/Utils/FileSystem.hpp>

#include <Core/Utils/Macros.hpp>
#include <Core/Utils/Logging.hpp>

#ifdef CUBBYFLOW_WINDOWS
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include <iostream>
#include <fstream>

#define APP_NAME "PathTracer"

int main(int argc, const char** argv)
{
    bool showHelp = false;
    size_t width = 1200, height = 900;
    std::string logFileName = APP_NAME ".log";
    std::string outputDir = APP_NAME "_output";

    // Parsing
    auto parser =
        clara::Help(showHelp) |
        clara::Opt(width, "width")
        ["-w"]["--width"]
        ("width of the window (default is 1200)") |
        clara::Opt(height, "height")
        ["-h"]["--height"]
        ("height of the window (default is 900)") |
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

#ifdef CUBBYFLOW_WINDOWS
    _mkdir(outputDir.c_str());
#else
    mkdir(outputDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif

    CubbyFlow::Logging::SetAllStream(&std::cout);

    Vox::FileSystem::AddDirectory("./Resources");
    Vox::FileSystem::AddDirectory(outputDir);

    Vox::Device::Initialize();
    Vox::Device::RunApp(std::make_shared<PathTracer>(), "PathTracer.xml");
    Vox::Device::Terminate();

    return EXIT_SUCCESS;
}