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
#include <algorithm>
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

	void FlycamController::UpdateCamera(double dt)
	{
		const CubbyFlow::Vector3F direction = _camera->GetCameraDirection();
		const CubbyFlow::Vector3F up		= _camera->GetCameraUp();
		const CubbyFlow::Vector3F right		= direction.Cross(up).Normalized();

		const float speed = _camSpeed * static_cast<float>(dt);
		CubbyFlow::Vector3F origin = _camera->GetCameraOrigin();

		if (_pressedKeys[KeyMoveInput::UP])
			origin = origin - direction * speed;

		if (_pressedKeys[KeyMoveInput::DOWN])
			origin = origin + direction * speed;

		if (_pressedKeys[KeyMoveInput::LEFT])
			origin = origin + right * speed;

		if (_pressedKeys[KeyMoveInput::RIGHT])
			origin = origin - right * speed;

		if (std::any_of(std::begin(_pressedKeys), std::end(_pressedKeys), [](bool b) { return b; }))
		{
			_camera->SetOrigin(origin);
			_camera->UpdateMatrix();
		}
	}

	void FlycamController::OnSetMouseCursorPos()
	{
		if (_isCursorPressed == false)
			return;

		constexpr double sensitivity = 8e-4;
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

};  // namespace Vox