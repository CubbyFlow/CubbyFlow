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

#include <Vox/GLTypes.hpp>
#include <Vox/Vertex.hpp>
#include <Vox/NonCopyable.hpp>
#include <Core/BoundingBox/BoundingBox3.hpp>
#include <Core/Vector/Vector2.hpp>
#include <Core/Vector/Vector3.hpp>
#include <Core/Array/Array1.hpp>
#include <memory>


namespace Vox {

    class FrameContext;
    
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
        //! Constructor with geometry cache.
        Mesh(const MeshShape& geometryShape, VertexFormat format, bool bInterleaved=false);
        //! Default destructor
        ~Mesh();

        //! Generate opengl resources with given geometry shape.
        void GenerateMeshObject(const MeshShape& geometryShape, VertexFormat format, bool bInterleaved=false);
        //! Clear and Reset contained opengl resources.
        void ClearMeshObject();
        //! Draw mesh with configured opengl resources.
        void DrawMesh(const std::shared_ptr<FrameContext>& ctx);

    protected:
        GLuint _vao, _vbo, _ebo; 
        unsigned int _numVertices;
    };
}

#endif