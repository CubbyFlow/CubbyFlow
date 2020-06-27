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

    PerspectiveCamera::PerspectiveCamera(const Pivot& pivot, float fov)
        : Camera(pivot), _fov(fov)
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
        const float aspect = static_cast<float>(_pivot.viewport.getWidth()) / _pivot.viewport.getHeight();
        const float fovTan = 1.0f / std::tan((PI_FLOAT * _fov / 180.0f) / 2.0f);
        
        Matrix4x4F projection(0.0f);
		projection(0, 0) = fovTan / aspect;
		projection(1, 1) = fovTan;
		projection(2, 2) = -(_pivot.zFar - _pivot.zNear) / (_pivot.zFar + _pivot.zNear);
		projection(2, 3) = -1.0f;
		projection(3, 2) = -2.0f * _pivot.zFar * _pivot.zNear / (_pivot.zFar - _pivot.zNear);
        return projection;
    }

    inline void PerspectiveCamera::setFov(float fov)
    {
        _fov = fov;
    }
    inline float PerspectiveCamera::getFov() const
    {
        return _fov;
    }
} 
}