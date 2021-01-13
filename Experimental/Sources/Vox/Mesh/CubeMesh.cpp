/*************************************************************************
> File Name: CubeMesh.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Mesh class for sharing resourches which have same vertices data.
> Created Time: 2021/01/10
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Mesh/CubeMesh.hpp>
#include <Core/Vector/Vector2.hpp>
#include <Core/Vector/Vector3.hpp>

namespace Vox {

    CubeMesh::CubeMesh(const float width, const float height, const float depth)
    {
        MeshShape shape;

        const float halfWidth   = width  * 0.5f;
        const float halfHeight  = height * 0.5f;
        const float halfDepth   = depth  * 0.5f;

        shape.positions = {
            CubbyFlow::Vector3F(-halfWidth, -halfHeight, -halfDepth),
            CubbyFlow::Vector3F( halfWidth,  halfHeight, -halfDepth), 
            CubbyFlow::Vector3F( halfWidth, -halfHeight, -halfDepth), 
            CubbyFlow::Vector3F( halfWidth,  halfHeight, -halfDepth), 
            CubbyFlow::Vector3F(-halfWidth, -halfHeight, -halfDepth),
            CubbyFlow::Vector3F(-halfWidth,  halfHeight, -halfDepth),

            CubbyFlow::Vector3F(-halfWidth, -halfHeight,  halfDepth),
            CubbyFlow::Vector3F( halfWidth, -halfHeight,  halfDepth),
            CubbyFlow::Vector3F( halfWidth,  halfHeight,  halfDepth),
            CubbyFlow::Vector3F( halfWidth,  halfHeight,  halfDepth), 
            CubbyFlow::Vector3F(-halfWidth,  halfHeight,  halfDepth),
            CubbyFlow::Vector3F(-halfWidth, -halfHeight,  halfDepth),

            CubbyFlow::Vector3F(-halfWidth,  halfHeight,  halfDepth),
            CubbyFlow::Vector3F(-halfWidth,  halfHeight, -halfDepth),
            CubbyFlow::Vector3F(-halfWidth, -halfHeight, -halfDepth),
            CubbyFlow::Vector3F(-halfWidth, -halfHeight, -halfDepth),
            CubbyFlow::Vector3F(-halfWidth, -halfHeight,  halfDepth),
            CubbyFlow::Vector3F(-halfWidth,  halfHeight,  halfDepth),

            CubbyFlow::Vector3F( halfWidth,  halfHeight,  halfDepth), 
            CubbyFlow::Vector3F( halfWidth, -halfHeight, -halfDepth), 
            CubbyFlow::Vector3F( halfWidth,  halfHeight, -halfDepth), 
            CubbyFlow::Vector3F( halfWidth, -halfHeight, -halfDepth), 
            CubbyFlow::Vector3F( halfWidth,  halfHeight,  halfDepth), 
            CubbyFlow::Vector3F( halfWidth, -halfHeight,  halfDepth), 

            CubbyFlow::Vector3F(-halfWidth, -halfHeight, -halfDepth),
            CubbyFlow::Vector3F( halfWidth, -halfHeight, -halfDepth),
            CubbyFlow::Vector3F( halfWidth, -halfHeight,  halfDepth),
            CubbyFlow::Vector3F( halfWidth, -halfHeight,  halfDepth), 
            CubbyFlow::Vector3F(-halfWidth, -halfHeight,  halfDepth),
            CubbyFlow::Vector3F(-halfWidth, -halfHeight, -halfDepth),

            CubbyFlow::Vector3F(-halfWidth,  halfHeight, -halfDepth),
            CubbyFlow::Vector3F( halfWidth,  halfHeight,  halfDepth), 
            CubbyFlow::Vector3F( halfWidth,  halfHeight, -halfDepth), 
            CubbyFlow::Vector3F( halfWidth,  halfHeight,  halfDepth), 
            CubbyFlow::Vector3F(-halfWidth,  halfHeight, -halfDepth),
            CubbyFlow::Vector3F(-halfWidth,  halfHeight,  halfDepth),
        };
        shape.texCoords = {
            CubbyFlow::Vector2F(0.0f, 0.0f),
            CubbyFlow::Vector2F(1.0f, 1.0f),
            CubbyFlow::Vector2F(1.0f, 0.0f),
            CubbyFlow::Vector2F(1.0f, 1.0f),
            CubbyFlow::Vector2F(0.0f, 0.0f),
            CubbyFlow::Vector2F(0.0f, 1.0f),

            CubbyFlow::Vector2F(0.0f, 0.0f),
            CubbyFlow::Vector2F(1.0f, 0.0f),
            CubbyFlow::Vector2F(1.0f, 1.0f),
            CubbyFlow::Vector2F(1.0f, 1.0f),
            CubbyFlow::Vector2F(0.0f, 1.0f),
            CubbyFlow::Vector2F(0.0f, 0.0f),

            CubbyFlow::Vector2F(1.0f, 0.0f),
            CubbyFlow::Vector2F(1.0f, 1.0f),
            CubbyFlow::Vector2F(0.0f, 1.0f),
            CubbyFlow::Vector2F(0.0f, 1.0f),
            CubbyFlow::Vector2F(0.0f, 0.0f),
            CubbyFlow::Vector2F(1.0f, 0.0f),

            CubbyFlow::Vector2F(1.0f, 0.0f),
            CubbyFlow::Vector2F(0.0f, 1.0f),
            CubbyFlow::Vector2F(1.0f, 1.0f),
            CubbyFlow::Vector2F(0.0f, 1.0f),
            CubbyFlow::Vector2F(1.0f, 0.0f),
            CubbyFlow::Vector2F(0.0f, 0.0f),

            CubbyFlow::Vector2F(0.0f, 1.0f),
            CubbyFlow::Vector2F(1.0f, 1.0f),
            CubbyFlow::Vector2F(1.0f, 0.0f),
            CubbyFlow::Vector2F(1.0f, 0.0f),
            CubbyFlow::Vector2F(0.0f, 0.0f),
            CubbyFlow::Vector2F(0.0f, 1.0f),

            CubbyFlow::Vector2F(0.0f, 1.0f),
            CubbyFlow::Vector2F(1.0f, 0.0f),
            CubbyFlow::Vector2F(1.0f, 1.0f),
            CubbyFlow::Vector2F(1.0f, 0.0f),
            CubbyFlow::Vector2F(0.0f, 1.0f),
            CubbyFlow::Vector2F(0.0f, 0.0f),
        };
        shape.normals = {
            CubbyFlow::Vector3F( 0.0f,  0.0f, -1.0f),
            CubbyFlow::Vector3F( 0.0f,  0.0f, -1.0f),
            CubbyFlow::Vector3F( 0.0f,  0.0f, -1.0f),
            CubbyFlow::Vector3F( 0.0f,  0.0f, -1.0f),
            CubbyFlow::Vector3F( 0.0f,  0.0f, -1.0f),
            CubbyFlow::Vector3F( 0.0f,  0.0f, -1.0f),

            CubbyFlow::Vector3F( 0.0f,  0.0f,  1.0f),
            CubbyFlow::Vector3F( 0.0f,  0.0f,  1.0f),
            CubbyFlow::Vector3F( 0.0f,  0.0f,  1.0f),
            CubbyFlow::Vector3F( 0.0f,  0.0f,  1.0f),
            CubbyFlow::Vector3F( 0.0f,  0.0f,  1.0f),
            CubbyFlow::Vector3F( 0.0f,  0.0f,  1.0f),

            CubbyFlow::Vector3F(-1.0f,  0.0f,  0.0f),
            CubbyFlow::Vector3F(-1.0f,  0.0f,  0.0f),
            CubbyFlow::Vector3F(-1.0f,  0.0f,  0.0f),
            CubbyFlow::Vector3F(-1.0f,  0.0f,  0.0f),
            CubbyFlow::Vector3F(-1.0f,  0.0f,  0.0f),
            CubbyFlow::Vector3F(-1.0f,  0.0f,  0.0f),

            CubbyFlow::Vector3F( 1.0f,  0.0f,  0.0f),
            CubbyFlow::Vector3F( 1.0f,  0.0f,  0.0f),
            CubbyFlow::Vector3F( 1.0f,  0.0f,  0.0f),
            CubbyFlow::Vector3F( 1.0f,  0.0f,  0.0f),
            CubbyFlow::Vector3F( 1.0f,  0.0f,  0.0f),
            CubbyFlow::Vector3F( 1.0f,  0.0f,  0.0f),

            CubbyFlow::Vector3F( 0.0f, -1.0f,  0.0f),
            CubbyFlow::Vector3F( 0.0f, -1.0f,  0.0f),
            CubbyFlow::Vector3F( 0.0f, -1.0f,  0.0f),
            CubbyFlow::Vector3F( 0.0f, -1.0f,  0.0f),
            CubbyFlow::Vector3F( 0.0f, -1.0f,  0.0f),
            CubbyFlow::Vector3F( 0.0f, -1.0f,  0.0f),

            CubbyFlow::Vector3F( 0.0f,  1.0f,  0.0f),
            CubbyFlow::Vector3F( 0.0f,  1.0f,  0.0f),
            CubbyFlow::Vector3F( 0.0f,  1.0f,  0.0f),
            CubbyFlow::Vector3F( 0.0f,  1.0f,  0.0f),
            CubbyFlow::Vector3F( 0.0f,  1.0f,  0.0f),
            CubbyFlow::Vector3F( 0.0f,  1.0f,  0.0f),
        };
        
        GenerateMeshObject(shape, VertexFormat::Position3Normal3TexCoord2, false);
    }
}