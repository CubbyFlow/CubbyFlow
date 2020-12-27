/*************************************************************************
> File Name: Camera.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Base camera class
> Created Time: 2020/12/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Camera/Camera.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Core/Program.hpp>
#include <Core/Math/Quaternion.hpp>

using namespace CubbyFlow;
namespace Vox {
    
    void Camera::SetViewTransform(const CubbyFlow::Vector3F& origin, const CubbyFlow::Vector3F& point, const CubbyFlow::Vector3F& up)
    {
        SetOrigin(origin);
        SetDirection(_origin - point);
        _up = up;
    }

    CubbyFlow::Matrix4x4F Camera::GetViewProjectionMatrix() const
    {
        return _viewProjection;
    }

};
