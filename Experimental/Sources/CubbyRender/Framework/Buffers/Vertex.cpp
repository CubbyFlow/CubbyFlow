/*************************************************************************
> File Name: Vertex.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Vertex formats and Vertex Structures.
> Created Time: 2020/03/19
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Buffer/Vertex.h>
#include <Framework/Utils/Common.h>
#include <Core/Utils/Timer.h>
#include <algorithm>
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {

    size_t VertexHelper::getNumberOfFloats(VertexFormat format) 
    {
        size_t size = 0;

        if (static_cast<int>(format & VertexFormat::Position3)) 
            size += 3;

        if (static_cast<int>(format & VertexFormat::Normal3)) 
            size += 3;

        if (static_cast<int>(format & VertexFormat::TexCoord2)) 
            size += 2;

        if (static_cast<int>(format & VertexFormat::TexCoord3)) 
            size += 3;

        if (static_cast<int>(format & VertexFormat::Color4))
            size += 4;

        return size;
    }

    size_t VertexHelper::getSizeInBytes(VertexFormat format) 
    {
        return sizeof(float) * getNumberOfFloats(format);
    }
    
}  
}  