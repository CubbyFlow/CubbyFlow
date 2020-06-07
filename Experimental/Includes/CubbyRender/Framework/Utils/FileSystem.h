/*************************************************************************
> File Name: FileSystem.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Useful template functions related to filesystem such as looping over directory.
> Created Time: 2020/06/05
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_FILESYSTEM_H
#define CUBBYFLOW_FILESYSTEM_H

#include <Core/Utils/Macros.h>
#include <string>
#include <vector>

namespace CubbyFlow {
namespace CubbyRender {
#ifdef CUBBYFLOW_WINDOWS
    template <typename Callback>
    inline void loopDirectory(const std::wstring& dir, const std::wstring& extension, Callback callback, bool recursive=true);
#else
    template <typename Callback>
    inline void loopDirectory(const std::wstring& dir, const std::wstring& extension, Callback callback, bool recursive=true);
#endif

    inline void listFileNames(const std::wstring& dir, const std::wstring& extension, std::vector<std::wstring>* files)
    {
        files->clear();
        loopDirectory(dir, extension, [&](const std::wstring& path){
            files->push_back(path);
        });
    }
};
};

#include <Framework/Utils/FileSystem-Impl.h>

#endif