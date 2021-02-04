/*************************************************************************
> File Name: OrthogonalCamera.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: camera class with orthogonal projection
> Created Time: 2020/12/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Camera/OrthographicCamera.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Core/Program.hpp>
#include <Core/Math/Quaternion.hpp>
#include <iostream>

using namespace CubbyFlow;
namespace Vox {
    void OrthographicCamera::SetViewFrustum(const float left, const float right, const float top, const float bottom)
    {
        //TODO(snowapril) : must implement orthograpic projection
        static_assert("Not yet implemented");
        UNUSED_VARIABLE(left);
        UNUSED_VARIABLE(right);
        UNUSED_VARIABLE(top);
        UNUSED_VARIABLE(bottom);
    }
  
    void OrthographicCamera::UpdateMatrix()
    {
        static_assert("Not yet implemented");
    }
};
