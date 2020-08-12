/*************************************************************************
> File Name: GeometryCache.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Several Conversion functions such as tinyobj to vertices with alignment packed.
            reconstruction function largely referenced on tinyobj example code.
> Created Time: 2020/08/11
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/GeometryCache.hpp>
#include <Vox/DebugUtils.hpp>
#include <Vox/VectorUtils.hpp>
#include <Core/Utils/Serialization.h>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <array>
#include <functional>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>



using namespace CubbyFlow;
namespace Vox {

    namespace Detail
    {
        inline bool HasSmoothingGroup(const tinyobj::shape_t& shape)
        {
            for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++) 
            {
                if (shape.mesh.smoothing_group_ids[i] > 0) 
                {
                    return true;
                }
            }
            return false;
        }
    
        void ComputeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape, std::map<int, Vector3F>& smoothVertexNormals) 
        {
            smoothVertexNormals.clear();
            const auto& vertices = attrib.vertices;

            for (size_t faceIndex = 0; faceIndex < shape.mesh.indices.size() / 3; faceIndex++) 
            {
                // Get the three indexes of the face (all faces are triangular)
                tinyobj::index_t idx0 = shape.mesh.indices[3 * faceIndex + 0];
                tinyobj::index_t idx1 = shape.mesh.indices[3 * faceIndex + 1];
                tinyobj::index_t idx2 = shape.mesh.indices[3 * faceIndex + 2];
            
                // Get the three vertex indexes and coordinates
                Vector3F position[3];  // coordinates

                int f0 = idx0.vertex_index;
                int f1 = idx1.vertex_index;
                int f2 = idx2.vertex_index;
                assert(f0 >= 0 && f1 >= 0 && f2 >= 0);

                position[0] = Vector3F(vertices[3 * f0], vertices[3 * f0 + 1], vertices[3 * f0 + 2]);
                position[1] = Vector3F(vertices[3 * f1], vertices[3 * f1 + 1], vertices[3 * f1 + 2]);
                position[2] = Vector3F(vertices[3 * f2], vertices[3 * f2 + 1], vertices[3 * f2 + 2]);
            
                // Compute the normal of the face
                Vector3F normal = CalculateNormal(position[0], position[1], position[2]);
            
                // Add the normal to the three vertexes
                int faces[3] = {f0, f1, f2};
                for (size_t i = 0; i < 3; ++i) 
                {
                    auto iter = smoothVertexNormals.find(faces[i]);
                    if (iter == smoothVertexNormals.end())
                    {
                        smoothVertexNormals[faces[i]] = normal;
                    }
                    else
                    {
                        iter->second += normal;
                    }
                }
            
            }  // f
            
            // Normalize the normals, that is, make them unit vectors
            for (auto p : smoothVertexNormals) 
            {
                p.second.Normalize();
            }
        }

        constexpr float EPSILON = 1e-6f;

        struct PackedVertex
        {
            Vector3F position;
            Vector2F texCoord;
            Vector3F normal;

            PackedVertex(Vector3F pos, Vector2F uv, Vector3F n)
                : position(pos), texCoord(uv), normal(n) {};
        };

        //! lexicographically sorting vector.
        inline bool operator<(const PackedVertex& v1, const PackedVertex& v2)
        {
            if (std::fabs(v1.position.x - v2.position.x) < EPSILON) return v1.position.x < v2.position.x;
            if (std::fabs(v1.position.y - v2.position.y) < EPSILON) return v1.position.y < v2.position.y;
            if (std::fabs(v1.position.z - v2.position.z) < EPSILON) return v1.position.z < v2.position.z;
            if (std::fabs(v1.texCoord.x - v2.texCoord.x) < EPSILON) return v1.texCoord.x < v2.texCoord.x;
            if (std::fabs(v1.texCoord.y - v2.texCoord.y) < EPSILON) return v1.texCoord.y < v2.texCoord.y;
            if (std::fabs( v1.normal.x  -  v2.normal.x ) < EPSILON) return v1.normal.x < v2.normal.x;
            if (std::fabs( v1.normal.y  -  v2.normal.y ) < EPSILON) return v1.normal.y < v2.normal.y;
            if (std::fabs( v1.normal.z  -  v2.normal.z ) < EPSILON) return v1.normal.z < v2.normal.z;
            return false;
        }
    };

    GeometryCache::GeometryCache()
    {
        _boundingBox.Reset();
    }

    GeometryCache::GeometryCache(const Vox::Path& format, size_t index)
        : GeometryCache()
    {
        LoadCache(format, index);
    }

    GeometryCache::~GeometryCache()
    {
        //! Do nothing.
    }

    void GeometryCache::LoadCache(const Vox::Path& format, size_t index)
    {
        const std::string formatStr = format.ToString();
        const size_t idx = formatStr.find_last_of('.');
		const std::string& extension = formatStr.substr(idx + 1);

		char baseName[256];
		snprintf(baseName, sizeof(baseName), format.ToString().c_str(), index);

		if (extension == "xyz") LoadXyzCache(baseName);
		else if (extension == "pos") LoadPosCache(baseName);
        else if (extension == "obj") LoadObjCache(baseName);
		else VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown Particle File Extension [" + extension + "]");
    }

    size_t GeometryCache::GetNumberOfShape() const
    {
        return _shapes.size();
    }

    const Vox::GeometryCache::Shape& GeometryCache::GetShape(size_t index) const
    {
        VoxAssert((index < _shapes.size()), CURRENT_SRC_PATH_TO_STR,"Out Of Range Error");
        return _shapes[index];
    }

    void GeometryCache::LoadPosCache(const Vox::Path& path)
	{
		std::ifstream file(path.ToString(), std::ifstream::binary);
		VoxAssert(file.is_open(), CURRENT_SRC_PATH_TO_STR, "Failed to load file [" + path.ToString() + "]");

		Array1<Vector3D> tempParticles;
	    const std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	    Deserialize(buffer, &tempParticles);
	    file.close();
		
        Vox::GeometryCache::Shape newShape;
        newShape.format = VertexFormat::Position3;
		newShape.vertices.Resize(tempParticles.size() * 3);
		tempParticles.ParallelForEachIndex([&](size_t index){
			const size_t baseIndex = index * 3;
            const auto& particle = tempParticles[index].CastTo<float>();
			newShape.vertices[  baseIndex  ] = particle.x;
			newShape.vertices[baseIndex + 1] = particle.y;
			newShape.vertices[baseIndex + 2] = particle.z;
			
            newShape.boundingBox.Merge(particle);
		});
        _shapes.Append(newShape);
	}

	void GeometryCache::LoadXyzCache(const Vox::Path& path)
	{
		std::ifstream file(path.ToString());
		VoxAssert(file.is_open(), CURRENT_SRC_PATH_TO_STR, "Failed to load file [" + path.ToString() + "]");

        Vox::GeometryCache::Shape newShape;
        newShape.format = VertexFormat::Position3;
		std::string line;
		register float x, y, z;
		while (std::getline(file, line))
		{
			std::istringstream isstr(line);
			isstr >> x >> y >> z;
			newShape.vertices.Append(x);
			newShape.vertices.Append(y);
			newShape.vertices.Append(z);
			newShape.boundingBox.Merge(CubbyFlow::Vector3F(x, y, z));
		}
	    file.close();
        _shapes.Append(newShape);
	}

    void GeometryCache::LoadObjCache(const Vox::Path& path)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        //! Load obj file with tinyobjloader 
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.ToString().c_str());
        VoxAssert(warn.empty(), CURRENT_SRC_PATH_TO_STR, warn);
        VoxAssert(err.empty(), CURRENT_SRC_PATH_TO_STR, warn);
        VoxAssert(ret, CURRENT_SRC_PATH_TO_STR, "LoadObj [" + path.ToString() + "] Failed");
        VoxAssert(shapes.size() >= 1, CURRENT_SRC_PATH_TO_STR, "Loaded Obj have no shape");
        
        _shapes.Reserve(shapes.size());
        for (auto& shape : shapes)
        {
            std::map<int, Vector3F> smoothVertexNormals;
            if (Detail::HasSmoothingGroup(shape)) 
            {
                Detail::ComputeSmoothingNormals(attrib, shape, smoothVertexNormals);
            }

            Vox::GeometryCache::Shape newShape;
            if (!attrib.vertices.empty())   newShape.format |= VertexFormat::Position3;
            if (!attrib.normals.empty())    newShape.format |= VertexFormat::Normal3;
            if (!attrib.texcoords.empty())  newShape.format |= VertexFormat::TexCoord2;
            newShape.boundingBox.Reset();
            newShape.vertices.Reserve(shape.mesh.indices.size() * VertexHelper::GetNumberOfFloats(newShape.format));
            newShape.indices.Reserve(shape.mesh.indices.size());

            std::map<Detail::PackedVertex, unsigned int> packedVerticesMap;
            for (size_t faceIndex = 0; faceIndex < shape.mesh.indices.size() / 3; ++faceIndex)
            {
                /*
                idx0 (pos (float3), normal(float3), texcoords(float2))
                |\
                | \
                |  \
                |   \ idx2 (pos (float3), normal(float3), texcoords(float2))
                |   /
                |  /
                | /
                |/
                idx1 (pos (float3), normal(float3), texcoords(float2))
                */
                tinyobj::index_t idx0 = shape.mesh.indices[3 * faceIndex + 0];
                tinyobj::index_t idx1 = shape.mesh.indices[3 * faceIndex + 1];
                tinyobj::index_t idx2 = shape.mesh.indices[3 * faceIndex + 2];

                Vector3F position[3];
                Vector2F texCoord[3];
                Vector3F normal[3];

                for (int k = 0; k < 3; k++) 
                {
                    int f0 = idx0.vertex_index;
                    int f1 = idx1.vertex_index;
                    int f2 = idx2.vertex_index;
                    assert(f0 >= 0 && f1 >= 0 && f2 >= 0);

                    position[0][k] = attrib.vertices[3 * f0 + k];
                    position[1][k] = attrib.vertices[3 * f1 + k];
                    position[2][k] = attrib.vertices[3 * f2 + k];
                    //! Find bounding box lefttop and rightbottom corner.
                    newShape.boundingBox.Merge(position[k]);
                }

                bool invalidNormal = false;
                if (attrib.normals.size() > 0)
                {
                    int f0 = idx0.normal_index;
                    int f1 = idx1.normal_index;
                    int f2 = idx2.normal_index;
                    if (f0 < 0 || f1 < 0 || f2 < 0)
                    {
                        invalidNormal = true;
                    }
                    else
                    {
                        for (size_t k = 0; k < 3; k++)
                        {
                            assert(size_t(3 * f0 + k) < attrib.normals.size());
                            assert(size_t(3 * f1 + k) < attrib.normals.size());
                            assert(size_t(3 * f2 + k) < attrib.normals.size());
                            normal[0][k] = attrib.normals[3 * f0 + k];
                            normal[1][k] = attrib.normals[3 * f1 + k];
                            normal[2][k] = attrib.normals[3 * f2 + k];
                        }
                    }
                }
                else
                {
                    invalidNormal = true;
                }
                if (invalidNormal) 
                {
                    if (!smoothVertexNormals.empty())
                    {
                        //! Use smoothing normals
                        int f0 = idx0.vertex_index;
                        int f1 = idx1.vertex_index;
                        int f2 = idx2.vertex_index;
                        if (f0 >= 0 && f1 >= 0 && f2 >= 0) 
                        {
                            normal[0] = smoothVertexNormals[f0];
                            normal[1] = smoothVertexNormals[f1];
                            normal[2] = smoothVertexNormals[f2];
                        }
                    }
                    else
                    {
                        normal[0] = CalculateNormal(position[0], position[1], position[2]);
                        normal[1] = normal[0];
                        normal[2] = normal[0];
                    }    
                }

                if (attrib.texcoords.size() > 0)
                {
                    int f0 = idx0.texcoord_index;
                    int f1 = idx1.texcoord_index;
                    int f2 = idx2.texcoord_index;

                    if (f0 < 0 || f1 < 0 || f2 < 0)
                    {
                        texCoord[0] = Vector2F(0.0f, 0.0f);
                        texCoord[1] = Vector2F(0.0f, 0.0f);
                        texCoord[2] = Vector2F(0.0f, 0.0f);
                    }
                    else
                    {
                        assert(attrib.texcoords.size() > size_t(2 * f0 + 1));
                        assert(attrib.texcoords.size() > size_t(2 * f1 + 1));
                        assert(attrib.texcoords.size() > size_t(2 * f2 + 1));

                        //! Flip Y coord.
                        texCoord[0] = Vector2F(attrib.texcoords[2 * f0], 1.0f - attrib.texcoords[2 * f0 + 1]);
                        texCoord[1] = Vector2F(attrib.texcoords[2 * f1], 1.0f - attrib.texcoords[2 * f1 + 1]);
                        texCoord[2] = Vector2F(attrib.texcoords[2 * f2], 1.0f - attrib.texcoords[2 * f2 + 1]);
                    }
                }
                else
                {
                    texCoord[0] = Vector2F(0.0f, 0.0f);
                    texCoord[1] = Vector2F(0.0f, 0.0f);
                    texCoord[2] = Vector2F(0.0f, 0.0f);
                }

                //! From now on, vertices in one face allocated.
                for (unsigned int k = 0; k < 3; ++k)
                {
                    const unsigned int index = 3 * static_cast<unsigned int>(faceIndex) + k;
                    Detail::PackedVertex vertex(position[k], texCoord[k], normal[k]);
                    
                    auto iter = packedVerticesMap.find(vertex);

                    if (iter == packedVerticesMap.end())
                    {
                        if (static_cast<int>(newShape.format & VertexFormat::Position3))
                        {
                            newShape.vertices.Append(position[k].x);
                            newShape.vertices.Append(position[k].y);
                            newShape.vertices.Append(position[k].z);
                        }
                        if (static_cast<int>(newShape.format & VertexFormat::Normal3))
                        {
                            newShape.vertices.Append(normal[k].x);
                            newShape.vertices.Append(normal[k].y);
                            newShape.vertices.Append(normal[k].z);
                        }
                        if (static_cast<int>(newShape.format & VertexFormat::TexCoord2))
                        {
                            newShape.vertices.Append(texCoord[k].x);
                            newShape.vertices.Append(texCoord[k].y);
                        }
                        unsigned int newIndex = static_cast<unsigned int>(newShape.vertices.size() - 1);
                        packedVerticesMap.emplace(vertex, newIndex);
                        newShape.indices.Append(newIndex);
                    }
                    else
                    {
                        newShape.indices.Append(index);
                    }
                }
            }

            _shapes.Append(newShape);
            _boundingBox.Merge(newShape.boundingBox);
        }
    }

    const CubbyFlow::BoundingBox3F& GeometryCache::GetBoundingBox() const
    {
        return _boundingBox;
    }
}