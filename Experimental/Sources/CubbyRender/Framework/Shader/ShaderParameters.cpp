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

namespace CubbyFlow {
namespace CubbyRender {

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
}
}