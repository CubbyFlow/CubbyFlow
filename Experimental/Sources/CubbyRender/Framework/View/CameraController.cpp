/*************************************************************************
> File Name: CameraController.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The camera controller class 
> Created Time: 2020/04/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/View/CameraController.h>
#include <Framework/View/Camera.h>
#include <Framework/Utils/MathUtils.h>
#include <Core/Math/Quaternion.h>
#include <cmath>

namespace CubbyFlow {
namespace CubbyRender {
    
    CameraController::CameraController()
    {
        //! Do nothing
    }

    CameraController::CameraController(CameraPtr camera)
    {
        _camera = camera;
    }

    CameraController::~CameraController()
    {
        //! Do nothing
    }

    void CameraController::setCamera(CameraPtr camera)
    {
        _camera = camera;
    }

    CameraPtr CameraController::getCamera()
    {
        return _camera;
    }

    void CameraController::translateCamera(int vertical, int horizontal)
    {
        auto& pivot = _camera->getPivot();
        if (vertical) //! If vertical value is non-zero
        {
            pivot.origin -= pivot.lookAt * (_camSpeed * vertical);
        }
        if (horizontal) //! If horizontal value is non-zero
        {
            pivot.origin -= pivot.lookAt.Cross(pivot.lookUp).Normalized() * (_camSpeed * horizontal);
        }
    }

    void CameraController::modifyDirection(double xoffset, double yoffset)
    {
        _yaw     += static_cast<float>(xoffset);
        _pitch   += static_cast<float>(yoffset);
        //! Clamping Pitch;
        _pitch = std::min( std::max(-89.0f, _pitch), 89.0f );

        auto& pivot = _camera->getPivot();
        const float yawRadian   = degreeToRadian(_yaw);
        const float pitchRadian = degreeToRadian(_pitch);
        pivot.lookAt.x = std::cos(yawRadian) * std::cos(pitchRadian);
        pivot.lookAt.y = std::sin(pitchRadian);
        pivot.lookAt.z = std::sin(yawRadian) * std::cos(pitchRadian);
        pivot.lookAt.Normalize();
    }

    void CameraController::orbitRotation(const Vector3F& focusPoint, float yaw, float pitch, float distance)
    {
        auto& pivot = _camera->getPivot();
        Vector3F camFocus = (pivot.origin - focusPoint).Normalized();
        //! create quaternion matrix with up vector and yaw angle.
        QuaternionF yawRotation(pivot.lookUp, yaw); 
        //! create quaternion matrix with right vector and pitch angle.
        QuaternionF pitchRotation(pivot.lookAt.Cross(pivot.lookUp).Normalized(), pitch); 

        camFocus = (yawRotation * pitchRotation * camFocus);
        pivot.origin = focusPoint + camFocus * distance;
        pivot.lookAt = (focusPoint - pivot.origin).Normalized();
    }
} 
}