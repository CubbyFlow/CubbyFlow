/*************************************************************************
> File Name: MathUtils-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Calculation utilities. (ex. degree to radian).
> Created Time: 2020/06/06
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_MATH_UTILITIES_IMPL_H
#define CUBBYFLOW_MATH_UTILITIES_IMPL_H

#include <Core/Utils/Constants.h>

namespace CubbyFlow {
namespace CubbyRender {
    template <typename T>
    inline T degreeToRadian(T degree)
    {
        return static_cast<T>(degree * PI<T>() / 180.0f);
    }

    template <typename T>
    inline T radianToDegree(T radian)
    {
        return static_cast<T>(radian * 180.0f / PI<T>());
    }
}
}


#endif