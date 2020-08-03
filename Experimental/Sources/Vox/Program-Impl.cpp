/*************************************************************************
> File Name: Program-Impl.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Program wrapper class
> Created Time: 2020/08/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Program-Impl.hpp>
#include <Vox/FileSystem.hpp>
#include <Vox/DebugUtils.hpp>
#include <glad/glad.h>

struct GLFWwindow;

namespace Vox {
    
    namespace Detail
    {
        GLuint GetUniformVariable(GLuint program, const std::string& name)
        {
            GLint loc = glGetUniformLocation(program, name.c_str());
            //! If return value of glGetUniformLocation is -1, it means there is no uniform variable with given name arg.
            VoxAssert(loc != -1, CURRENT_SRC_PATH_TO_STR, std::string("Unknown Program Uniform Variable Name - ") + name);
            return loc;
        }

        void SendUniformVariable__Impl(GLint loc, float f)
        {
            glUniform1f(loc, f);
        }
        
        void SendUniformVariable__Impl(GLint loc, CubbyFlow::Vector3F&& vec3)
        {
            glUniform3fv(loc, 1, &(vec3.x));
        }

        void SendUniformVariable__Impl(GLint loc, CubbyFlow::Vector4F&& vec4)
        {
            glUniform4fv(loc, 1, &(vec4.x));
        }

        void SendUniformVariable__Impl(GLint loc, CubbyFlow::Matrix4x4F&& mat4)
        {
            auto uniformData = std::move(mat4);
            glUniformMatrix4fv(loc, 1, false, uniformData.data());
        }       
    }

};