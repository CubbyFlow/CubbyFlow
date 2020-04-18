/*************************************************************************
> File Name: PerspectiveCamera.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The perspective camera derived from camera class.
> Created Time: 2020/04/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_PERSPECTIVE_CAMERA_H
#define CUBBYFLOW_PERSPECTIVE_CAMERA_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/View/Camera.h>
#include <Core/Matrix/Matrix4x4.h>
#include <Core/Utils/Constants.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief The camera class which view simulator scene
    //!
    class PerspectiveCamera : public Camera 
    {
    public:
        //! Default constructor.
        PerspectiveCamera();

        //! constructor with cameraState.
        PerspectiveCamera(const CameraState& camState, float fov);

        //! Default destructor.
        ~PerspectiveCamera();

        //! Get perspective projection matrix.
        Matrix4x4F getProjectionMatrix() const override;
    protected:
        float _fov = HALF_PI_FLOAT;
    private:
    };

    using PerspectiveCameraPtr = std::shared_ptr<PerspectiveCamera>;
} 
}

#endif