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
#include <Framework/View/Viewport.h>
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

        //! Default destructor.
        ~Camera();

    protected:
    private:
        Viewport _viewport;
    };

    using CameraPtr = std::shared_ptr<Camera>;
} 
}

#endif