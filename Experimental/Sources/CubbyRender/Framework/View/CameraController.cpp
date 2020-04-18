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
} 
}