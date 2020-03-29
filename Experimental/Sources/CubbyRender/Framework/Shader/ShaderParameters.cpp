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
    
    ShaderParameters::Metadata::Metadata(ParameterType type, Array1<unsigned char>&& data)
        : _type(type), _data(std::move(data))
    {
        //! Do nothing
    }

    ShaderParameters::Metadata::~Metadata()
    {
        //! Do nothing
    }
    
    ArrayAccessor1<unsigned char> ShaderParameters::Metadata::getDataAccessor()
    {
        return _data.Accessor();
    }
}
}