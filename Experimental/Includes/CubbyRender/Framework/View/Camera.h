/*************************************************************************
> File Name: Camera.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The camera class which view simulator scene
> Created Time: 2020/04/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_CAMERA_H
#define CUBBYFLOW_CAMERA_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/View/CameraState.h>
#include <Framework/View/Viewport.h>
#include <Core/Matrix/Matrix4x4.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief The camera class which view simulator scene
    //!
    class Camera 
    {
    public:
        //! Default constructor.
        Camera();

        //! constructor with cameraState.
        Camera(const CameraState& camState);

        //! Default destructor.
        virtual ~Camera();

        //! Get camera view transition matrix.
        Matrix4x4F getViewMatrix() const;

        //! Get perspective projection matrix.
        virtual Matrix4x4F getProjectionMatrix() const = 0;

        //! Get the mutable reference of the camera state
        CameraState& getCameraState();

        //! Get the immutable reference of the camera state.
        const CameraState& getCameraState() const;

    protected:
        CameraState _camState;
    private:
    };

    using CameraPtr = std::shared_ptr<Camera>;
} 
}

#endif