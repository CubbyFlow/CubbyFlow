/*************************************************************************
> File Name: ArcballController.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Arcball camera controller
> Created Time: 2020/12/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Camera/ArcballController.hpp>
#include <Vox/Camera/Camera.hpp>
#include <GLFW/glfw3.h>
#include <Core/Vector/Vector3.hpp>
#include <Core/Matrix/Matrix3x3.hpp>

namespace Vox
{
	ArcballController::ArcballController(const std::shared_ptr<Camera>& camera)
		: CameraController(camera)
	{
		//! Do nothing.
	}

	ArcballController::~ArcballController()
	{
		//! Do nothing
	}

	void ArcballController::OnSetMouseCursorPos(double x, double y)
	{
		auto a = GetArcballVector(_lastCursorPos);
		auto b = GetArcballVector(_cursorPos);
		double angle = std::acos(std::min(1.0, a.Dot(b)));
		CubbyFlow::Vector3D axisInCameraCoord = a.Cross(b);
		(void)angle;
		(void)axisInCameraCoord;
	}

	CubbyFlow::Vector3D ArcballController::GetArcballVector(const CubbyFlow::Point2I& pos)
	{
		CubbyFlow::Vector3D point{ pos.x / _screenSize.x * 2 - 1.0, 1.0 - pos.y / _screenSize.y * 2, 0.0 };
		double pointSquared = point.x * point.x + point.y * point.y;
		if (pointSquared <= 1.0)
		{
			point.z = std::sqrt(1.0 - pointSquared);
		}
		else
		{
			point.Normalize();
		}
		return point;
	}
};  // namespace Vox