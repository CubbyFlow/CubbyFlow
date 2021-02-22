/*************************************************************************
> File Name: CameraController.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Camera controller which transform camera with given user input
> Created Time: 2020/12/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_CAMERA_CONTROLLER_HPP
#define CUBBYFLOW_VOX_CAMERA_CONTROLLER_HPP

#include <Core/Vector/Vector3.hpp>
#include <Core/Geometry/Point2.hpp>

namespace Vox
{
class Camera;
/**
 * Camera controller which transform camera with given user input
 */
class CameraController
{
 public:
    //! Constructor with camera smart pointer
    CameraController(const std::shared_ptr<Camera>& camera);
    //! Default Destructor
    virtual ~CameraController();
    //! Update the camera like position and direction and etc...
    virtual void UpdateCamera(double dt);
    //! Set camera speed(default is 0.08)
    void SetCameraSpeed(const float speed);
    //! Input processing methods
    void SetKey(int key, int scancode, int action, int mods);
    void SetMouseButton(int button, int action, int mods);
    void SetMouseCursorPos(double x, double y);
    void SetMouseScroll(double deltaX, double deltaY);

    enum KeyMoveInput
    {
        LEFT = 0,
        RIGHT,
        UP,
        DOWN,
        MAX_KEY_MOVE_INPUT
    };
 protected:
    //! Implementations of the SetMouseCursorPos and SetMouseScroll
    //! which will be used for camera translation.
     virtual void OnSetMouseCursorPos() {};
     virtual void OnSetMouseScroll(double deltaX, double deltaY);

    std::shared_ptr<Camera> _camera;
    CubbyFlow::Point2I _lastCursorPos{ 0, 0 };
    CubbyFlow::Point2I _cursorPos{ 0, 0 };
    float _camSpeed { 0.03f };
    bool _pressedKeys[MAX_KEY_MOVE_INPUT] { false, };
    bool _isCursorPressed{ false };
private:
};

};  // namespace Vox

#endif