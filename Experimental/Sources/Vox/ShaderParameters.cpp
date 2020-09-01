/*************************************************************************
> File Name: ShaderParameters.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Parameters collector for shader
> Created Time: 2020/03/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/ShaderParameters.hpp>
#include <Vox/DebugUtils.hpp>
#include <Vox/FileSystem.hpp>
#include <cstring>

namespace Vox {

    ShaderParameters::ShaderParameters()
    {
        //! Do nothing
    }
    
    ShaderParameters::~ShaderParameters()
    {
        //! Do nothing
    }

    void ShaderParameters::SetMetadata(const std::string& name, const Metadata& metadata)
    {
        auto iter = _params.find(name);

        if (iter == _params.end())  //! not found
        {
            _params.emplace(name, metadata);
        }
        else //! found
        {
            iter->second = metadata;
        }
    }

    const ShaderParameters::meta_table& ShaderParameters::GetMetatable() const
    {
        return _params;
    }
    
    ShaderParameters::Metadata::Metadata(ParameterType type_, CubbyFlow::Array1<unsigned char>&& data_)
        : type(type_), data(std::move(data_))
    {
        //! Do nothing
    }

    ShaderParameters::Metadata::~Metadata()
    {
        //! Do nothing
    }
}