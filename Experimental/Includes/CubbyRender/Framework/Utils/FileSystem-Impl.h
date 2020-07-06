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
#include <string.h>
#endif

namespace CubbyFlow {
namespace CubbyRender {
    
#ifdef CUBBYFLOW_WINDOWS
    template <typename Callback>
    void loopDirectory(const std::string& dir, const std::string& extension, Callback callback, bool recursive)
    {
        HANDLE hFind = INVALID_HANDLE_VALUE;
        WIN32_FIND_DATAA ffd;
        std::string filename;
        std::string format = "." + extension;
        char_t* ext = nullptr;

        filename = dir + "/*";
        hFind = FindFirstFileA(filename.c_str(), &ffd);
        if (hFind == INVALID_HANDLE_VALUE)
        {
            std::cerr << L"Cannot read directory [" << dir << "]" << std::endl;
            return;
        }

        do 
        {
            //! If the filename is "." or "..", skip it.
            if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0)
            {
                continue;
            }
            //! Insert full path of the file to the variable.
            filename = dir + "/" + ffd.cFileName;
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
                if ((ext = strchr(ffd.cFileName, '.')) == NULL)
                    return;
                //! if the extension of current file is equal to the extension parameter, 
                //! pass it to the callback.
                if (strcmp(ext, format.c_str()) == 0)
                {
                    callback(filename);
                }    
            }
        } while(FindNextFileA(hFind, &ffd) != 0);
        //! After looping over the directory, close the handle.
        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }
#else
    template <typename Callback>
    void loopDirectory(const std::string& dir, const std::string& extension, Callback callback, bool recursive)
    {
    	struct dirent* file = NULL;
    	struct stat buf;
        std::string format = "." + extension;
    	std::string filename;
    	char* ext;
    	DIR* dirHandle;
        //! If opening directory was failed, return.
    	if ((dirHandle = opendir(dir.c_str())) == NULL)
    	{
    		std::cerr << "Cannot read directory [" << dir << "]" << std::endl;
    		return;
    	}
        //! Loop over directory.
    	while((file = readdir(dirHandle)) != NULL)
    	{
            //! Skip file with name "." or ".."
    		if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
    		{
    			continue;
    		}
            //! Insert full path to the filename variable.
            filename = dir + "/" + file->d_name;
            
            //! Reading file status to a buf.
    		if (stat(filename.c_str(), &buf) == -1)
    		{
    			continue;
    		}

            //! If the given file is directory and recursive variable is true, recursively call function again.
    		if (S_ISDIR(buf.st_mode) && recursive)
    		{
                loopDirectory(filename, extension, callback, recursive);
    		}
    		else if (S_ISREG(buf.st_mode))
    		{
                
                //! If the given file is regular file, pass it to the callback function.
                if ((ext = strchr(file->d_name, '.')) == NULL)
                    return;
                //! if the extension of current file is equal to the extension parameter, 
                //! pass it to the callback.
                if (strcmp(ext, format.c_str()) == 0)
                {
                    callback(filename);
                }    
    		}
    	}
        //! After looping over directory, close the directory.
    	closedir(dirHandle);
    }
#endif
};
};


#endif