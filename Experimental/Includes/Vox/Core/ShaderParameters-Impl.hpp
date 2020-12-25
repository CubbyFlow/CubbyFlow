/*************************************************************************
> File Name: ShaderParameters-Impl.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Parameters collector for shader
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_SHADERPARAMETERS_IMPL_HPP
#define CUBBYFLOW_VOX_SHADERPARAMETERS_IMPL_HPP

#include <Vox/Core/ShaderParameters.hpp>
#include <Vox/Utils/StringID.hpp>
#include <Core/Vector/Vector2.hpp>
#include <Core/Vector/Vector3.hpp>
#include <Core/Vector/Vector4.hpp>
#include <Core/Matrix/Matrix4x4.hpp>

namespace Vox {

    namespace Detail {
        ParameterType QueryTypeAndData(void* dest, int value);
        ParameterType QueryTypeAndData(void* dest, float value);
        ParameterType QueryTypeAndData(void* dest, CubbyFlow::Vector2F value);
        ParameterType QueryTypeAndData(void* dest, CubbyFlow::Vector3F value);
        ParameterType QueryTypeAndData(void* dest, CubbyFlow::Vector4F value);
        ParameterType QueryTypeAndData(void* dest, const CubbyFlow::Matrix4x4F& value);
        ParameterType QueryTypeAndData(void* dest, CubbyFlow::Matrix4x4F&& value);
    }

    template <typename T>
    void ShaderParameters::SetParameter(const std::string& name, T&& value)
    {
        CubbyFlow::Array1<unsigned char> data(sizeof(T));
        ParameterType type = Detail::QueryTypeAndData(data.data(), std::forward<T>(value));
        SetMetadata(name, ShaderParameters::Metadata(type, std::move(data)));
    }

}

#endif