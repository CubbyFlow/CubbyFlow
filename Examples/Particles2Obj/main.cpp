// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <../ClaraUtils.hpp>

#include <Core/Array/Array.hpp>
#include <Core/Geometry/BoundingBox.hpp>
#include <Core/Geometry/MarchingCubes.hpp>
#include <Core/Geometry/TriangleMesh3.hpp>
#include <Core/Grid/ScalarGrid.hpp>
#include <Core/Grid/VertexCenteredScalarGrid.hpp>
#include <Core/PointsToImplicit/AnisotropicPointsToImplicit3.hpp>
#include <Core/PointsToImplicit/SPHPointsToImplicit3.hpp>
#include <Core/PointsToImplicit/SphericalPointsToImplicit3.hpp>
#include <Core/PointsToImplicit/ZhuBridsonPointsToImplicit3.hpp>
#include <Core/Utils/Serialization.hpp>

#include <pystring/pystring.h>
#include <Clara/include/clara.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace CubbyFlow;

const std::string strSpherical = "spherical";
const std::string strSPH = "sph";
const std::string strZhuBridson = "zhu_bridson";
const std::string strAnisotropic = "anisotropic";

double valSPHCutOffDensity = 0.5;
double valZhuBridsonCutOffThreshold = 0.25;
double valAnisoCutOffDensity = 0.5;
double valAnisoPositionSmoothingFactor = 0.5;
size_t valAnisoMinNumNeighbors = 25;

void PrintInfo(const Vector3UZ& resolution, const BoundingBox3D& domain,
               const Vector3D& gridSpacing, size_t numberOfParticles,
               const std::string& method)
{
    printf("Resolution: %zu x %zu x %zu\n", resolution.x, resolution.y,
           resolution.z);
    printf("Domain: [%f, %f, %f] x [%f, %f, %f]\n", domain.lowerCorner.x,
           domain.lowerCorner.y, domain.lowerCorner.z, domain.upperCorner.x,
           domain.upperCorner.y, domain.upperCorner.z);
    printf("Grid spacing: [%f, %f, %f]\n", gridSpacing.x, gridSpacing.y,
           gridSpacing.z);
    printf("Number of particles: %zu\n", numberOfParticles);
    printf("Reconstruction method: %s\n", method.c_str());
}

void TriangulateAndSave(const ScalarGrid3& sdf, const std::string& objFileName)
{
    TriangleMesh3 mesh;
    MarchingCubes(sdf.DataView(), sdf.GridSpacing(), sdf.DataOrigin(), &mesh,
                  0.0, DIRECTION_ALL);

    std::ofstream file(objFileName.c_str());
    if (file)
    {
        printf("Writing %s...\n", objFileName.c_str());
        mesh.WriteObj(&file);
        file.close();
    }
    else
    {
        printf("Cannot write file %s.\n", objFileName.c_str());
        exit(EXIT_FAILURE);
    }
}

void ParticlesToObj(const Array1<Vector3D>& positions,
                    const Vector3UZ& resolution, const Vector3D& gridSpacing,
                    const Vector3D& origin, double kernelRadius,
                    const std::string& method, const std::string& objFileName)
{
    PointsToImplicit3Ptr converter;
    if (method == strSpherical)
    {
        converter =
            std::make_shared<SphericalPointsToImplicit3>(kernelRadius, false);
    }
    else if (method == strSPH)
    {
        converter = std::make_shared<SPHPointsToImplicit3>(
            kernelRadius, valSPHCutOffDensity, false);
    }
    else if (method == strZhuBridson)
    {
        converter = std::make_shared<ZhuBridsonPointsToImplicit3>(
            kernelRadius, valZhuBridsonCutOffThreshold, false);
    }
    else
    {
        converter = std::make_shared<AnisotropicPointsToImplicit3>(
            kernelRadius, valAnisoCutOffDensity,
            valAnisoPositionSmoothingFactor, valAnisoMinNumNeighbors, false);
    }

    VertexCenteredScalarGrid3 sdf(resolution, gridSpacing, origin);
    PrintInfo(resolution, sdf.GetBoundingBox(), gridSpacing, positions.Length(),
              method);

    converter->Convert(positions, &sdf);

    TriangulateAndSave(sdf, objFileName);
}

