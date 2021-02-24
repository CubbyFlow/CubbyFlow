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
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Core/Renderer.hpp>
#include <Vox/Scene/VoxScene.hpp>
#include <Vox/Utils/VectorUtils.hpp>
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

    void Program::BindProgram(const std::shared_ptr<FrameContext>& ctx)
    {
        UNUSED_VARIABLE(ctx);
        glUseProgram(_program);
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
                    glUniformMatrix4fv(location, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(data));
                    break;
                }                   
                default:
                {
                    VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown ShaderParamter::ParameterType");
                }
            }
        }
    }    

    void Program::LoadXMLNode(VoxScene* scene, const pugi::xml_node& node)
    {
        UNUSED_VARIABLE(scene);
        //! At first, vs, fs and gs are initialized by zero.
        GLuint vs{ 0 }, fs{ 0 }, gs{ 0 };
        for (const auto& shader : node.child("shader"))
        {
            //! Parse shader type. It would be vs, fs or gs.
            const std::string type = shader.name();
            const auto& path = FileSystem::FindPath(shader.attribute("value").value()).ToString();

            //! Create shader resource with given shader type.
            if (type == "vs")
            {
                vs = Renderer::CreateShaderFromFile(path, GL_VERTEX_SHADER);
            }
            else if (type == "fs")
            {
                fs = Renderer::CreateShaderFromFile(path, GL_FRAGMENT_SHADER);
            }
            else if (type == "gs")
            {
                gs = Renderer::CreateShaderFromFile(path, GL_GEOMETRY_SHADER);
            }
            else
            {
                VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown shader type : " + type);
            }
        }

        //! Assign linked program ID to the member variable.
        this->_program = Renderer::CreateProgram(vs, gs, fs);

        //! After program generation, delete shaders because they are out of need anymore.
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        if (gs) glDeleteShader(gs);

        //! Get reference of the shader parameters.
        auto& shaderParams = this->GetParameters();

        //! Store uniform variables which will be passed at once in the render loop.
        for (const auto& uniform : node.child("uniform"))
        {
            const std::string name = uniform.attribute("name").value();
            const std::string type = uniform.name();

            const auto& valueNode = uniform.attribute("value");
            if (type == "integer")
            {
                shaderParams.SetParameter(name, valueNode.as_int());
            }
            else if (type == "float")
            {
                shaderParams.SetParameter(name, valueNode.as_float());
            }
            else if (type == "rgb" || type == "xyz")
            {
                shaderParams.SetParameter(name, ParseFromString<float, 3>(valueNode.value()));
            }
            else
            {
                VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown shader uniform variable type : " + type);
            }
        }
    }

    void Program::WriteXMLNode(pugi::xml_node& node)
    {
        UNUSED_VARIABLE(node);
    }
};