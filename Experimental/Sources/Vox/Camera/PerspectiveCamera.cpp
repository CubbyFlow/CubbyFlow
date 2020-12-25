/*************************************************************************
> File Name: PerspectiveCamera.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: amera class with perspective projection
> Created Time: 2020/08/02
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Camera/PerspectiveCamera.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Core/Program.hpp>
#include <Core/Math/Quaternion.hpp>
#include <iostream>

using namespace CubbyFlow;
namespace Vox {
    void PerspectiveCamera::SetViewFrustum(const float fov, const float near, const float far)
    {
        _fov = fov;
        _near = near;
        _far = far;
    }

    void PerspectiveCamera::SetAspectRatio(const float aspectRatio)
    {
        _aspectRatio = aspectRatio;
    }

    void PerspectiveCamera::UpdateMatrix()
    {
        constexpr Vector3F up = Vector3F(0.0f, 1.0f, 0.0f);
        const Vector3F right = (up.Cross(_dir)).Normalized();
        const Vector3F cameraUp = (_dir.Cross(right)).Normalized();

        Matrix4x4F projection(0.0f);
        projection(0, 0) = 1.0f / (std::tan(PI_FLOAT * _fov / 360.0f) * _aspectRatio);
        projection(1, 1) = 1.0f / (std::tan(PI_FLOAT * _fov / 360.0f));
        projection(2, 2) = (_far + _near) / (_near - _far);
        projection(2, 3) = (2 * _far * _near) / (_near - _far);
        projection(3, 2) = -1.0f;

        Matrix4x4F view = { {right.x,        right.y,        right.z,        0.0f},
                            {cameraUp.x,     cameraUp.y,     cameraUp.z,     0.0f},
                            {_dir.x,         _dir.y,         _dir.z,         0.0f},
                            {0.0f,           0.0f,           0.0f,           1.0f} };

        Matrix4x4F translation = { {1.0f, 0.0f, 0.0f, -_origin.x},
                                   {0.0f, 1.0f, 0.0f, -_origin.y},
                                   {0.0f, 0.0f, 1.0f, -_origin.z},
                                   {0.0f, 0.0f, 0.0f,   1.0f  } };

        _viewProjection = projection * view * translation;
    }
};
