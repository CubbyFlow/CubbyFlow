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

#include <Framework/GL3/GL3Shader.h>
#include <Framework/Common.h>
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
            if (!success) 
            {
				glGetShaderiv(target, GL_INFO_LOG_LENGTH, &infoLogLength);
				infoLog.resize(infoLogLength);
				glGetShaderInfoLog(target, static_cast<GLsizei>(infoLog.size()), nullptr, &infoLog[0]);
                CUBBYFLOW_ERROR << infoLog.data();
            }
        }
        else if (type == TargetType::PROGRAM)
        {
            glGetProgramiv(target, GL_LINK_STATUS, &success);
			if (!success) 
            {
				glGetProgramiv(target, GL_INFO_LOG_LENGTH, &infoLogLength);
				infoLog.resize(infoLogLength);
				glGetProgramInfoLog(target, static_cast<GLsizei>(infoLog.size()), nullptr, &infoLog[0]);
                CUBBYFLOW_ERROR << infoLog.data();
            }
        }

        return !success;
    }
  
    GL3Shader::GL3Shader()
    {
        //! Do nothing.
    }

    GL3Shader::GL3Shader(const std::string& shaderName)
    {
        UNUSED_VARIABLE(shaderName);
        assert(load(shaderName) == 0);
    }

    GL3Shader::GL3Shader(VertexFormat format, const ShaderMap& fileMap)
    {
        UNUSED_VARIABLE(format);
        UNUSED_VARIABLE(fileMap);
        assert(load(format, fileMap) == 0);
    }

    GL3Shader::~GL3Shader()
    {
        //! Do nothing.
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
            assert( shaderTypeMap.count(shaderPair.first) != 0 );

            const char* source = shaderPair.second.c_str();
            GLuint shaderType = shaderTypeMap[shaderPair.first];
            GLuint shader = glCreateShader(shaderType);
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);
            assert( checkStatus(shader, TargetType::SHADER) == 0 );
            compiledShaders.push_back(shader);
        }

        _programID = glCreateProgram();
        for (GLuint shader : compiledShaders)
            glAttachShader(_programID, shader);
    	glLinkProgram(_programID);
        assert( checkStatus(_programID, TargetType::PROGRAM) == 0 );

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
    }
        
    void GL3Shader::onUnbind(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
        glUseProgram(0U);
    }

    void GL3Shader::onDestroy(RendererPtr renderer)
    {
        UNUSED_VARIABLE(renderer);
        glDeleteProgram(_programID);
    }

} 
}

#endif 