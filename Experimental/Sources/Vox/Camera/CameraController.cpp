/*************************************************************************
> File Name: CameraController.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Camera controller class which will be implemented as arcball or flyingcam
> Created Time: 2020/12/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Camera/CameraController.hpp>
#include <Vox/Camera/Camera.hpp>
#include <GLFW/glfw3.h>

namespace Vox
{
	CameraController::CameraController(const std::shared_ptr<Camera>& camera)
		: _camera(camera)
	{
		//! Do nothing.
	}

	CameraController::~CameraController()
	{
		//! Do nothing
	}

	void CameraController::UpdateCamera(double dt)
	{
		UNUSED_VARIABLE(dt);
	}

	void CameraController::SetCameraSpeed(const float speed)
	{
		_camSpeed = speed;
	}

	void CameraController::SetMouseScroll(double deltaX, double deltaY)
	{
		OnSetMouseScroll(deltaX, deltaY);
	}

	void CameraController::SetMouseCursorPos(double x, double y)
	{
		_cursorPos = CubbyFlow::Point2I(static_cast<int>(x), static_cast<int>(y));

		OnSetMouseCursorPos();
		_lastCursorPos = _cursorPos;
	}

	void CameraController::SetKey(int key, int scancode, int action, int mods)
	{
		UNUSED_VARIABLE(scancode);
		UNUSED_VARIABLE(mods);
		
		if (action == GLFW_PRESS || action == GLFW_RELEASE)
		{
			switch (key)
			{
			case GLFW_KEY_W:
				_pressedKeys[KeyMoveInput::UP] = action;
				break;
			case GLFW_KEY_A:
				_pressedKeys[KeyMoveInput::LEFT] = action;
				break;
			case GLFW_KEY_S:
				_pressedKeys[KeyMoveInput::DOWN] = action;
				break;
			case GLFW_KEY_D:
				_pressedKeys[KeyMoveInput::RIGHT] = action;
				break;
			}
		}
	}

	void CameraController::SetMouseButton(int button, int action, int mods)
	{
		UNUSED_VARIABLE(mods);
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			if (action == GLFW_PRESS)
				_isCursorPressed = true;
			else if (action == GLFW_RELEASE)
				_isCursorPressed = false;
		}
	}

	void CameraController::OnSetMouseScroll(double deltaX, double deltaY)
	{
		UNUSED_VARIABLE(deltaX);
		UNUSED_VARIABLE(deltaY);
	}
};  // namespace Vox