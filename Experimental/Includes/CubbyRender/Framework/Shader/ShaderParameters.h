/*************************************************************************
> File Name: ShaderParameters.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Parameters collector for shader
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_SHADERPARAMETERS_H
#define CUBBYFLOW_SHADERPARAMETERS_H

#include <Core/Array/Array1.h>
#include <Core/Vector/Vector2.h>
#include <Core/Vector/Vector3.h>
#include <Core/Vector/Vector4.h>
#include <Core/Matrix/Matrix4x4.h>
#include <memory>
#include <unordered_map>

namespace CubbyFlow {
namespace CubbyRender {

    class ShaderParameters final
    {
    public:
        enum class ParameterType
        {
            NONE     = 0,
            INT      = 1,
            FLOAT1   = 2,
            FLOAT2   = 3,
            FLOAT3   = 4,
            FLOAT4   = 5,
            FLOAT4X4 = 6
        };

        //! Default Constructor
        ShaderParameters();

        //! Default Destructor
        ~ShaderParameters();

        class Metadata
        {
            friend class ShaderParameters;
        public:
            //! Default constructor
            Metadata(ParameterType type, Array1<unsigned char>&& data);

            //! Default destructor
            ~Metadata();

            ArrayAccessor1<unsigned char> getDataAccessor();

        private:
            ParameterType _type;
            Array1<unsigned char> _data;
        };
        
        template <typename Type>
        void setParameter(const std::string& name, Type&& value);

    private:
        void setMetadata(const std::string& name, const ShaderParameters::Metadata&  metadata);

        std::unordered_map<std::string, ShaderParameters::Metadata> _params;
    };
}
}

#include <Framework/Shader/ShaderParameters-Impl.h>

#endif