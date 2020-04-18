/*************************************************************************
> File Name: Viewport.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The Viewport class matched with camera view
> Created Time: 2020/04/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/View/Viewport.h>
#include <cmath>

namespace CubbyFlow {
namespace CubbyRender {
    
    float Viewport::getWidth() const
    {
        return std::fabs(rightBottom.x - leftTop.x);
    }
    
    float Viewport::getHeight() const
    {
        return std::fabs(leftTop.y - rightBottom.y);
    }

    bool Viewport::operator==(const Viewport& other) const
    {
        return (leftTop == other.leftTop) &&
               (rightBottom == other.rightBottom);
    }

    bool Viewport::operator!=(const Viewport& other) const
    {
        return !(*this == other);
    }

} 
}