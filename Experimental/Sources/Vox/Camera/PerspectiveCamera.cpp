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
#include <Vox/Utils/MatrixUtils.hpp>
#include <Core/Math/Quaternion.hpp>
#include <iostream>

using namespace CubbyFlow;
namespace Vox {
    void PerspectiveCamera::SetViewFrustum(const float fov, const float far)
    {
        _fov = fov;
        _far = far;
    }

    void PerspectiveCamera::SetAspectRatio(const float aspectRatio)
    {
        _aspectRatio = aspectRatio;
    }

    void PerspectiveCamera::UpdateMatrix()
    {
        _viewProjection = Perspective(_aspectRatio, _near, _far, _fov) * LookAt(_origin, _dir, _up);
    }
};
