/*************************************************************************
> File Name: GL3Shader.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Renderer class implemented with Modern OpenGL
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifdef CUBBYFLOW_USE_GL

#include <GL3/Shader/GL3Shader.h>
#include <Framework/Utils/Common.h>
#include <Core/Utils/Logging.h>
#include <glad/glad.h>
#include <cassert>
#include <vector>

namespace CubbyFlow {
namespace CubbyRender {
    
    enum class TargetType 
    {
        SHADER = 0,
        PROGRAM = 1
    };

    //! Check compile or link result. If result is failed, print info log.
    //! \param target shader or program id
    //! \param type TargetType::SHADER or TargetType::PROGRAM
    //! \return return compile or link result.
    int checkStatus(GLuint target, TargetType type)
    {
        int success = 0;
        int infoLogLength;
        std::vector<char> infoLog;

        if (type == TargetType::SHADER)
        {
			glGetShaderiv(target, GL_COMPILE_STATUS, &success);
			glGetShaderiv(target, GL_INFO_LOG_LENGTH, &infoLogLength);
			infoLog.resize(infoLogLength);
			glGetShaderInfoLog(target, static_cast<GLsizei>(infoLog.size()), nullptr, &infoLog[0]);
        }
        else if (type == TargetType::PROGRAM)
        {
            glGetProgramiv(target, GL_LINK_STATUS, &success);
		    glGetProgramiv(target, GL_INFO_LOG_LENGTH, &infoLogLength);
		    infoLog.resize(infoLogLength);
		    glGetProgramInfoLog(target, static_cast<GLsizei>(infoLog.size()), nullptr, &infoLog[0]);
        }

        return !success;
    }
  
    GL3Shader::GL3Shader()
    {
        //! Do nothing.
    }

    GL3Shader::GL3Shader(const std::string& shaderName)
    {
        int result = load(shaderName);
        if (result) 
            abort();
    }

    GL3Shader::GL3Shader(VertexFormat format, const ShaderMap& fileMap)
    {
        int result = load(format, fileMap);
        if (result)
            abort();
    }

    GL3Shader::~GL3Shader()
    {
        destroy();
    }
    
    GLuint GL3Shader::getProgramID() const
    {
        return _programID;
    }

    GLuint GL3Shader::getAttribLocation(const std::string& name)
    {
        auto iter = _locationCache.find(name);
        if (iter == _locationCache.end())
        {
            //! When Cache miss.
            GLuint location = glGetAttribLocation(_programID, name.c_str());
            _locationCache[name] = location;

           return location; 
        }
        else
        {
            //! When Cache hit.
            return iter->second;
        }
    }

    GLuint GL3Shader::getUniformLocation(const std::string& name)
    {
        auto iter = _locationCache.find(name);
        if (iter == _locationCache.end())
        {
            //! When Cache miss.
            GLuint location = glGetUniformLocation(_programID, name.c_str());
            _locationCache[name] = location;

           return location; 
        }
        else
        {
            //! When Cache hit.
            return iter->second;
        }
    }

    int GL3Shader::onLoad(const ShaderMap& shaderMap)
    {
        //! If shader sources dont exist return 1.
        if (shaderMap.empty())
            return 1;

        //! If vertex shader doesnt exist, return 1.
        //! vertex shader is essential component for shader program.
        if (shaderMap.count("VertexShader") == 0)
            return 1;

        static std::unordered_map<std::string, GLuint> shaderTypeMap = {
            {"VertexShader", GL_VERTEX_SHADER},
            {"GeometryShader", GL_GEOMETRY_SHADER},
            {"FragmentShader", GL_FRAGMENT_SHADER},
            //{"TessControlShader", GL_TESS_CONTROL_SHADER},
            //{"TessEvaluationShader", GL_TESS_EVALUATION_SHADER},
            //{"ComputeShader", GL_COMPUTE_SHADER},
        };

        std::vector<GLuint> compiledShaders;
        for (const auto& shaderPair : shaderMap)
        {
            if (shaderTypeMap.count(shaderPair.first) == 0)
                abort();

            const char* source = shaderPair.second.c_str();
            GLuint shaderType = shaderTypeMap[shaderPair.first];
            GLuint shader = glCreateShader(shaderType);
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);
            if (checkStatus(shader, TargetType::SHADER))
                abort();
            compiledShaders.push_back(shader);
        }

        _programID = glCreateProgram();
        for (GLuint shader : compiledShaders)
            glAttachShader(_programID, shader);
    	glLinkProgram(_programID);
        if (checkStatus(_programID, TargetType::PROGRAM))
                abort();
                
        for (GLuint shader : compiledShaders)
        {
            glDetachShader(_programID, shader);
            glDeleteShader(shader);
        }
        return 0;
    }

    void GL3Shader::onBind(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
        glUseProgram(_programID);
        sendParametersToGPU();
    }
        
    void GL3Shader::onUnbind(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
        glUseProgram(0U);
    }

    void GL3Shader::onDestroy()
    {
        if (static_cast<int>(_programID))
        {
            glDeleteProgram(_programID);
            _programID = 0U;
        }
    }

    void GL3Shader::sendParametersToGPU()
    {
        const auto& metatable = _parameters.getMetatable();

        for (const auto& p : metatable)
        {
            const auto& name = p.first;
            const auto& metadata = p.second;

            GLint location = getUniformLocation(name);

            const unsigned char* data = metadata.data.data();
            const GLint* intdata = reinterpret_cast<const GLint*>(data);
            const GLfloat* floatdata = reinterpret_cast<const GLfloat*>(data);

            switch(metadata.type)
            {
                case ParameterType::INT      :
                {
                    glUniform1i(location, *intdata);
                    break;
                }      
                case ParameterType::FLOAT1   :
                {
                    glUniform1f(location, *floatdata);
                    break;
                }            
                case ParameterType::FLOAT2   :
                {
                    glUniform2fv(location, 1, floatdata);
                    break;
                }               
                case ParameterType::FLOAT3   :
                {
                    glUniform3fv(location, 1, floatdata);
                    break;
                }                     
                case ParameterType::FLOAT4   :
                {
                    glUniform4fv(location, 1, floatdata);
                    break;
                }            
                case ParameterType::FLOAT4X4 :
                {
                    glUniformMatrix4fv(location, 1, GL_FALSE, floatdata);
                    break;
                }                   
                default:
                {
                    CUBBYFLOW_ERROR << "Unknown ShaderParamter::ParameterType";
                    abort();
                }
            }
        }
    }    
} 
}

#endif 