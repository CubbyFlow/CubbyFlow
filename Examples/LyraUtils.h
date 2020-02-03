/*************************************************************************
> File Name: LyraUtils.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Util functions for Lyra library.
> Created Time: 2018/05/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_LYRA_UTILS_H
#define CUBBYFLOW_LYRA_UTILS_H

#include <lyra/lyra.hpp>

#include <string>

inline std::string ToString(const lyra::opt& opt)
{
    std::ostringstream oss;
    oss << (lyra::cli_parser() | opt);
    return oss.str();
}

inline std::string ToString(const lyra::cli_parser& p)
{
    std::ostringstream oss;
    oss << p;
    return oss.str();
}

#endif