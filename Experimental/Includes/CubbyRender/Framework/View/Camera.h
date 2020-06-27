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
#include <Core/Matrix/Matrix4x4.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    enum class ProjectionMethod 
    {
        PERSPECTIVE,
        ORTHOGONAL
    };

    //!
    //! \brief The camera class which view simulator scene
    //!
    class Camera 
    {
    public:
        //! Default constructor.
        Camera();

        //! constructor with pivot..
        Camera(const Pivot& pivot);

        //! Default destructor.
        virtual ~Camera();

        //! Get camera view transition matrix.
        Matrix4x4F getViewMatrix() const;

        //! Get projection matrix.
        Matrix4x4F getProjectionMatrix() const;

        //! Setters
        void setProjectionMethod(ProjectionMethod method);
        void setViewport(Viewport viewport);
        void setNearFar(float near, float far);
        void setFovy(float fovy);
        //! Mutable getters
        Pivot& getPivot();
        Viewport& getViewport();
        //! Immutable getters
        ProjectionMethod getProjectionMethod() const;
        const Pivot& getPivot() const;
        const Viewport& getViewport() const;
        float getNear() const;
        float getFar() const;
        float getFovy() const;
    protected:
        Pivot _pivot;
        Viewport _viewport;
        float _zNear { 0.1f };
        float _zFar { 10000.0f };
        float _fovy { 60.0f };
        ProjectionMethod _method { ProjectionMethod::PERSPECTIVE };
    private:
    };

    using CameraPtr = std::shared_ptr<Camera>;
    
    using Light = Camera;
    using LightPtr = std::shared_ptr<Light>;
} 
}

#endif