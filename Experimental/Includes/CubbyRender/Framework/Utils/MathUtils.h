/*************************************************************************
> File Name: MathUtils.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Calculation utilities. (ex. degree to radian).
> Created Time: 2020/06/06
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_MATH_UTILITIES_H
#define CUBBYFLOW_MATH_UTILITIES_H

namespace CubbyFlow {
namespace CubbyRender {
    template <typename T>
    inline T degreeToRadian(T degree);

    template <typename T>
    inline T radianToDegree(T radian);
}
}

#include <Framework/Utils/MathUtils-Impl.h>

#endif