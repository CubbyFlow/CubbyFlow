/*************************************************************************
> File Name: CameraState.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The camera state class which contains properties of camera.
> Created Time: 2020/04/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/View/CameraState.h>

namespace CubbyFlow {
namespace CubbyRender {

    bool CameraState::operator==(const CameraState& other) const
    {
        return (viewport == other.viewport) &&
               (origin == other.origin) && (lookAt == other.lookAt) && (lookUp == other.lookUp) &&
               (zNear == other.zNear) && (zFar == other.zFar);
    }

    bool CameraState::operator!=(const CameraState& other) const
    {
        return !(*this == other);
    }

} 
}