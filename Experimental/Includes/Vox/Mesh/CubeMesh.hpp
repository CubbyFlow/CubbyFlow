/*************************************************************************
> File Name: CubeMesh.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Mesh class for sharing resourches which have same vertices data.
> Created Time: 2021/01/10
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_CUBE_MESH_HPP
#define CUBBYFLOW_VOX_CUBE_MESH_HPP

#include <Vox/Mesh/Mesh.hpp>

namespace Vox {

    /**
     *  OpenGL Cube Mesh class
     */
    class CubeMesh : public Mesh
    {
    public:
        //! Constructor with geometry cache.
        CubeMesh(const float width, const float height, const float depth);
    };
}

#endif