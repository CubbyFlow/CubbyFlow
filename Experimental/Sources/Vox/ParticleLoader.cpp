/*************************************************************************
> File Name: GeometryCacheParser.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Fluid data parser (pos, xyz)
> Created Time: 2020/06/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/ParticleLoader.hpp>
#include <Core/Utils/Logging.h>
#include <Core/Utils/Serialization.h>
#include <Core/Vector/Vector3.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdio>
#include <functional>

using namespace CubbyFlow;

namespace Vox {
	ParticleLoader::ParticleLoader()
	{
		//! Do nothing.
	}

	ParticleLoader::~ParticleLoader()
	{
		//! Do nothing.
	}

	void ParticleLoader::LoadParticles(const std::string& format, size_t count)
	{
		using namespace std::placeholders;
		size_t idx = format.find_last_of('.');
		const std::string& extension = format.substr(idx + 1);

		std::function<void(const std::string&)> loadMethod;
		if (extension == "xyz") loadMethod = std::bind(&ParticleLoader::LoadXyzFile, this, std::placeholders::_1);
		else if (extension == "pos") loadMethod = std::bind(&ParticleLoader::LoadPosFile, this, std::placeholders::_1);
		else
		{
			CUBBYFLOW_ERROR << "Unknown Particle File extension [" << extension << "]";
			std::abort();
		}

		char baseName[256];
		for (size_t i = 0; i < count; ++i)
		{
			snprintf(baseName, sizeof(baseName), format.c_str(), i);
			loadMethod(baseName);
		}
	}

	size_t ParticleLoader::GetNumberOfFrame() const
	{
		return _frames.size();
	}

    size_t ParticleLoader::GetNumberOfBytes(size_t frameIndex) const
	{
		assert(frameIndex < _frames.size());
		return _frames[frameIndex].size() * sizeof(float);
	}
    void ParticleLoader::CopyParticleData(void* dst, size_t frameIndex) const
	{
		assert(frameIndex < _frames.size());
		const void* src = static_cast<const void*>(_frames[frameIndex].data());
		std::memcpy(dst, src, GetNumberOfBytes(frameIndex));
	}

	void ParticleLoader::LoadPosFile(const std::string& path)
	{
		std::ifstream file(path, std::ifstream::binary);
		if (file.is_open() == false)
	    {
			CUBBYFLOW_ERROR << "Failed to load file [" << path << "]";
			return;
	    }
		Array1<Vector3D> tempParticles;
	    const std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	    Deserialize(buffer, &tempParticles);
		
		const size_t numElements = tempParticles.size() * 3;
		Array1<float> particles(numElements);
		tempParticles.ParallelForEachIndex([&](size_t index){
			const size_t baseIndex = index * 3;
			particles[  baseIndex  ] = static_cast<float>(tempParticles[index].x);
			particles[baseIndex + 1] = static_cast<float>(tempParticles[index].y);
			particles[baseIndex + 2] = static_cast<float>(tempParticles[index].z);
		});
		_frames.push_back(particles);
	    file.close();
	}

	void ParticleLoader::LoadXyzFile(const std::string& path)
	{
		std::ifstream file(path);
		if (file.is_open() == false)
		{
			CUBBYFLOW_ERROR << "Failed to load file [" << path << "]";
			return;
		}

		Array1<float> particles;
		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream isstr(line);
			float x, y, z;
			isstr >> x >> y >> z;
			particles.Append(x);
			particles.Append(y);
			particles.Append(z);
		}
		_frames.push_back(particles);
	    file.close();
	}
};


