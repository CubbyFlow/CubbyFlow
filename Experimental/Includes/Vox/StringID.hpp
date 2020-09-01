/*************************************************************************
> File Name: VoxResource.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Texture wrapper class
> Created Time: 2020/08/30
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_STRING_ID_HPP
#define CUBBYFLOW_VOX_STRING_ID_HPP

#include <string>

namespace Vox {

    inline unsigned int CellHash(const std::string &str)
    {
        unsigned int hash = 0;
    
        for (auto& it : str) {
            hash = 37 * hash + 17 * static_cast<char>(it);
        }
    
        return hash;
    }
    
    inline unsigned int CellHash(const char* cStr)
    {
        std::string str(cStr);
        return CellHash(str);
    }
    
    #define VoxStringID(string) CellHash(string)
};

#endif