/*************************************************************************
> File Name: main.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Convert particles to offline rendered image with gvdb
> Created Time: 2020/06/16
> Copyright (c) 2020, Chan-Ho Chris Ohk
*************************************************************************/
#include <../ClaraUtils.h>

#include <Clara/include/clara.hpp>
#include <pystring/pystring.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

#include <Core/Vector/Vector3.h>
#include <Core/Array/Array1.h>
#include <Core/Utils/Serialization.h>

#include <gvdb.h>
#include <file_png.h>

using namespace CubbyFlow;
using namespace nvdb;

class Sample
{
public:
    Sample(int width, int height);
    ~Sample();

    bool initGVDB(const std::string& dir, const std::string& format);

    void clearGVDB();

    void updateGVDB();

    //! Load scene data file and parse.
    void parseScene(const std::string& dir, const std::string& format);

    void setWindowSize(int width, int height);

    void renderToFile(const std::string& dir);

    void renderFrame();

    void advanceFrame()
    {
        _frame++;
    }
protected:
private:
    DataPtr _particles;
    DataPtr _original;
    std::string _particleDirectory;
    std::string _format;
    int _width = 600;
    int _height = 400;
    int _numParticles = 0;
    int _frame = 0;
    float _radius = 0.5f;
};

using SamplePtr = std::shared_ptr<Sample>;

VolumeGVDB gvdb;
SamplePtr sample;

Sample::Sample(int width, int height)
    : _width(width), _height(height)
{

}
Sample::~Sample()
{

}

bool Sample::initGVDB(const std::string& dir, const std::string& format)
{
    gvdb.SetDebug(true);
    gvdb.SetVerbose(true);
    gvdb.SetProfile(false, true);
    gvdb.SetCudaDevice(GVDB_DEV_FIRST);
    gvdb.Initialize();

    Camera3D* cam = new Camera3D;
    cam->setFov(50.0f);
    cam->setNearFar(1.0f, 10000.0f);
    cam->setOrbit(nvdb::Vector3DF(50.0f, 30.0f, 0.0f), nvdb::Vector3DF(128.0f, 128.0f, 128.0f), 300, 1.0);
    gvdb.getScene()->SetCamera(cam);

    Light* light = new Light;
    light->setOrbit(nvdb::Vector3DF(0.0f, 40.0f, 0.0f), nvdb::Vector3DF(128.0f, 128.0f, 128.0f), 2000, 1.0);
    gvdb.getScene()->SetLight(0, light);

    gvdb.getScene()->SetSteps(0.1f, 16.0f, 0.1f);
    gvdb.getScene()->SetExtinct(-1.0f, 1.1f, 0.0f);
    gvdb.getScene()->SetVolumeRange(0.0f, -1.0f, 3.0f);
    gvdb.getScene()->SetCutoff(0.005f, 0.001f, 0.0f);
	gvdb.getScene()->SetBackgroundClr(0.1f, 0.2f, 0.4f, 1.0);

    gvdb.AddRenderBuf(0, _width, _height, 4);

    gvdb.Configure(3, 3, 3, 3, 4);  
	gvdb.SetChannelDefault(32, 32, 1);
	gvdb.AddChannel( 0, T_FLOAT, 1, F_LINEAR );	
	gvdb.FillChannel( 0, nvdb::Vector4DF(0, 0, 0, 0) );

    _particleDirectory= dir;
    _format = format;
    clearGVDB();
    parseScene(_particleDirectory, _format);

    updateGVDB();

    return true;
}

void Sample::updateGVDB()
{
    nvdb::Vector3DF origin(0.0f, 0.0f, 0.0f);

    gvdb.RebuildTopology(_numParticles, _radius * 2.0f, origin);
    gvdb.FinishTopology(false, true);
    gvdb.UpdateAtlas();

    gvdb.ClearChannel(0);

    int scPntLen = 0;
	int subcell_size = 4;
	gvdb.InsertPointsSubcell_FP16 (subcell_size, _numParticles, _radius, origin, scPntLen);
	gvdb.GatherLevelSet_FP16 (subcell_size, _numParticles, _radius, origin, scPntLen, 0, 0);
	gvdb.UpdateApron(0, 3.0f);
    
    gvdb.Compute( FUNC_SMOOTH, 0, 0.0f, nvdb::Vector3DF(0.0f, 0.0f, 0.0f), true, true, 3.0f);		// 8x smooth iterations	

    std::vector<std::string> outlist;
	gvdb.MemoryUsage("gvdb", outlist);
	for (size_t n = 0; n < outlist.size(); n++)
		printf("%s\n", outlist[n].c_str());
    gvdb.Measure(true);
}

