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
#include <Framework/View/Pivot.h>
#include <Framework/View/Viewport.h>
#include <Framework/Scene/SceneObject.h>
#include <Core/Matrix/Matrix4x4.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief The camera class which view simulator scene
    //!
    class Camera : public SceneObject
    {
    public:
        //! Default constructor.
        Camera();

        //! constructor with pivot.
        Camera(const Pivot& pivot);

        //! Default destructor.
        virtual ~Camera();

        //! Get camera view transition matrix.
        Matrix4x4F getViewMatrix() const;

        //! Get perspective projection matrix.
        virtual Matrix4x4F getProjectionMatrix() const = 0;

        //! Get the mutable reference of the camera state
        Pivot& getPivot();

        //! Get the immutable reference of the camera state.
        const Pivot& getPivot() const;

    protected:
        Pivot _pivot;
    private:
    };

    using CameraPtr = std::shared_ptr<Camera>;
} 
}

#endif