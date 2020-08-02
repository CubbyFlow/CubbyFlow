/*************************************************************************
> File Name: PerspectiveCamera.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: amera class with perspective projection
> Created Time: 2020/08/02
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/PerspectiveCamera.hpp>
#include <Vox/FrameContext.hpp>

using namespace CubbyFlow;
namespace Vox {
    
    void PerspectiveCamera::SetViewElements(const CubbyFlow::Vector3F& origin, const CubbyFlow::Vector3F& point)
    {
        _origin = origin;
        _dir = (_origin - point).Normalized();
    }

    void PerspectiveCamera::SetPerspectiveElements(const float fov, const float aspectRatio, const float near, const float far)
    {
        _fov = fov;
        _aspectRatio = aspectRatio;
        _near = near;
        _far = far;
    }

    void PerspectiveCamera::UpdateMatrix()
    {
        constexpr Vector3F up = Vector3F(0.0f, 1.0f, 0.0f);
        const Vector3F right    = (up.Cross(_dir)).Normalized();
        const Vector3F cameraUp = (_dir.Cross(right)).Normalized(); 
        const float tanHalfFovInv = 1.0f / std::tan(_fov * 0.5f); //! inverse of tangent half fov.
        const float aspectRatioInv = 1.0f / _aspectRatio; //! inverse of aspect ratio
        const float nfDiffInv = 1.0f / (_far - _near); //! inverse of difference between near and far

        Matrix4x4F projection   = {{tanHalfFovInv * aspectRatioInv, 0.0f, 0.0f, 0.0f},
                                   {0.0f, tanHalfFovInv, 0.0f, 0.0f},
                                   {0.0f, 0.0f, -(_far + _near) * nfDiffInv, -(2 * _far * _near) * nfDiffInv},
                                   {0.0f, 0.0f, -1.0f, 0.0f}};
        Matrix4x4F view         = {{right.x,        right.y,        right.z,        0.0f},
                                   {cameraUp.x,     cameraUp.y,     cameraUp.z,     0.0f},
                                   {_dir.x,         _dir.y,         _dir.z,         0.0f},
                                   {0.0f,           0.0f,           0.0f,           1.0f}};
        Matrix4x4F translation  = {{1.0f, 0.0f, 0.0f, -_origin.x},
                                   {0.0f, 1.0f, 0.0f, -_origin.y},
                                   {0.0f, 0.0f, 1.0f, -_origin.z},
                                   {0.0f, 0.0f, 0.0f,   1.0f  }};

        _viewProjection = projection * view * translation;
    }

    CubbyFlow::Matrix4x4F PerspectiveCamera::GetViewProjectionMatrix() const
    {
        return _viewProjection;
    }

};