void Sample::parseScene(const std::string& dir, const std::string& format)
{
    char path[256];
    snprintf(path, sizeof(path), "frame_%06d.%s", _frame, format.c_str());
    std::string fileName = pystring::os::path::join(dir, path);

    Array1<Vector3D> positions;
    std::vector<float> data;
    constexpr float FLOAT_LOWER_BOUND = std::numeric_limits<float>::min();
    constexpr float FLOAT_UPPER_BOUND = std::numeric_limits<float>::max();
    nvdb::Vector3DF bbmin(FLOAT_UPPER_BOUND, FLOAT_UPPER_BOUND, FLOAT_UPPER_BOUND);
    nvdb::Vector3DF bbmax(FLOAT_LOWER_BOUND, FLOAT_LOWER_BOUND, FLOAT_LOWER_BOUND);
    if (format == "pos")
    {
        std::ifstream positionFile(fileName.c_str(), std::ifstream::binary);
        if (positionFile)
        {
            const std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(positionFile)), (std::istreambuf_iterator<char>()));
            Deserialize(buffer, &positions);
            positions.ForEach([&](const Vector3D& pos){
                Vector3D scaledPos = pos * 5.0;
                data.push_back(static_cast<float>(scaledPos.x));
                data.push_back(static_cast<float>(scaledPos.y));
                data.push_back(static_cast<float>(scaledPos.z));
                bbmin.x = std::min(bbmin.x, static_cast<float>(scaledPos.x));
                bbmin.y = std::min(bbmin.y, static_cast<float>(scaledPos.y));
                bbmin.z = std::min(bbmin.z, static_cast<float>(scaledPos.z));
                bbmax.x = std::max(bbmax.x, static_cast<float>(scaledPos.x));
                bbmax.y = std::max(bbmax.y, static_cast<float>(scaledPos.y));
                bbmax.z = std::max(bbmax.z, static_cast<float>(scaledPos.z));
            });
            positionFile.close();
        }
        else
        {
            printf("Cannot read file %s.\n", fileName.c_str());
            exit(EXIT_FAILURE);
        }
    }
    else if (format == "xyz")
    {
        std::ifstream positionFile(fileName.c_str());
        if (positionFile)
        {
            std::string line;
            register float x, y, z;
            while(std::getline(positionFile, line))
            {
                std::istringstream isstr(line);
                isstr >> x >> y >> z;
                data.push_back(x);
                data.push_back(y);
                data.push_back(z);
                bbmin.x = std::min(bbmin.x, x);
                bbmin.y = std::min(bbmin.y, y);
                bbmin.z = std::min(bbmin.z, z);
                bbmax.x = std::max(bbmax.x, x);
                bbmax.y = std::max(bbmax.y, y);
                bbmax.z = std::max(bbmax.z, z);
            }
            positionFile.close();
        }
        else
        {
            printf("Cannot read file %s.\n", fileName.c_str());
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("Unknown format %s.\n", format.c_str());
        exit(EXIT_FAILURE);
    }
    _numParticles = static_cast<int>(data.size() / 3);
    DataPtr temp;
    gvdb.AllocData(_original, _numParticles, sizeof(float) * 3, true);
    gvdb.AllocData(_particles, _numParticles, sizeof(float) * 3, false);
    std::memcpy(_original.cpu, data.data(), _numParticles * sizeof(float) * 3);
    gvdb.CommitData(_original);
    //!Vector3DF delta((bbmax.x - bbmin.x) / bbmax.x, (bbmax.y - bbmin.y) / bbmax.y, (bbmax.z - bbmin.z) / bbmax.z );
    gvdb.ConvertAndTransform(_original, 4, _particles, 4, _numParticles, bbmin, bbmax, nvdb::Vector3DF(0.0f, 0.0f, 0.0f), nvdb::Vector3DF(1.0f, 1.0f, 1.0f));
    gvdb.SetPoints(_particles, temp, temp);
}

void Sample::renderFrame()
{
    gvdb.getScene()->SetCrossSection(nvdb::Vector3DF(0.0f, 0.0f, 0.0f), nvdb::Vector3DF(0.0f, 0.0f, -1.0f));
    gvdb.Render(SHADE_LEVELSET, 0, 0);
    parseScene(_particleDirectory, _format);
    updateGVDB();
}

void Sample::renderToFile(const std::string& dir)
{
    char path[256];
    snprintf(path, sizeof(path), "rendered_frame_%06d.png", _frame);
    std::string fileName = pystring::os::path::join(dir, path);

    unsigned char* buffer = (unsigned char*)malloc(_width * _height * 4);
    gvdb.ReadRenderBuf(0, buffer);
    save_png(fileName.c_str(), buffer, _width, _height, 4);
    free(buffer);
}

void Sample::clearGVDB()
{
    nvdb::DataPtr temp;
    gvdb.SetPoints(temp, temp, temp);
    gvdb.CleanAux();
}

void Sample::setWindowSize(int width, int height)
{
    _width = width;
    _height = height;
}

int main(int argc, char* argv[])
{
    bool showHelp = false;

    std::string inputDirectory = "HybridLiquidSim_output";
    std::string outputDirectory;

    // Parsing
    auto parser =
        clara::Help(showHelp) |
        clara::Opt(inputDirectory, "inputFileName")
        ["-i"]["--input"]
        ("input obj file name") |
        clara::Opt(outputDirectory, "outputFileName")
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

    sample = std::make_shared<Sample>(600, 400);
    sample->initGVDB(inputDirectory, "pos");
    for (int i = 0; i < 62; ++i)
    {
        sample->renderFrame();
        sample->renderToFile("./Result");
        sample->advanceFrame();
    }
    
    //!if (inputDirectory.empty() || outputDirectory.empty())
    //!{
    //!    std::cout << ToString(parser) << '\n';
    //!    exit(EXIT_FAILURE);
    //!}

    return EXIT_SUCCESS;
}