/*************************************************************************
> File Name: Vertex.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Vertex formats and Vertex Structures.
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_VERTEX_HPP
#define CUBBYFLOW_VOX_VERTEX_HPP

#include <Core/Matrix/Matrix.h>
#include <vector>
#include <cstdint>

namespace Vox {

    //! Vertex format enums.
    enum class VertexFormat 
    {
        //! None
        None = 0,
        
        //! Position in 3D.
        Position3 = 1 << 0,

        //! Normal in 3D.
        Normal3 = 1 << 1,

        //! Texture coordinates in 2D.
        TexCoord2 = 1 << 2,

        //! Texture coordinates in 3D.
        TexCoord3 = 1 << 3,

        //! Color in RGBA (4D).
        Color4 = 1 << 4,

        //! Position (3D) and normal (3D).
        Position3Normal3 = Position3 | Normal3,

        //! Position (3D) and texture coordinates (2D).
        Position3TexCoord2 = Position3 | TexCoord2,

        //! Position (3D) and texture coordinates (3D).
        Position3TexCoord3 = Position3 | TexCoord3,

        //! Position (3D) and color in RGBA (4D).
        Position3Color4 = Position3 | Color4,

        //! Position (3D), normal (3D), and texture coordinates (2D).
        Position3Normal3TexCoord2 = Position3Normal3 | TexCoord2,

        //! Position (3D), normal (3D), and color in RGBA (4D).
        Position3Normal3Color4 = Position3Normal3 | Color4,

        //! Position (3D), texture coordinates (2D), and color in RGBA (4D).
        Position3TexCoord2Color4 = Position3TexCoord2 | Color4,

        //! Position (3D), normal (3D), texture coordinates (2D), and color in RGBA
        //! (4D).
        Position3Normal3TexCoord2Color4 = Position3Normal3TexCoord2 | Color4,

        //! Position (3D), normal (3D), texture coordinates (3D), and color in RGBA
        //! (4D).
        Position3Normal3TexCoord3Color4 = Position3Normal3Color4 | TexCoord3,

    };

    //! Bit-wise operator for two vertex formats
    inline VertexFormat operator|(VertexFormat a, VertexFormat b)
    {
        return static_cast<VertexFormat>(static_cast<int>(a) | static_cast<int>(b));
    }

    //! Bit-wise operator for two vertex formats.
    inline void operator|=(VertexFormat& a, const VertexFormat& b)
    {
        a = static_cast<VertexFormat>(static_cast<int>(a) | static_cast<int>(b));
    }

    //! Bit-wise operator for two vertex formats.
    inline VertexFormat operator&(VertexFormat a, VertexFormat b) 
    {
        return static_cast<VertexFormat>(static_cast<int>(a) & static_cast<int>(b));
    }

    //! Vertex with position (3D) data only.
    struct VertexPosition3 
    {
        float x, y, z;
    };

    //! Vertex with position (3D) and normal (3D) data only.
    struct VertexPosition3Normal3 
    {
        float x, y, z;
        float nx, ny, nz;
    };

    //! Vertex with position (3D) and texture coordinates (2D) data only.
    struct VertexPosition3TexCoord2 
    {
        float x, y, z;
        float u, v;
    };

    //! Vertex with position (3D) and texture coordinates (3D) data only.
    struct VertexPosition3TexCoord3 
    {
        float x, y, z;
        float u, v, w;
    };

    //! Vertex with position (3D) and RGBA color (4D) data only.
    struct VertexPosition3Color4 
    {
        float x, y, z;
        float r, g, b, a;
    };

    //! Vertex with position (3D), normal (3D), and texture coordinates (3D) data
    //! only.
    struct VertexPosition3Normal3TexCoord2 
    {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
    };

    //! Vertex with position (3D), normal (3D), and RGBA color (4D) data only.
    struct VertexPosition3Normal3Color4 
    {
        float x, y, z;
        float nx, ny, nz;
        float r, g, b, a;
    };

    //! Vertex with position (3D), texture coordinates (2D), and RGBA color (4D)
    //! data only.
    struct VertexPosition3TexCoord2Color4
    {
        float x, y, z;
        float u, v;
        float r, g, b, a;
    };

    //! Vertex with position (3D), normal (3D), texture coordinates (2D), and RGBA
    //! color (4D) data only.
    struct VertexPosition3Normal3TexCoord2Color4 
    {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
        float r, g, b, a;
    };

    //! Vertex with position (3D), normal (3D), texture coordinates (3D), and RGBA
    //! color (4D) data only.
    struct VertexPosition3Normal3TexCoord3Color4 
    {
        float x, y, z;
        float nx, ny, nz;
        float u, v, w;
        float r, g, b, a;
    };

    //! Collection of vertex helper functions.
    class VertexHelper
    {
     public:
        //! Returns number of floats for a single vertex with given vertex format.
        static size_t GetNumberOfFloats(VertexFormat format);

        //! Returns size of a single vertex with given format in bytes.
        static size_t GetSizeInBytes(VertexFormat format);
    };

}  

#endif 
