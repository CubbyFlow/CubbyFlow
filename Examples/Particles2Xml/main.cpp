/*************************************************************************
> File Name: main.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Convert particles to XML file.
> Created Time: 2017/09/09
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <../LyraUtils.h>

#include <Core/Array/Array1.h>
#include <Core/Vector/Vector3.h>
#include <Core/Utils/Serialization.h>

#include <lyra/lyra.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace CubbyFlow;

void PrintInfo(size_t numberOfParticles)
{
    printf("Number of particles: %zu\n", numberOfParticles);
}

void ParticlesToXML(
    const Array1<Vector3D>& positions,
    const std::string& xmlFileName)
{
    PrintInfo(positions.size());

    std::ofstream file(xmlFileName.c_str());
    if (file)
    {
        printf("Writing %s...\n", xmlFileName.c_str());

        file << "<scene version=\"0.5.0\">";

        for (const auto& pos : positions)
        {
            file << "<shape type=\"instance\">";
            file << "<ref id=\"spheres\"/>";
            file << "<transform name=\"toWorld\">";

            char buffer[64];
            snprintf(
                buffer,
                sizeof(buffer),
                "<translate x=\"%f\" y=\"%f\" z=\"%f\"/>",
                pos.x,
                pos.y,
                pos.z);
            file << buffer;

            file << "</transform>";
            file << "</shape>";
        }

        file << "</scene>";

        file.close();
    }
    else
    {
        printf("Cannot write file %s.\n", xmlFileName.c_str());
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[])
{
    bool showHelp = false;

    std::string inputFileName;
    std::string outputFileName;

    // Parsing
    auto parser =
        lyra::help(showHelp) |
        lyra::opt(inputFileName, "inputFileName")
        ["-i"]["--input"]
        ("input particle position file name") |
        lyra::opt(outputFileName, "outputFileName")
        ["-o"]["--output"]
        ("output xml file name");

    auto result = parser.parse(lyra::args(argc, argv));
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

    // Read particle positions
    Array1<Vector3D> positions;
    std::ifstream positionFile(inputFileName.c_str(), std::ifstream::binary);
    if (positionFile)
    {
        const std::vector<uint8_t> buffer(
            (std::istreambuf_iterator<char>(positionFile)),
            (std::istreambuf_iterator<char>()));
        Deserialize(buffer, &positions);
        positionFile.close();
    }
    else
    {
        printf("Cannot read file %s.\n", inputFileName.c_str());
        exit(EXIT_FAILURE);
    }

    // Run marching cube and save it to the disk
    ParticlesToXML(positions, outputFileName);

    return EXIT_SUCCESS;
}