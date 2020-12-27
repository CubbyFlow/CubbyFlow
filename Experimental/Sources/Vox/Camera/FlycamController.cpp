/*************************************************************************
> File Name: FlycamController.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Flying camera controller
> Created Time: 2020/12/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Camera/FlycamController.hpp>
#include <Vox/Camera/Camera.hpp>
#include <Core/Math/MathUtils.hpp>
#include <GLFW/glfw3.h>
#include <Core/Vector/Vector2.hpp>
#include <Core/Vector/Vector3.hpp>
#include <Core/Math/Quaternion.hpp>

namespace Vox
{
	FlycamController::FlycamController(const std::shared_ptr<Camera>& camera)
		: CameraController(camera)
	{
		//! Do nothing.
	}

	FlycamController::~FlycamController()
	{
		//! Do nothing
	}

	void FlycamController::OnSetMouseCursorPos()
	{
		if (_isCursorPressed == false)
			return;

		constexpr double sensitivity = 0.001;
		const double xoffset = (_cursorPos.x - _lastCursorPos.x) * sensitivity;
		const double yoffset = (_lastCursorPos.y - _cursorPos.y) * sensitivity;

		CubbyFlow::Vector3F direction = _camera->GetCameraDirection();
		CubbyFlow::Vector3F up = _camera->GetCameraUp();

		//! create quaternion matrix with up vector and yaw angle.
		CubbyFlow::QuaternionF yawRotation(up, xoffset);
		//! create quaternion matrix with right vector and pitch angle.
		CubbyFlow::QuaternionF pitchRotation(direction.Cross(up).Normalized(), yoffset);

		direction = (yawRotation * pitchRotation * direction);
		_camera->SetDirection(direction);
		_camera->UpdateMatrix();
	}

	void FlycamController::OnSetKey(int key, int action)
	{
		const CubbyFlow::Vector3F direction = _camera->GetCameraDirection();
		const CubbyFlow::Vector3F up = _camera->GetCameraUp();
		const CubbyFlow::Vector3F right = direction.Cross(up).Normalized();

		CubbyFlow::Vector3F origin = _camera->GetCameraOrigin();
		if (action == GLFW_PRESS)
		{
			switch (key)
			{
			case GLFW_KEY_W:
				origin = origin - direction * _camSpeed;
				break;
			case GLFW_KEY_A:
				origin = origin - right * _camSpeed;
				break;
			case GLFW_KEY_S:
				origin = origin + direction * _camSpeed;
				break;
			case GLFW_KEY_D:
				origin = origin + right * _camSpeed;
				break;
			}

			_camera->SetOrigin(origin);
			_camera->UpdateMatrix();
		}
	}

};  // namespace Vox