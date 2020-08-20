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

#include <Vox/GeometryCache.hpp>
#include <Vox/GLTypes.hpp>
#include <memory>

namespace Vox {

    class FrameContext;
    /**
     *  OpenGL Mesh class for sharing resourches which have same vertices data.
     */
    class Mesh
    {
    public:
        //! Default constructor
        Mesh();
        //! Constructor with geometry cache.
        Mesh(const GeometryCache::Shape& geometryShape, VertexFormat format, bool bInterleaved=false);
        //! Default destructor
        ~Mesh();

        //! Generate opengl resources with given geometry shape.
        void GenerateMeshObject(const GeometryCache::Shape& geometryShape, VertexFormat format, bool bInterleaved=false);
        //! Clear and Reset contained opengl resources.
        void ClearMeshObject();
        //! Draw mesh with configured opengl resources.
        void DrawMesh(const std::shared_ptr<FrameContext>& ctx);

    private:
        GLuint _vao, _vbo, _ebo;
        unsigned int _numVertices;
    };
}

#endif