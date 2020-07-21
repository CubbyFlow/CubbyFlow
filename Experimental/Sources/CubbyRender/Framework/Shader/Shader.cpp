/*************************************************************************
> File Name: Shader.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Shader object.
> Created Time: 2020/02/19
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Shader/Shader.h>
#include <Framework/View/Camera.h>
#include <Framework/View/Light.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/Shader/ShaderPreset.h>
#include <Framework/Utils/Common.h>
#include <Core/Matrix/Matrix4x4.h>
#include <cassert>

#include <fstream>
#include <sstream>

namespace CubbyFlow {
namespace CubbyRender {
    
    Shader::Shader()
    {
        //! Do nothing.
    }

    Shader::~Shader()
    {   
        //! Do nothing.
    }

    int Shader::load(const std::string& shaderName)
    {
        ShaderMap shaderMap;

        if (shaderName == "simple_shader")
        {
            const std::string& vertexShader     = kSimpleColorShaders[0];
            const std::string& fragmentShader   = kSimpleColorShaders[1];

            shaderMap = {
                {"VertexShader", vertexShader},
                {"FragmentShader", fragmentShader}
            };

            _format = VertexFormat::Position3;
        }
        else if (shaderName == "point_shader")
        {
            const std::string& vertexShader     = kPointsShaders[0];
            const std::string& fragmentShader   = kPointsShaders[1];

            shaderMap = {
                {"VertexShader", vertexShader},
                {"FragmentShader", fragmentShader}
            };
        }
        else if (shaderName == "screen_shader")
        {
            const std::string& vertexShader     = kScreenShaders[0];
            const std::string& fragmentShader   = kScreenShaders[1];

            shaderMap = {
                {"VertexShader", vertexShader},
                {"FragmentShader", fragmentShader}
            };
        }
        else if (shaderName == "fluid_mesh_shader")
        {
            const std::string& vertexShader     = kFluidMeshShaders[0];
            const std::string& fragmentShader   = kFluidMeshShaders[1];

            shaderMap = {
                {"VertexShader", vertexShader},
                {"FragmentShader", fragmentShader}
            };
        }
        return onLoad(shaderMap);
    }

    int Shader::load(VertexFormat format, const ShaderMap& fileMap)
    {
        ShaderMap shaderMap;

        for(const auto& shaderPair : fileMap)
        {
            const std::string& shaderType = shaderPair.first;
            std::string shaderFilePath = shaderPair.second;
            
            std::ifstream file(shaderFilePath.c_str());
            if (file)
            {
		        std::ostringstream osstr;
		        osstr << file.rdbuf();
		        std::string shaderSource = osstr.str();
                shaderMap.insert(std::make_pair(shaderType, shaderSource));
		        file.close();
            }
            else
            {
                return 1;
            }
        }
        
        _format = format;
        return onLoad(shaderMap);
    }

    VertexFormat Shader::getInputVertexFormat() const
    {
        return _format;
    }

    const ShaderParameters& Shader::getParameters() const
    {
        return _parameters;
    }

    ShaderParameters& Shader::getParameters()
    {
        return _parameters;
    }

    void Shader::setParameters(const ShaderParameters& parameters)
    {
        _parameters = parameters;
    }

    void Shader::bind(RendererPtr renderer)
    {
        const auto& camera = renderer->getCamera();
        if (camera)
        {
            const auto&     view    = camera->getViewMatrix();
            const auto& projection  = camera->getProjectionMatrix();
            
            _parameters.setParameter("View", view);
            _parameters.setParameter("Projection", projection);
        }

        const auto& lights = renderer->getLights();
        if (lights.empty() == false)
        {
            const size_t numLights = lights.size();
            _parameters.setParameter("NumLights", static_cast<int>(numLights));
        
            for (size_t i = 0; i < numLights; ++i)
            {
                const auto& light = lights[i];
                
                const auto& pivot = light->getPivot();
                const std::string& prefix = "directionalLights[" + std::to_string(i) + "]";
                //_parameters.setParameter(prefix + ".origin", pivot.origin);
                _parameters.setParameter(prefix + ".lookAt", pivot.lookAt);
                //_parameters.setParameter(prefix + ".color", light->getColor());
            }
        }

        onBind(renderer);
    }

    void Shader::unbind(RendererPtr renderer)
    {
        onUnbind(renderer);
    }

    void Shader::destroy()
    {
        onDestroy();
    }

} 
}