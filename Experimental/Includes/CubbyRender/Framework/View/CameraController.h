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

        void setCamera(CameraPtr camera);

        CameraPtr getCamera();
    protected:
        CameraPtr _camera;
    private:
    };

    using CameraControllerPtr = std::shared_ptr<CameraController>;
} 
}

#endif