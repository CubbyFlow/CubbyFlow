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
#include <Core/Utils/Constants.h>
#include <cassert>
#include <iostream>

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
        //! https://github.com/g-truc/glm/blob/master/glm/ext/matrix_clip_space.inl
        Matrix4x4F projection(0.0f);
        const float tanHalfFovy = std::tan(0.45f / static_cast<float>(2));
        const float aspect = _camState.viewport.getWidth() / _camState.viewport.getHeight();
        const float zNear = _camState.zNear;
        const float zFar = _camState.zFar;
        
		projection(0, 0) = static_cast<float>(1) / (aspect * tanHalfFovy);
		projection(1, 1) = static_cast<float>(1) / (tanHalfFovy);
		projection(2, 2) = zFar / (zNear - zFar);
		projection(2, 3) = - static_cast<float>(1);
		projection(3, 2) = -(zFar * zNear) / (zFar - zNear);
        return projection;
    }
} 
}