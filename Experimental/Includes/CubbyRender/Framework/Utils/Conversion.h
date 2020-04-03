/*************************************************************************
> File Name: Conversion.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Useful macros and functions collection.
> Created Time: 2020/04/04
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_CONVERSION_H
#define CUBBYFLOW_CONVERSION_H

//! Cubbyflow public headers
#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#include <tiny_obj_loader.h>
#include <vector>

namespace CubbyFlow {
namespace CubbyRender {

    using namespace tinyobj;
    class Reconstructor final
    {
    public:
        static std::vector<float> PackingVertices(attrib_t attrib, std::vector<shape_t> shapes, std::vector<material_t> materials);
    };
}
}

#endif