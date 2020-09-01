/*************************************************************************
> File Name: ShaderParameters-Impl.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Parameters collector for shader
> Created Time: 2020/03/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/ShaderParameters-Impl.hpp>
#include <cstring>

namespace Vox {

    ParameterType Detail::QueryTypeAndData(void* dest, int value)
    {
        std::memcpy(dest, &value, sizeof(int));
        return ParameterType::INT;
    }
    
    ParameterType Detail::QueryTypeAndData(void* dest, float value)
    {
        std::memcpy(dest, &value, sizeof(float));
        return ParameterType::FLOAT1;
    }
    
    ParameterType Detail::QueryTypeAndData(void* dest, CubbyFlow::Vector2F value)
    {
        std::memcpy(dest, &(value.x), sizeof(CubbyFlow::Vector2F));
        return ParameterType::FLOAT2;
    }
    
    ParameterType Detail::QueryTypeAndData(void* dest, CubbyFlow::Vector3F value)
    {
        std::memcpy(dest, &(value.x), sizeof(CubbyFlow::Vector3F));
        return ParameterType::FLOAT3;
    }
    
    ParameterType Detail::QueryTypeAndData(void* dest, CubbyFlow::Vector4F value)
    {
        std::memcpy(dest, &(value.x), sizeof(CubbyFlow::Vector4F));
        return ParameterType::FLOAT4;
    }

    ParameterType Detail::QueryTypeAndData(void* dest, const CubbyFlow::Matrix4x4F& value)
    {
        std::memcpy(dest, value.data(), sizeof(CubbyFlow::Matrix4x4F));
        return ParameterType::FLOAT4X4;
    }

    ParameterType Detail::QueryTypeAndData(void* dest, CubbyFlow::Matrix4x4F&& value)
    {
        auto moveVal = std::move(value);
        std::memcpy(dest, moveVal.data(), sizeof(CubbyFlow::Matrix4x4F));
        return ParameterType::FLOAT4X4;
    }
}