/*************************************************************************
> File Name: OrthogonalCamera.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The orthogonal camera derived from camera class.
> Created Time: 2020/06/27
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_ORTHOGONAL_CAMERA_H
#define CUBBYFLOW_ORTHOGONAL_CAMERA_H

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
    class OrthogonalCamera : public Camera 
    {
    public:
        //! Default constructor.
        OrthogonalCamera();

        //! constructor with cameraState.
        OrthogonalCamera(const Pivot& pivot);

        //! Default destructor.
        ~OrthogonalCamera();

        //! Get orthogonal projection matrix.
        Matrix4x4F getProjectionMatrix() const override;
    protected:
        float _fov = 60.0f;
    private:
    };

    using OrthogonalCameraPtr = std::shared_ptr<OrthogonalCamera>;
} 
}

#endif