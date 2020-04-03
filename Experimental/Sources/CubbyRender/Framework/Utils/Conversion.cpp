/*************************************************************************
> File Name: Conversion.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Several Conversion functions such as tinyobj to vertices with alignment packed.
> Created Time: 2020/04/04
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Utils/Conversion.h>
#include <Framework/Utils/Common.h>

namespace CubbyFlow {
namespace CubbyRender {

    std::vector<float> PackingVertices(attrib_t attrib, std::vector<shape_t> shapes, std::vector<material_t> materials)
    {
        UNUSED_VARIABLE(attrib);
        UNUSED_VARIABLE(shapes);
        UNUSED_VARIABLE(materials);
        return std::vector<float>();
    }
}
}