/*************************************************************************
> File Name: main.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Simulate Cubbyflow interactively while modifying several parameters.
> Created Time: 2020/06/16
> Copyright (c) 2020, Chan-Ho Chris Ohk
*************************************************************************/
#include <../ClaraUtils.h>

#include <Clara/include/clara.hpp>
#include <pystring/pystring.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


void PrintInfo()
{
}

int main(int argc, char* argv[])
{
    bool showHelp = false;

    std::string inputFileName;
    std::string outputFileName;

    // Parsing
    auto parser =
        clara::Help(showHelp) |
        clara::Opt(inputFileName, "inputFileName")
        ["-i"]["--input"]
        ("input obj file name") |
        clara::Opt(outputFileName, "outputFileName")
        ["-o"]["--output"]
        ("output obj file name");

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

    if (inputFileName.empty() || outputFileName.empty())
    {
        std::cout << ToString(parser) << '\n';
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}