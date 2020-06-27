/*************************************************************************
> File Name: OrthogonalCamera.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The orthogonal camera derived from camera class.
> Created Time: 2020/06/27
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/View/OrthogonalCamera.h>
#include <Core/Utils/Constants.h>
#include <cassert>
#include <iostream>

namespace CubbyFlow {
namespace CubbyRender {
    
    OrthogonalCamera::OrthogonalCamera()
    {
        //! Do nothing
    }

    OrthogonalCamera::OrthogonalCamera(const Pivot& pivot)
        : Camera(pivot)
    {
        //! Do nothing
    }

    OrthogonalCamera::~OrthogonalCamera()
    {
        //! Do nothing
    }

    Matrix4x4F OrthogonalCamera::getProjectionMatrix() const
    {
        //! http://www.songho.ca/opengl/gl_projectionmatrix.html
        const float r = _pivot.viewport.rightBottom.x;
        const float b = _pivot.viewport.rightBottom.y;
        const float l = _pivot.viewport.leftTop.x;
        const float t = _pivot.viewport.leftTop.y;
        const float f = _pivot.zFar;
        const float n = _pivot.zNear;

        Matrix4x4F projection(0.0f);
		projection(0, 0) = 2 * n / (r - l);
		projection(1, 1) = 2 * n / (t - b);
		projection(2, 0) = (r + l) / (r - l);
		projection(2, 1) = (t + b) / (t - b);
		projection(2, 2) = -(f + n) / (f - n);
        projection(2, 3) = -1.0f;
		projection(3, 2) = -2 * f * n / (f - n);
        return projection;
    }
} 
}