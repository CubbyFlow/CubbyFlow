/*************************************************************************
> File Name: CameraController.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Camera controller class which will be implemented as arcball or flyingcam
> Created Time: 2020/12/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_CAMERA_CONTROLLER_HPP
#define CUBBYFLOW_VOX_CAMERA_CONTROLLER_HPP

#include <Core/Vector/Vector3.hpp>
#include <Core/Geometry/Point2.hpp>
#include <Vox/VoxSceneObject.hpp>

namespace Vox
{
class Camera;
/**
 * Camera class with perspective projection
 */
class CameraController : public VoxSceneObject
{
 public:
    CameraController(const std::shared_ptr<Camera>& camera);
    virtual ~CameraController();

    void SetScreenSize(const CubbyFlow::Point2I& screenSize);
    void SetMouseCursorPos(double x, double y);
    void SetMouseButton(int button, int action);
 protected:
     virtual void OnSetMouseCursorPos(double x, double y);

     std::shared_ptr<Camera> _camera;
     CubbyFlow::Point2I _lastCursorPos{ 0, 0 };
     CubbyFlow::Point2I _cursorPos{ 0, 0 };
     CubbyFlow::Point2I _screenSize{ 0, 0 };
     bool _isCursorPressed{ false };
private:
};

};  // namespace Vox

#endif