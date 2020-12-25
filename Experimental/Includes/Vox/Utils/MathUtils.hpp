/*************************************************************************
> File Name: MathUtils.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Math utilties like degree to radian function/
> Created Time: 2020/12/25
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_MATH_UTILS_HPP
#define CUBBYFLOW_VOX_MATH_UTILS_HPP

#include <Core/Utils/Constants.hpp>

namespace Vox {

    inline float DegreeToRadian(const float degree)
    {
        return degree * CubbyFlow::PI_FLOAT / 180.0f;
    }

    inline float RadianToDegree(const float rad)
    {
        return 180.0f * rad / CubbyFlow::PI_FLOAT;
    }
}

#endif