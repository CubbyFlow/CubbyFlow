/*************************************************************************
> File Name: ArcbalController.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Arcball Camera controller
> Created Time: 2020/12/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_ARCBALL_CONTROLLER_HPP
#define CUBBYFLOW_VOX_ARCBALL_CONTROLLER_HPP

#include <Vox/CameraController.hpp>
#include <Core/Vector/Vector3.hpp>

namespace Vox
{
/**
 * Camera class with perspective projection
 */
class ArcballController : public CameraController
{
 public:
     ArcballController(const std::shared_ptr<Camera>& camera);
    ~ArcballController();

 protected:
     void OnSetMouseCursorPos(double x, double y) override;
private:
    CubbyFlow::Vector3D GetArcballVector(const CubbyFlow::Vector2D& cursorPos);
};

};  // namespace Vox

#endif