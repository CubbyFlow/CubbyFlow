/*************************************************************************
> File Name: PerspectiveCamera.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The perspective camera derived from camera class.
> Created Time: 2020/04/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/View/PerspectiveCamera.h>
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {
    
    PerspectiveCamera::PerspectiveCamera()
    {
        //! Do nothing
    }

    PerspectiveCamera::PerspectiveCamera(const CameraState& camState, float fov)
        : Camera(camState), _fov(fov)
    {
        //! Do nothing
    }

    PerspectiveCamera::~PerspectiveCamera()
    {
        //! Do nothing
    }

    Matrix4x4F PerspectiveCamera::getProjectionMatrix() const
    {
        //! http://www.songho.ca/opengl/gl_projectionmatrix.html
        const float width   = _camState.viewport.getWidth();
        const float height  = _camState.viewport.getHeight();
        const float aspect  = width / height;
        const float far     = _camState.zFar;
        const float near    = _camState.zNear;

        Matrix4x4F projection = { {1.0f / (aspect * std::tan(_fov/2.0f)), 0.0f, 0.0f, 0.0f},
                                  {0.0f, 1.0f / std::tan(_fov/2.0f), 0.0f, 0.0f},
                                  {0.0f, 0.0f, -(far + near) / (far - near), -1.0f},
                                  {0.0f, 0.0f, 2.0f * far * near / (far - near), 0.0f}};
                                  
        return projection;
    }

} 
}