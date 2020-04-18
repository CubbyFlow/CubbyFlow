/*************************************************************************
> File Name: ShaderParameters.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Parameters collector for shader
> Created Time: 2020/03/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Shader/ShaderParameters.h>
#include <Framework/Utils/Common.h>
#include <cassert>
#include <cstring>

namespace CubbyFlow {
namespace CubbyRender {

    size_t ParameterHelper::getNumberOfFloats(ParameterType type) 
    {
        size_t numFloats = 0;

        switch(type)
        {
            case ParameterType::INT      :
            {
                numFloats = 1;
                break;
            }
            case ParameterType::FLOAT1   :
            {
                numFloats = 1;
                break;
            }
            case ParameterType::FLOAT2   :
            {
                numFloats = 2;
                break;
            }
            case ParameterType::FLOAT3   :
            {
                numFloats = 3;
                break;
            }
            case ParameterType::FLOAT4   :
            {
                numFloats = 4;
                break;
            }
            case ParameterType::FLOAT4X4 :
            {
                numFloats = 16;
                break;
            }
            default :
            {
                CUBBYFLOW_ERROR << "Unknown Shader Parameter Type is given.";
                abort();
            }
        }

        return numFloats;
    }

    size_t ParameterHelper::getSizeInBytes(ParameterType type) 
    {
        return sizeof(float) * getNumberOfFloats(type);
    }

    ShaderParameters::ShaderParameters()
    {
        //! Do nothing
    }
    
    ShaderParameters::~ShaderParameters()
    {
        //! Do nothing
    }

    void ShaderParameters::setMetadata(const std::string& name, const Metadata& metadata)
    {
        auto iter = _params.find(name);

        if (iter == _params.end())  //! not found
        {
            _params.emplace(name, metadata);
        }
        else                        //! found
        {
            iter->second = metadata;
        }
    }

    const ShaderParameters::meta_table& ShaderParameters::getMetatable() const
    {
        return _params;
    }
    
    ShaderParameters::Metadata::Metadata(ParameterType type_, Array1<unsigned char>&& data_)
        : type(type_), data(std::move(data_))
    {
        //! Do nothing
    }

    ShaderParameters::Metadata::~Metadata()
    {
        //! Do nothing
    }

    void ShaderParameters::setParameter(const std::string& name, int value)
    {
        setParameter(name, reinterpret_cast<void*>(&value), ParameterType::INT);
    }
    
    void ShaderParameters::setParameter(const std::string& name, float value)
    {
        setParameter(name, reinterpret_cast<void*>(&value), ParameterType::FLOAT1);
    }
    
    void ShaderParameters::setParameter(const std::string& name, Vector2F value)
    {
        setParameter(name, reinterpret_cast<void*>(&(value.x)), ParameterType::FLOAT2);
    }
    
    void ShaderParameters::setParameter(const std::string& name, Vector3F value)
    {
        setParameter(name, reinterpret_cast<void*>(&(value.x)), ParameterType::FLOAT3);
    }
    
    void ShaderParameters::setParameter(const std::string& name, Vector4F value)
    {
        setParameter(name, reinterpret_cast<void*>(&(value.x)), ParameterType::FLOAT4);
    }
    
    void ShaderParameters::setParameter(const std::string& name, Matrix4x4F value)
    {
        setParameter(name, reinterpret_cast<void*>(value.data()), ParameterType::FLOAT4X4);
    }

    void ShaderParameters::setParameter(const std::string& name, void* bytes, ParameterType type)
    {
        const size_t sizeBytes = ParameterHelper::getSizeInBytes(type);
        Array1<unsigned char> data(sizeBytes);
        std::memcpy(static_cast<void*>(data.data()), bytes, sizeBytes);
        setMetadata(name, ShaderParameters::Metadata(type, std::move(data)));
    }

}
}