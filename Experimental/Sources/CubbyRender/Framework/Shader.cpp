/*************************************************************************
> File Name: Shader.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Shader object.
> Created Time: 2020/02/19
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Shader.h>
#include <Framework/Renderer.h>
#include <Framework/ShaderPreset.h>
#include <cassert>

#include <fstream>
#include <sstream>

namespace CubbyFlow {
namespace CubbyRender {
    
    Shader::Shader()
    {
        //! Do nothing.
    }

    Shader::Shader(const std::string& shaderName)
    {
        assert( load(shaderName) == 0 );
    }

    Shader::Shader(const ShaderMap& fileMap)
    {
        assert( load(fileMap) == 0 );
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
            std::string vertexShader = kSimpleColorShaders[0];
            std::string fragmentShader = kSimpleColorShaders[1];

            shaderMap = {
                {"VertexShader", vertexShader},
                {"FragmentShader", fragmentShader}
            };
        }

        return onLoad(shaderMap);
    }

    int Shader::load(const ShaderMap& fileMap)
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
        
        return onLoad(shaderMap);
    }

} 
}