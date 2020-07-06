/*************************************************************************
> File Name: GeometryCacheParser.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Fluid data parser (pos, xyz)
> Created Time: 2020/06/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Utils/GeometryCacheParser.h>
#include <Framework/Utils/FileSystem.h>
#include <Framework/Utils/Common.h>
#include <Framework/Utils/ObjReconstructor.h>
#include <Core/Utils/Serialization.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdio>

namespace CubbyFlow {
namespace CubbyRender {
	GeometryCacheParser::GeometryCacheParser()
	{
		//! Do nothing.
	}

	GeometryCacheParser::~GeometryCacheParser()
	{
		//! Do nothing.
	}

	void GeometryCacheParser::loadGeometryCache(const std::string& format, size_t count)
	{
		using namespace std::placeholders;
		size_t idx = format.find_last_of('.');
		const std::string& extension = format.substr(idx + 1);

		std::function<void(const std::string&)> loadMethod;
		if (extension == "xyz") loadMethod = std::bind(&GeometryCacheParser::loadXyzFile, this, std::placeholders::_1);
		else if (extension == "pos") loadMethod = std::bind(&GeometryCacheParser::loadPosFile, this, std::placeholders::_1);
		else if (extension == "obj") loadMethod = std::bind(&GeometryCacheParser::loadObjFile, this, std::placeholders::_1);
		else
		{
			CUBBYFLOW_ERROR << "Unknown geometry cache file extension [" << extension << "]";
			std::abort();
		}

		char baseName[256];
		for (size_t i = 0; i < count; ++i)
		{
			snprintf(baseName, sizeof(baseName), format.c_str(), i);
			loadMethod(baseName);
		}
	}

	ArrayAccessor1<float> GeometryCacheParser::getVertexCache(size_t index)
	{
		assert(index < _vertexCache.size());
		return _vertexCache[index].Accessor();
	}

    ConstArrayAccessor1<float> GeometryCacheParser::getVertexCache(size_t index) const
	{
		assert(index < _vertexCache.size());
		return _vertexCache[index].ConstAccessor();
	}

    ArrayAccessor1<unsigned int> GeometryCacheParser::getIndexCache(size_t index)
	{
		assert(index < _indexCache.size());
		return _indexCache[index].Accessor();
	}

    ConstArrayAccessor1<unsigned int> GeometryCacheParser::getIndexCache(size_t index) const
	{
		assert(index < _indexCache.size());
		return _indexCache[index].ConstAccessor();
	}

	size_t GeometryCacheParser::getNumberOfGeometryCache() const
	{
		return _vertexCache.size();
	}

	void GeometryCacheParser::loadPosFile(const std::string& path)
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
		_vertexCache.push_back(particles);
	    file.close();
	}

	void GeometryCacheParser::loadXyzFile(const std::string& path)
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
		_vertexCache.push_back(particles);
	    file.close();
	}

	void GeometryCacheParser::loadObjFile(const std::string& path)
	{
		ObjReconstructor reconstructor;
		reconstructor.setVertexFormat(VertexFormat::Position3Normal3);
		reconstructor.loadAndReconstruct(path);

		_vertexCache.push_back(reconstructor.getVertices());
		_indexCache.push_back(reconstructor.getIndices());
	}
};
};


