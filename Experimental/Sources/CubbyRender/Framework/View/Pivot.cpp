/*************************************************************************
> File Name: Pivot.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The Pivot class which contains properties of camera.
> Created Time: 2020/04/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/View/Pivot.h>

namespace CubbyFlow {
namespace CubbyRender {

    bool Pivot::operator==(const Pivot& other) const
    {
        return (viewport == other.viewport) &&
               (origin == other.origin) && (lookAt == other.lookAt) && (lookUp == other.lookUp) &&
               (zNear == other.zNear) && (zFar == other.zFar);
    }

    bool Pivot::operator!=(const Pivot& other) const
    {
        return !(*this == other);
    }

} 
}