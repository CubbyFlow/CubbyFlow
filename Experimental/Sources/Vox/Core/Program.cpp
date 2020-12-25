/*************************************************************************
> File Name: Program.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Program wrapper class
> Created Time: 2020/08/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/Program.hpp>
#include <Vox/Scene/VoxScene.hpp>
#include <Vox/Camera/PerspectiveCamera.hpp>
#include <glad/glad.h>

namespace Vox {

    Program::Program(GLuint program)
        : _program(program)
    {
        //! Do nothing
    }

    Program::~Program()
    {
        if(_program) glDeleteProgram(_program);
    }

    void Program::BindProgram(const std::shared_ptr<VoxScene>& scene)
    {
        glUseProgram(_program);
        
        if (HasUniformVariable("ViewProjection"))
        {
            const auto& camera = scene->GetSceneObject<PerspectiveCamera>("Camera");
            if (camera)
            {
                _parameters.SetParameter("ViewProjection", camera->GetViewProjectionMatrix());
            }
        }

        if (HasUniformVariable("ViewPos"))
        {
            const auto& camera = scene->GetSceneObject<PerspectiveCamera>("Camera");
            if (camera)
            {
                _parameters.SetParameter("ViewPos", camera->GetCameraOrigin());
            }
        }

        if (HasUniformVariable("LightPosition"))
        {
            const auto& light = scene->GetSceneObject<Light>("DirectionalLight");
            if (light)
            {
                _parameters.SetParameter("LightPosition", light->GetCameraOrigin());
            }
        }

        SendParametersToGPU();
    }
    
    const ShaderParameters& Program::GetParameters() const
    {
        return _parameters;
    }

    ShaderParameters& Program::GetParameters()
    {
        return _parameters;
    }

    GLint Program::GetUniformLocation(const std::string& name)
    {
        auto iter = _locationCache.find(name);
        if (iter == _locationCache.end())
        {
            //! When Cache miss.
            GLint location = glGetUniformLocation(_program, name.c_str());
            _locationCache[name] = location;

           return location; 
        }
        else
        {
            //! When Cache hit.
            return iter->second;
        }
    }

    bool Program::HasUniformVariable(const std::string& name)
    {
        return GetUniformLocation(name.c_str()) >= 0;
    }

    void Program::SendParametersToGPU()
    {
        const auto& metatable = _parameters.GetMetatable();

        for (const auto& p : metatable)
        {
            GLint location = GetUniformLocation(p.first);
            VoxAssert(location >= 0, CURRENT_SRC_PATH_TO_STR, "Cannot find uniform [" << p.first << "] in this shader");
            
            const auto& metadata = p.second;
            const void* data = metadata.data.data();

            switch(metadata.type)
            {
                case ParameterType::INT      :
                {
                    glUniform1i(location, reinterpret_cast<const GLint*>(data)[0]);
                    break;
                }      
                case ParameterType::FLOAT1   :
                {
                    glUniform1f(location, reinterpret_cast<const GLfloat*>(data)[0]);
                    break;
                }            
                case ParameterType::FLOAT2   :
                {
                    glUniform2fv(location, 1, reinterpret_cast<const GLfloat*>(data));
                    break;
                }               
                case ParameterType::FLOAT3   :
                {
                    glUniform3fv(location, 1, reinterpret_cast<const GLfloat*>(data));
                    break;
                }                     
                case ParameterType::FLOAT4   :
                {
                    glUniform4fv(location, 1, reinterpret_cast<const GLfloat*>(data));
                    break;
                }            
                case ParameterType::FLOAT4X4 :
                {
                    glUniformMatrix4fv(location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(data));
                    break;
                }                   
                default:
                {
                    VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown ShaderParamter::ParameterType");
                }
            }
        }
    }    
};