int main(int argc, char* argv[])
{
    bool showHelp = false;

    std::string inputFileName;
    std::string outputFileName;
    Vector3UZ resolution(100, 100, 100);
    Vector3D gridSpacing(0.01, 0.01, 0.01);
    Vector3D origin;
    std::string method = "anisotropic";
    double kernelRadius = 0.2;

    std::string strResolution;
    std::string strGridSpacing;
    std::string strOrigin;
    std::string strMethod;

    // Parsing
    auto parser =
        clara::Help(showHelp) |
        clara::Opt(inputFileName,
                   "inputFileName")["-i"]["--input"]("input obj file name") |
        clara::Opt(outputFileName,
                   "outputFileName")["-o"]["--output"]("output obj file name") |
        clara::Opt(strResolution, "resolution")["-r"]["--resolution"](
            "grid resolution in CSV format (default is 100,100,100)") |
        clara::Opt(strGridSpacing, "gridSpacing")["-g"]["--grid_spacing"](
            "grid spacing in CSV format (default is 0.01,0.01,0.01)") |
        clara::Opt(strOrigin, "origin")["-n"]["--origin"](
            "domain origin in CSV format (default is 0,0,0)") |
        clara::Opt(method, "method")["-m"]["--method"](
            "spherical, sph, zhu_bridson, and anisotropic "
            "followed by optional method-dependent parameters (default is "
            "anisotropic)") |
        clara::Opt(kernelRadius, "kernelRadius")["-k"]["--kernel"](
            "interpolation kernel radius (default is 0.2)");

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

    // Resolution
    if (!strResolution.empty())
    {
        std::vector<std::string> tokens;
        pystring::split(strResolution, tokens, ",");

        if (tokens.size() == 1)
        {
            resolution.x = resolution.y = resolution.z =
                static_cast<size_t>(atoi(strResolution.c_str()));
        }
        else if (tokens.size() == 3)
        {
            resolution.x = static_cast<size_t>(atoi(tokens[0].c_str()));
            resolution.y = static_cast<size_t>(atoi(tokens[1].c_str()));
            resolution.z = static_cast<size_t>(atoi(tokens[2].c_str()));
        }
    }

    // Grid spacing
    if (!strGridSpacing.empty())
    {
        std::vector<std::string> tokens;
        pystring::split(strGridSpacing, tokens, ",");

        if (tokens.size() == 1)
        {
            gridSpacing.x = gridSpacing.y = gridSpacing.z =
                atof(strGridSpacing.c_str());
        }
        else if (tokens.size() == 3)
        {
            gridSpacing.x = atof(tokens[0].c_str());
            gridSpacing.y = atof(tokens[1].c_str());
            gridSpacing.z = atof(tokens[2].c_str());
        }
    }

    // Origin
    if (!strOrigin.empty())
    {
        std::vector<std::string> tokens;
        pystring::split(strOrigin, tokens, ",");

        if (tokens.size() == 1)
        {
            origin.x = origin.y = origin.z = atof(strOrigin.c_str());
        }
        else if (tokens.size() == 3)
        {
            origin.x = atof(tokens[0].c_str());
            origin.y = atof(tokens[1].c_str());
            origin.z = atof(tokens[2].c_str());
        }
    }

    // Method
    if (!strMethod.empty())
    {
        std::vector<std::string> tokens;
        pystring::split(strMethod, tokens, ",");

        method = tokens[0];

        if (method == strSpherical)
        {
            // No other options accepted
        }
        else if (method == strSPH)
        {
            if (tokens.size() > 1)
            {
                valSPHCutOffDensity = atof(tokens[1].c_str());
            }
        }
        else if (method == strZhuBridson)
        {
            if (tokens.size() > 1)
            {
                valZhuBridsonCutOffThreshold = atof(tokens[1].c_str());
            }
        }
        else if (method == strAnisotropic)
        {
            if (tokens.size() > 1)
            {
                valAnisoCutOffDensity = atof(tokens[1].c_str());
            }
            if (tokens.size() > 2)
            {
                valAnisoPositionSmoothingFactor = atof(tokens[2].c_str());
            }
            if (tokens.size() > 3)
            {
                valAnisoMinNumNeighbors =
                    static_cast<size_t>(atoi(tokens[3].c_str()));
            }
        }
        else
        {
            fprintf(stderr, "Unknown method %s.\n", method.c_str());
            std::cout << ToString(parser) << '\n';
            exit(EXIT_FAILURE);
        }
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
    ParticlesToObj(positions, resolution, gridSpacing, origin, kernelRadius,
                   method, outputFileName);

    return EXIT_SUCCESS;
}