/*************************************************************************
> File Name: ShaderParameters-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Parameters collector for shader
> Created Time: 2020/03/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_SHADERPARAMETERS_IMPL_H
#define CUBBYFLOW_SHADERPARAMETERS_IMPL_H

#include <Framework/Utils/Common.h>
#include <cstring>
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {

    template <typename Type>
    ShaderParameters::Metadata buildMetadata(Type&& value)
    {
        using ParameterType = ShaderParameters::ParameterType;
        ParameterType type = ParameterType::NONE;
        size_t sizeBytes = 0U;

        if (std::is_same<Type, int>::value)
        {
            type = ParameterType::INT;
            sizeBytes = 4U;
        }
        else if (std::is_same<Type, float>::value)
        {
            type = ParameterType::FLOAT1;
            sizeBytes = 4U;
        }
        else if (std::is_same<Type, Vector2F>::value)
        {
            type = ParameterType::FLOAT2;
            sizeBytes = 8U;
        }
        else if (std::is_same<Type, Vector3F>::value)
        {
            type = ParameterType::FLOAT4;
            sizeBytes = 12U;
        }
        else if (std::is_same<Type, Vector4F>::value)
        {
            type = ParameterType::FLOAT4;
            sizeBytes = 16U;
        }
        else if (std::is_same<Type, Matrix4x4F>::value)
        {
            type = ParameterType::FLOAT4X4;
            sizeBytes = 64U;
        }
        else
        {
            CUBBYFLOW_ERROR << "Unknown ShaderParamter::ParameterType";
            abort();
        }
        
        Array1<unsigned char> data(sizeBytes);
        std::memcpy(static_cast<void*>(data.data()), static_cast<void*>(&value), sizeBytes);
        return ShaderParameters::Metadata(type, std::move(data));
    }

    template <typename Type>
    void ShaderParameters::setParameter(const std::string& name, Type&& value)
    {
        setMetadata(name, buildMetadata(std::forward<Type>(value)));
    }   
}
}

#endif