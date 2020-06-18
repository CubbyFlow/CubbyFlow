/*************************************************************************
> File Name: ParticleParser.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Fluid data parser (pos, xyz)
> Created Time: 2020/06/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Utils/ParticleParser.h>

#include <Framework/Utils/FileSystem.h>
#include <Framework/Utils/Common.h>
#include <Core/Utils/Serialization.h>
#include <fstream>
#include <sstream>
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {
	ParticleParser::ParticleParser()
	{
		//! Do nothing.
	}

	ParticleParser::~ParticleParser()
	{
		//! Do nothing.
	}

	bool ParticleParser::loadParticleFiles(const std::string& dir, const std::string& format)
	{
		std::vector<std::string> files;
		listFileNames(dir, format, &files);
		if (files.empty())
		{
			CUBBYFLOW_ERROR << "No fluid files detected with format [" << format << "]";
			return false;	
		}
		CUBBYFLOW_INFO << "#Loaded fluid files : " << files.size();
		std::sort(files.begin(), files.end());

		if (strcmp(format.c_str(), "pos") == 0)
		{
			_particleStates.clear();
			for (const auto& path : files)
			{
				loadPosFile(path);
			}
			return true;
		}
		else if (strcmp(format.c_str(), "xyz") == 0)
		{
			_particleStates.clear();
			for (const auto& path : files)
			{
				loadXyzFile(path);
			}
			return true;
		}
		else
		{
			CUBBYFLOW_ERROR << "Unknown file format [" << format << "]";
			return false;
		}
	}

	ArrayAccessor1<Vector3F> ParticleParser::getParticles(size_t index)
	{
		assert(index < _particleStates.size());
		return _particleStates[index].Accessor();
	}

	size_t ParticleParser::getNumberOfParticleState() const
	{
		return _particleStates.size();
	}

	void ParticleParser::loadPosFile(const std::string& path)
	{
		std::ifstream file(path, std::ifstream::binary);
		if (file.is_open() == false)
	    {
			CUBBYFLOW_ERROR << "Failed to load file [" << path << "]";
			return;
	    }
		Array1<Vector3D> dParticles;
	    const std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	    Deserialize(buffer, &dParticles);
		Array1<Vector3F> fParticles(dParticles.size());
		fParticles.ParallelForEachIndex([&](size_t index){
			fParticles[index] = dParticles[index].CastTo<float>();
		});
		_particleStates.push_back(fParticles);
	    file.close();
	}

	void ParticleParser::loadXyzFile(const std::string& path)
	{
		Array1<Vector3F> particles;
		std::ifstream file(path);
		if (file.is_open() == false)
		{
			CUBBYFLOW_ERROR << "Failed to load file [" << path << "]";
			return;
		}

		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream isstr(line);
			float x, y, z;
			isstr >> x >> y >> z;
			particles.Append(Vector3F(x, y, z));
		}
		_particleStates.push_back(particles);
	    file.close();
	}
};
};


