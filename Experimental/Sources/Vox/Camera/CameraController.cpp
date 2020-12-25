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

	void CameraController::SetScreenSize(const CubbyFlow::Point2I& screenSize)
	{
		_screenSize = screenSize;
	}

	void CameraController::SetMouseCursorPos(double x, double y)
	{
		_cursorPos = CubbyFlow::Point2I(static_cast<int>(x), static_cast<int>(y));

		if (_isCursorPressed)
		{
			_lastCursorPos = _cursorPos;
			_isCursorPressed = false;
		}

		OnSetMouseCursorPos(x, y);
	}

	void CameraController::SetMouseButton(int button, int action)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			_isCursorPressed = true;
		}
	}

	void CameraController::OnSetMouseCursorPos(double x, double y)
	{
		UNUSED_VARIABLE(x);
		UNUSED_VARIABLE(y);
	}
};  // namespace Vox