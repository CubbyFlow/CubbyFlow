/*************************************************************************
> File Name: FileSystem.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Simple filesystem class
> Created Time: 2020/07/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/FileSystem.hpp>
#include <Vox/DebugUtils.hpp>
#include <iostream>
#ifdef CUBBYFLOW_WINDOWS
#include <windows.h>
#include <fileapi.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#endif

std::vector<Vox::Path> Vox::FileSystem::kTargetRange;

namespace Vox {

    Path::Path(const std::string& path)
        : _path(path)
    {
        //! Do nothing
    }

    Path::Path(const char* path)
        : _path(path)
    {
        //! Do nothing
    }

    std::string Path::ToString() const
    {
        return std::string(_path);
    }

    bool Path::IsNullPath() const
    {
        return _path.empty();
    }
    
    Path::operator bool() const
    {
        return !IsNullPath();
    }

    bool Path::operator==(const Path& path)
    {
        return _path == path._path;
    }

    SourcePath::SourcePath(const char* function, const char* file, const int line)
        : Path(file), _function(function), _line(line)
    {
        //! Do nothing
    }

    std::string SourcePath::ToVerboseString() const
    {
        return _path + "::" + std::string(_function) + "(" + std::to_string(3) + ")";
    }

    void FileSystem::AddDirectory(const Path& path)
    {
        kTargetRange.push_back(path);
    }

    Path FileSystem::FindPath(const Path& path)
    {
        for (const auto& dir : kTargetRange)
        {
            Path result = LoopDirectory(dir, path);
            if (!result.IsNullPath())
                return result;
        }

        return Path(nullptr);
    }

    #ifdef CUBBYFLOW_WINDOWS
    Path FileSystem::LoopDirectory(const Path& dir, const Path& path)
    {
        HANDLE hFind = INVALID_HANDLE_VALUE;
        WIN32_FIND_DATAA ffd;
        std::string filename;
        std::string format = "." + extension;

        filename = dir.ToString() + "/*";
        hFind = FindFirstFileA(filename.c_str(), &ffd);
        if (hFind == INVALID_HANDLE_VALUE)
        {
            std::cerr << "Cannot read directory [" << dir << "]" << std::endl;
            return Path(std::string());
        }

        do 
        {
            //! If the filename is "." or "..", skip it.
            if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0)
            {
                continue;
            }
            //! Insert full path of the file to the variable.
            filename = dir.ToString() + "/" + ffd.cFileName;
            if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                //! if the file is directory and recursive flag set, call loopDirectory again.
                Path result = LoopDirectory(Path(filename), path);
                if (!result.IsNullPath())
                    return result;
            }
            else
            {
                //! If two relative path matches, return.
                if (Path(ffd.cFileName) == path)
                    return Path(filename);
            }
        } while(FindNextFileA(hFind, &ffd) != 0);
        //! After looping over the directory, close the handle.
        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
        //! If branch flow reaches here, there is no path in the list of directory.
        return Path(std::string());
    }

    void FileSystem::ReadFile(const Path& path, CubbyFlow::Array1<unsigned char>& data)
    {
        HANDLE hFind = INVALID_HANDLE_VALUE;
        DWORD dwBytesRead = 0;
        const std::string& filename = path.ToString();

        hFind = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        //! File open assertion.
        VoxAssert(hFind != INVALID_HANDLE_VALUE, CURRENT_SRC_PATH_TO_STR, "Cannot open file [" + filename + "]");

        //! Query file size without opening
        LARGE_INTEGER size;
        VoxAssertFailCallback(GetFileSize(hFind, &size), CURRENT_SRC_PATH_TO_STR, "Cannot Query File Size [" + filename + "]", [&hFind](){ 
            CloseHandle(hFind); 
        }); 

        //! resize the buffer according to file size.
        data.Resize(static_cast<size_t>(size.QuadPart));

        //! Read the buffer to the data argument.
        VoxAssertFailCallback(ReadFile(hFind, data.data(), data.size() - 1, &dwBytesRead, NULL), 
                  CURRENT_SRC_PATH_TO_STR, 
                  "Cannot Read File [" + filename + "]", 
                  [&hFind](){ CloseHandle(hFind); }
        );

        data[dwBytesRead + 1] = '\0';
        CloseHandle(hFind);
    }
    #else
    Path FileSystem::LoopDirectory(const Path& dir, const Path& path)
    {
        struct dirent* file = NULL;
    	struct stat buf;
    	std::string filename;
    	DIR* dirHandle;
        //! If opening directory was failed, return.
    	if ((dirHandle = opendir(dir.ToString().c_str())) == NULL)
    	{
    		std::cerr << "Cannot read directory [" << dir.ToString() << "]" << std::endl;
    		return Path(std::string());
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
            filename = dir.ToString() + "/" + file->d_name;
            //! Reading file status to a buf.
    		if (stat(filename.c_str(), &buf) == -1)
    		{
    			continue;
    		}

            //! If the given file is directory and recursive variable is true, recursively call function again.
    		if (S_ISDIR(buf.st_mode))
    		{
                Path result = LoopDirectory(Path(filename), path);
                if (!result.IsNullPath())
                    return result;
    		}
    		else if (S_ISREG(buf.st_mode))
    		{
                //! If two relative path matches, return.
                if (Path(file->d_name) == path)
                    return Path(filename);
    		}
    	}
        //! After looping over directory, close the directory.
    	closedir(dirHandle);
        //! If branch flow reaches here, there is no path in the list of directory.
        return Path(std::string());
    }

    void FileSystem::ReadFile(const Path& path, CubbyFlow::Array1<unsigned char>& data)
    {
        struct stat st;
        const std::string& filename = path.ToString();
        
        //! check stat of the file.
        VoxAssert(stat(filename.c_str(), &st) == 0, CURRENT_SRC_PATH_TO_STR, "Cannot call stat function to [" + filename + "]");

        //! resize the given buffer to the stat size.
        data.Resize(st.st_size);

        FILE* fp = fopen(filename.c_str(), "rb");
        VoxAssert(fp, CURRENT_SRC_PATH_TO_STR, "Cannot open file [" + filename + "]");

        int numRead = fread(data.data(), data.size(), 1, fp);
        VoxAssertFailCallback(numRead == 1, CURRENT_SRC_PATH_TO_STR, "An error occurred while reading file [" + filename + "]", [&fp](){
            fclose(fp);
        });

        fclose(fp);
    }

    #endif
};