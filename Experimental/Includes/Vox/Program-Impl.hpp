/*************************************************************************
> File Name: Program-Impl.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Program wrapper class
> Created Time: 2020/08/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_PROGRAM_IMPL_HPP
#define CUBBYFLOW_VOX_PROGRAM_IMPL_HPP

#include <Vox/Program.hpp>
#include <Vox/GLTypes.hpp>
#include <Core/Vector/Vector3.hpp>
#include <Core/Vector/Vector4.hpp>
#include <Core/Matrix/Matrix4x4.hpp>

namespace Vox {
    
    namespace Detail
    {
        GLuint GetUniformVariable(const GLuint program, const std::string& name);
        void SendUniformVariable__Impl(const GLint loc, const int i);
        void SendUniformVariable__Impl(const GLint loc, const float f);
        void SendUniformVariable__Impl(const GLint loc, const CubbyFlow::Vector3F vec3);
        void SendUniformVariable__Impl(const GLint loc, const CubbyFlow::Vector4F vec4);
        void SendUniformVariable__Impl(const GLint loc, const CubbyFlow::Matrix4x4F& mat4);
        void SendUniformVariable__Impl(const GLint loc, CubbyFlow::Matrix4x4F&& mat4);
    }

    template <typename UniformType>
    void Program::SendUniformVariable(const std::string& name, UniformType&& var)
    {
        auto iter = _uniformCache.find(name);
        if (iter == _uniformCache.end())
        {
            GLint loc = Detail::GetUniformVariable(_program, name);
            _uniformCache.emplace(name, loc);
            Detail::SendUniformVariable__Impl(loc, std::forward<UniformType>(var));
        }
        else
        {
            GLint loc = iter->second;
            Detail::SendUniformVariable__Impl(loc, std::forward<UniformType>(var));
        }
    }

};

#endif