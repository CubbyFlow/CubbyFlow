/*************************************************************************
> File Name: Camera.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The camera class which view simulator scene
> Created Time: 2020/04/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/View/Camera.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    Camera::Camera()
    {
        //! Do nothing
    }

    Camera::Camera(const CameraState& camState)
        : _camState(camState)
    {

    }

    Camera::~Camera()
    {
        //! Do nothing
    }

    Matrix4x4F Camera::getViewMatrix() const
    {
        //! http://www.songho.ca/opengl/gl_transform.html#modelview
        const auto& lookAt = _camState.lookAt;
        const auto& lookUp = _camState.lookUp;
        const auto& origin = _camState.origin;
        Vector3F right = lookAt.Cross(lookUp).Normalized();
        Vector3F up = right.Cross(lookAt).Normalized();

        Matrix4x4F view = { {right.x    , right.y   , right.z   , 0.0f},
                            {up.x       , up.y      , up.z      , 0.0f},
                            {lookAt.x   , lookAt.y  , lookAt.z  , 0.0f},
                            {origin.x   , origin.y  , origin.z  , 1.0f} };
        return view;
    }
    
    Vector3F Camera::getCameraOrigin() const
    {
        return _camState.origin;
    }

} 
}