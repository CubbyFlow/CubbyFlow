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
        //! Do nothing
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

        Vector3F at = (origin - lookAt).Normalized();
        Vector3F up = lookUp;
        Vector3F right = up.Cross(at).Normalized();
        up = at.Cross(right);

        Matrix4x4F view = {{right.x,           up.x,           at.x,               0.0f},
                           {right.y,           up.y,           at.y,               0.0f},
                           {right.z,           up.z,           at.z,               0.0f},
                           {right.Dot(origin), up.Dot(origin), (-at).Dot(origin),  1.0f}};

        return view;
    }
    
    Vector3F Camera::getCameraOrigin() const
    {
        return _camState.origin;
    }

} 
}