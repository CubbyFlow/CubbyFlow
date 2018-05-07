/*************************************************************************
> File Name: ClaraUtils.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Util functions for Clara library.
> Created Time: 2018/05/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CLARA_UTILS_H
#define CUBBYFLOW_CLARA_UTILS_H

#include <Clara/include/clara.hpp>

#include <string>

inline std::string ToString(const clara::Opt& opt)
{
    std::ostringstream oss;
    oss << (clara::Parser() | opt);
    return oss.str();
}

inline std::string ToString(const clara::Parser& p)
{
    std::ostringstream oss;
    oss << p;
    return oss.str();
}

#endif