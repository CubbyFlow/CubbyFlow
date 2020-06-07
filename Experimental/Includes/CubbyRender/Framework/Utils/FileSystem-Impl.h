/*************************************************************************
> File Name: FileSystem-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Useful template functions related to filesystem such as looping over directory.
> Created Time: 2020/06/05
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_FILESYSTEM_IMPL_H
#define CUBBYFLOW_FILESYSTEM_IMPL_H

#include <Core/Utils/Macros.h>
#include <iostream>

#ifdef CUBBYFLOW_WINDOWS
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

namespace CubbyFlow {
namespace CubbyRender {
    
#ifdef CUBBYFLOW_WINDOWS
    template <typename Callback>
    void loopDirectory(const std::wstring& dir, const std::wstring& extension, Callback callback, bool recursive)
    {
        HANDLE hFind = INVALID_HANDLE_VALUE;
        WIN32_FIND_DATAW ffd;
        std::wstring filename;
        std::wstring format = L"." + extension;
        wchar_t* ext = nullptr;

        filename = dir + L"/*";
        hFind = FindFirstFileW(filename.c_str(), &ffd);
        if (hFind == INVALID_HANDLE_VALUE)
        {
            std::wcerr << L"Cannot read directory [" << dir << L"]" << std::endl;
            return;
        }

        do 
        {
            //! If the filename is "." or "..", skip it.
            if (wcscmp(ffd.cFileName, L".") == 0 || wcscmp(ffd.cFileName, L"..") == 0)
            {
                continue;
            }
            //! Insert full path of the file to the variable.
            filename = dir + L"/" + ffd.cFileName;
            if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                //! if the file is directory and recursive flag set, call loopDirectory again.
                if (recursive)
                {
                    loopDirectory(filename, extension, callback, recursive);
                }
            }
            else
            {
                //! Split the filename with "."
                if ((ext = wcschr(ffd.cFileName, L'.')) == NULL)
                    return;
                //! if the extension of current file is equal to the extension parameter, 
                //! pass it to the callback.
                if (wcscmp(ext, format.c_str()) == 0)
                {
                    callback(filename);
                }    
            }
        } while(FindNextFileW(hFind, &ffd) != 0);
        //! After looping over the directory, close the handle.
        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }
#else
    template <typename Callback>
    void loopDirectory(const std::wstring& dir, const std::wstring& extension, Callback callback, bool recursive)
    {
    	struct wdirent* wFile = NULL;
    	struct wstat buf;
    	std::wstring filename;
    	char* ext;
    	WDIR* wDirHandle;
        //! If opening directory was failed, return.
    	if ((wDirHandle = wopendir(dir.c_str())) == NULL)
    	{
    		std::wcerr << "Cannot read directory [" << dir << "]" << std::endl;
    		return;
    	}
        //! Loop over directory.
    	while((wFile = readdir(dir)) != NULL)
    	{
            //! Skip file with name "." or ".."
    		if (strcmp(file->d_name, ".") || strcmp(file->d_name, ".."))
    		{
    			continue;
    		}
            //! Insert full path to the filename variable.
    		snprintf(filename, "%s/%s", dir.c_str(), file->d_name);

            //! Reading file status to a buf.
    		if (stat(filename, &buf) == -1)
    		{
    			continue;
    		}

            //! If the given file is directory and recursive variable is true, recursively call function again.
    		if (S_ISDIR(buf.st_mode) && recursive)
    		{
                loopDirecotry(filename, callback, recursive);
    		}
    		else if (S_ISREG(buf.st_mode))
    		{
                //! If the given file is regular file, pass it to the callback function.
                callback(filename);
    		}
    	}
        //! After looping over directory, close the directory.
    	closedir(dir);
    }
#endif
};
};


#endif