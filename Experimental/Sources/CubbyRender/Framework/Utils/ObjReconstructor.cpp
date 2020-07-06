/*************************************************************************
> File Name: ObjReconstructor.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Several Conversion functions such as tinyobj to vertices with alignment packed.
            reconstruction function largely referenced on tinyobj example code.
> Created Time: 2020/04/04
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Utils/ObjReconstructor.h>
#include <Framework/Utils/VectorCalculation.h>
#include <Framework/Utils/Common.h>
#include <Framework/Buffer/Vertex.h>
#include <Framework/Renderer/Renderer.h>
#include <Core/Utils/Timer.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <cassert>
#include <cmath>
#include <array>
#include <set>

namespace CubbyFlow {
namespace CubbyRender {

    ObjReconstructor::ObjReconstructor()
    {
        //! Do nothing.
    }

    ObjReconstructor::ObjReconstructor(const std::string& objPath, VertexFormat format)
        : _format(format)
    {
        bool result = loadAndReconstruct(objPath);
        if (!result)
        {
            std::abort();
        }
    }

    ObjReconstructor::~ObjReconstructor()
    {
        //! Do nothing.
    }

    void ObjReconstructor::setVertexFormat(VertexFormat format)
    {
        _format = format;
    }

    VertexFormat ObjReconstructor::getVertexFormat() const
    {
        return _format;
    }

    const Array1<float>& ObjReconstructor::getVertices() const
    {
        return _vertices;
    }

    const Array1<unsigned int>& ObjReconstructor::getIndices() const
    {
        return _indices;
    }

    std::tuple<Vector3F, Vector3F> ObjReconstructor::getBoundingBox() const
    {
        return std::make_tuple(_bbMin, _bbMax);
    }

    namespace //! Local utility functions.
    {
        inline bool hasSmoothingGroup(const tinyobj::shape_t& shape)
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
    
        void computeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape, std::map<int, Vector3F>& smoothVertexNormals) 
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
                Vector3F normal = calculateNormal(position[0], position[1], position[2]);
            
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

        constexpr float EPSILON = 1e-9f;

        struct Face
        {
            Vector3D position;
            Vector3D normal;
            Vector2D texCoord;
            size_t index; 
        };

        struct FaceComp
        {
            //! lexicographically sorting vector.
            inline bool operator() (const Face& face1, const Face& face2) const
            {
                for (size_t i = 0; i < 3; ++i)
                    if (std::fabs(face1.position[i] - face2.position[i]) > EPSILON)
                        return face1.position[i] < face2.position[i];
                for (size_t i = 0; i < 3; ++i)
                    if (std::fabs(face1.normal[i] - face2.normal[i]) > EPSILON)
                        return face1.normal[i] < face2.normal[i];
                for (size_t i = 0; i < 2; ++i)
                    if (std::fabs(face1.texCoord[i] - face2.texCoord[i]) > EPSILON)
                        return face1.texCoord[i] < face2.texCoord[i];
                return false;
            }
        };
    };

    bool ObjReconstructor::loadAndReconstruct(const std::string& objPath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string warn;
        std::string err;

        Timer timer;
        //! Load obj file with tinyobjloader 
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objPath.c_str());
        if (!warn.empty()) 
        {
            CUBBYFLOW_ERROR << warn;
        }
        if (!err.empty()) 
        {
            CUBBYFLOW_ERROR << err;
        }
        if (!ret) 
        {
            CUBBYFLOW_ERROR << "LoadObj failed";
            return false;
        }
        CUBBYFLOW_INFO << "tinyobj::LoadObj with obj file [" << objPath << "] took " << timer.DurationInSeconds() << " seconds";
        timer.Reset();
        
        //! Reset bounding box corners.
        _bbMin[0] = _bbMin[1] = _bbMin[2] = std::numeric_limits<float>::max();
        _bbMax[0] = _bbMax[1] = _bbMax[2] = -std::numeric_limits<float>::max();
        
        //std::set<Face, FaceComp> faces;
        //size_t index { 0 };

        for (auto& shape : shapes)
        {
            std::map<int, Vector3F> smoothVertexNormals;
            if (static_cast<int>(_format & VertexFormat::Position3))
            {
                if (hasSmoothingGroup(shape)) 
                {
                    computeSmoothingNormals(attrib, shape, smoothVertexNormals);
                    CUBBYFLOW_INFO << objPath << " has smoothing group. computing smoothing normals conducted";
                }
            }

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
                Vector3F normal[3];
                Vector2F texCoord[3];

                //! When Position3 is included in format.
                if (static_cast<int>(_format & VertexFormat::Position3))
                {
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
                        _bbMin[k] = std::min(position[0][k], _bbMin[k]);
                        _bbMin[k] = std::min(position[1][k], _bbMin[k]);
                        _bbMin[k] = std::min(position[2][k], _bbMin[k]);
                        _bbMax[k] = std::max(position[0][k], _bbMax[k]);
                        _bbMax[k] = std::max(position[1][k], _bbMax[k]);
                        _bbMax[k] = std::max(position[2][k], _bbMax[k]);
                    }
                }

                //! When Normal3 is included in format.
                if (static_cast<int>(_format & VertexFormat::Normal3))
                {
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

                    if (invalidNormal && static_cast<int>(_format & VertexFormat::Position3)) 
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
                            normal[0] = calculateNormal(position[0], position[1], position[2]);
                            normal[1] = normal[0];
                            normal[2] = normal[0];
                        }    
                    }
                }

                //! When TexCoord2 is included in format.
                if (static_cast<int>(_format & VertexFormat::TexCoord2))
                {
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
                }

                //! From now on, vertices in one face allocated.
                for (size_t k = 0; k < 3; ++k)
                {
                    if (static_cast<int>(_format & VertexFormat::Position3))
                    {
                        _vertices.Append(position[k].x);
                        _vertices.Append(position[k].y);
                        _vertices.Append(position[k].z);
                    }
                    if (static_cast<int>(_format & VertexFormat::Normal3))
                    {
                        _vertices.Append(normal[k].x);
                        _vertices.Append(normal[k].y);
                        _vertices.Append(normal[k].z);
                    }
                    if (static_cast<int>(_format & VertexFormat::TexCoord2))
                    {
                        _vertices.Append(texCoord[k].x);
                        _vertices.Append(texCoord[k].y);
                    }
                    //auto iter = faces.find({position[k], normal[k], texCoord[k], size_t(0)});
                    //if (iter == faces.end())
                    //{
                    //    faces.insert({ position[k], normal[k], texCoord[k], index });
                    //    _indices.Append(static_cast<unsigned int>(index++));
                    //}
                    //else
                    //{
                    //    _indices.Append(static_cast<unsigned int>(iter->index));
                    //}
                }
            }
        }

        //! std::set<Face, FaceComp> faces to std::vector<float> _vertices.
        //_vertices.Resize(faces.size() * VertexHelper::getNumberOfFloats(_format));
        //for (const auto& face : faces)
        //{
        //    const size_t index = face.index * VertexHelper::getNumberOfFloats(_format);
        //    if (static_cast<int>(_format & VertexFormat::Position3))
        //    {
        //        _vertices.At(  index  ) = face.position.x;
        //        _vertices.At(index + 1) = face.position.y;
        //        _vertices.At(index + 2) = face.position.z;
        //    }
        //    if (static_cast<int>(_format & VertexFormat::Normal3))
        //    {
        //        _vertices.At(index + 3) = face.normal.x;
        //        _vertices.At(index + 4) = face.normal.y;
        //        _vertices.At(index + 5) = face.normal.z;
        //    }
        //    if (static_cast<int>(_format & VertexFormat::TexCoord2))
        //    {
        //        _vertices.At(index + 6) = face.texCoord.x;
        //        _vertices.At(index + 7) = face.texCoord.y;
        //    }
        //}

        CUBBYFLOW_INFO << "Reconstruction Vertices with #vertex " << attrib.vertices.size() << " took " << timer.DurationInSeconds() << " seconds";
        return true;
    }
}
}