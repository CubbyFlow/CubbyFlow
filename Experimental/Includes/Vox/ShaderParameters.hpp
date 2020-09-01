/*************************************************************************
> File Name: ShaderParameters.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Parameters collector for shader
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_SHADERPARAMETERS_HPP
#define CUBBYFLOW_VOX_SHADERPARAMETERS_HPP

#include <Core/Array/Array1.hpp>
#include <memory>
#include <unordered_map>

namespace Vox {

    enum class ParameterType
    {
        NONE = 0,
        INT,
        FLOAT1,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        FLOAT4X4,
    };

    class ShaderParameters final
    {
    public:
        //! Default Constructor
        ShaderParameters();

        //! Default Destructor
        ~ShaderParameters();

        class Metadata
        {
        public:
            //! Default Constructor
            Metadata() = default;

            //! Default constructor
            Metadata(ParameterType type_, CubbyFlow::Array1<unsigned char>&& data_);

            //! Default destructor
            ~Metadata();
        public:
            ParameterType type;
            CubbyFlow::Array1<unsigned char> data;
        };

        using meta_table = std::unordered_map<std::string, ShaderParameters::Metadata>;
        
        template <typename T>
        void SetParameter(const std::string& name, T&& value);

        const meta_table& GetMetatable() const;
    private:
        void SetMetadata(const std::string& name, const ShaderParameters::Metadata& metadata);
        
        meta_table _params;
    };
}

#include <Vox/ShaderParameters-Impl.hpp>

#endif