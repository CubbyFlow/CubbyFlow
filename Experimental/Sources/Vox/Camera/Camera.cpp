/*************************************************************************
> File Name: Camera.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Base camera class
> Created Time: 2020/12/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Camera/Camera.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Core/Program.hpp>
#include <Core/Math/Quaternion.hpp>

using namespace CubbyFlow;
namespace Vox {
    
    void Camera::SetViewTransform(const CubbyFlow::Vector3F& origin, const CubbyFlow::Vector3F& point)
    {
        _origin = origin;
        _dir = (_origin - point).Normalized();
    }

    void Camera::OrbitRotation(const CubbyFlow::Vector3F& focusPoint, float yaw, float pitch, float distance)
    {
        constexpr CubbyFlow::Vector3F kLookUp = CubbyFlow::Vector3F(0.0f, 1.0f, 0.0f);
        CubbyFlow::Vector3F camFocus = (_origin - focusPoint).Normalized();
        //! create quaternion matrix with up vector and yaw angle.
        CubbyFlow::QuaternionF yawRotation(kLookUp, yaw); 
        //! create quaternion matrix with right vector and pitch angle.
        CubbyFlow::QuaternionF pitchRotation(_dir.Cross(kLookUp).Normalized(), pitch); 

        camFocus = (yawRotation * pitchRotation * camFocus);
        _origin = focusPoint + camFocus * distance;
        _dir = (focusPoint - _origin).Normalized();

        UpdateMatrix();
    }

    CubbyFlow::Matrix4x4F Camera::GetViewProjectionMatrix() const
    {
        return _viewProjection;
    }

};
