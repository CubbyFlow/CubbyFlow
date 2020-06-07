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

#include <cmath>
#include <iostream>

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
        auto& camState = _camera->getCameraState();
        if (vertical) //! If vertical value is non-zero
        {
            camState.origin -= camState.lookAt * (_camSpeed * vertical);
        }
        if (horizontal) //! If horizontal value is non-zero
        {
            camState.origin -= camState.lookAt.Cross(camState.lookUp).Normalized() * (_camSpeed * horizontal);
        }
    }

    void CameraController::modifyDirection(double xoffset, double yoffset)
    {
        _yaw     += static_cast<float>(xoffset);
        _pitch   += static_cast<float>(yoffset);
        //! Clamping Pitch;
        _pitch = std::min( std::max(-89.0f, _pitch), 89.0f );

        auto& camState = _camera->getCameraState();
        const float yawRadian   = degreeToRadian(_yaw);
        const float pitchRadian = degreeToRadian(_pitch);
        camState.lookAt.x = std::cos(yawRadian) * std::cos(pitchRadian);
        camState.lookAt.y = std::sin(pitchRadian);
        camState.lookAt.z = std::sin(yawRadian) * std::cos(pitchRadian);
        camState.lookAt.Normalize();
    }
} 
}