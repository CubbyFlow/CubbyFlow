/*************************************************************************
> File Name: Mesh.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Mesh class for sharing resourches which have same vertices data.
> Created Time: 2020/08/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_MESH_HPP
#define CUBBYFLOW_VOX_MESH_HPP

#include <Vox/Utils/GLTypes.hpp>
#include <Vox/Core/Vertex.hpp>
#include <Vox/Core/NonCopyable.hpp>
#include <Core/Geometry/BoundingBox3.hpp>
#include <Core/Vector/Vector2.hpp>
#include <Core/Vector/Vector3.hpp>
#include <Core/Array/Array1.hpp>
#include <memory>


namespace Vox {

    class FrameContext;
    class Material;
    
    struct MeshShape 
    {
        CubbyFlow::Array1<float> interleaved;
        CubbyFlow::Array1<CubbyFlow::Vector3F> positions;
        CubbyFlow::Array1<CubbyFlow::Vector2F> texCoords;
        CubbyFlow::Array1<CubbyFlow::Vector3F> normals;
        CubbyFlow::Array1<unsigned int> indices;
        CubbyFlow::BoundingBox3F boundingBox;
        VertexFormat format { VertexFormat::None };
    };

    /**
     *  OpenGL Mesh class for sharing resourches which have same vertices data.
     */
    class Mesh : public NonCopyable
    {
    public:
        //! Default Constructor
        Mesh();
        //! Default destructor
        virtual ~Mesh();
        //! Set buffer storage usage
        void SetBufferUsage(GLuint usage);
        //! Generate opengl resources with given geometry shape.
        void GenerateMeshObject(const MeshShape& geometryShape, VertexFormat format, bool bInterleaved=true);
        //! Clear and Reset contained opengl resources.
        void ClearMeshObject();
        //! Draw mesh with configured opengl resources.
        void DrawMesh(const std::shared_ptr<FrameContext>& ctx);
        //! Asynchronously transfer new data ptr.
        //! But, set the fences need to be done outside the class.
        void AsyncTransfer(const void* src, const size_t numBytes, bool isIndices=false);
        //! Generate empty interleaved buffers
        void GenerateEmptyMesh(VertexFormat format, const size_t verticesBytes, const size_t indicesBytes);
    protected:
        GLuint _usage;
        GLuint _vao, _vbo, _ebo; 
        unsigned int _numVertices;
    };
}

#endif