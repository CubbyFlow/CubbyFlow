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
#include <Clara/include/clara.hpp>
#include <pystring/pystring.h>

#include <Framework/Window/Window.h>
#include <Framework/Utils/Common.h>
#include <GL3/Application/GL3Application.h>
#include <GL3/Renderer/GL3Renderer.h>
#include <GL3/Utils/GL3Debugging.h>
#include <Media/ScreenRecorder.h>
#include <Core/Array/Array1.h>
#include <Core/Size/Size3.h>

#include <memory>
#include <fstream>
#include <iostream>
#include <string>

#ifdef CUBBYFLOW_WINDOWS
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include "SPHSimulation.h"
#include "PICSimulation.h"

#define APP_NAME "GL3Examples"

using namespace CubbyFlow;
using namespace CubbyRender;

int main(int argc, char* argv[])
{
    bool showHelp = false;
    int numberOfFrames = 60;
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
        clara::Opt(numberOfFrames, "numberOfFrames")
        ["-n"]["--numberOfFrames"]
        ("total number of frames (default is 100)") |
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

    ApplicationPtr application = std::make_shared<GL3Application>();
    if (application->initialize())
    {
        std::cerr << "Initialize application failed.";
        return -1;
    }
    
    application->createMainWindow("SPH Simulation", resX, resY);
    
    auto window = application->getMainWindow();
    window->registerSimulation(std::make_shared<SPHSimulation>(fps));
    
#ifdef CUBBYFLOW_RECORDING
    auto recorder = std::make_shared<ScreenRecorder>();
    recorder->setWorkingDirectory(APP_NAME "_output");

    application->run(numberOfFrames, [&](Size2 dim, const ArrayAccessor1<unsigned char>& frame){
        return recorder->EncodeFrame(dim, frame);
    });

    recorder.reset();
#else
    application->run(numberOfFrames, nullptr);
#endif
    return EXIT_SUCCESS;
}