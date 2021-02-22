/*************************************************************************
> File Name: FlycamController.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Flying Camera controller
> Created Time: 2020/12/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_FLYCAM_CONTROLLER_HPP
#define CUBBYFLOW_VOX_FLYCAM_CONTROLLER_HPP

#include <Vox/Camera/CameraController.hpp>
#include <Core/Vector/Vector3.hpp>

namespace Vox
{
/**
 * Camera class with perspective projection
 */
class FlycamController : public CameraController
{
 public:
     FlycamController(const std::shared_ptr<Camera>& camera);
    ~FlycamController();
    
    //! Update the camera like position and direction and etc...
    void UpdateCamera(double dt) override;

 protected:
     void OnSetMouseCursorPos() override;
};

};  // namespace Vox

#endif