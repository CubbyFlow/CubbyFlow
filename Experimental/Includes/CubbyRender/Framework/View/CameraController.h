/*************************************************************************
> File Name: CameraController.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The camera controller class 
> Created Time: 2020/04/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_CAMERACONTROLLER_H
#define CUBBYFLOW_CAMERACONTROLLER_H

#include <Framework/Utils/Prerequisites.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief The camera controller class 
    //!
    class CameraController
    {
    public:
        //! Default constructor.
        CameraController();

        //! Constructor with camera pointer.
        CameraController(CameraPtr camera);

        //! Default destructor.
        ~CameraController();

        //! Set the camera instance.
        void setCamera(CameraPtr camera);

        //! Return the camera pointer.
        CameraPtr getCamera();

        //! Translate the origin of the camera
        void translateCamera(int vertical, int horizontal);

        //! Rotate the pointing direction.
        void modifyDirection(double xoffset, double yoffset);
    protected:
        CameraPtr _camera;
        float _camSpeed = 0.5f;
        float _pitch    = 0.0f;
        float _yaw      = -90.0f;
    private:
    };

    using CameraControllerPtr = std::shared_ptr<CameraController>;
} 
}

#endif