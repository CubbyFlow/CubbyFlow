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

    class ParameterHelper final
    {
    public:
        static size_t getNumberOfFloats(ParameterType type);
        static size_t getSizeInBytes(ParameterType type);
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
            Metadata(ParameterType type_, Array1<unsigned char>&& data_);

            //! Default destructor
            ~Metadata();
        public:
            ParameterType type;
            Array1<unsigned char> data;
        };

        using meta_table = std::unordered_map<std::string, ShaderParameters::Metadata>;
        
        void setParameter(const std::string& name, int value);
        void setParameter(const std::string& name, float value);
        void setParameter(const std::string& name, Vector2F value);
        void setParameter(const std::string& name, Vector3F value);
        void setParameter(const std::string& name, Vector4F value);
        void setParameter(const std::string& name, Matrix4x4F value);

        const meta_table& getMetatable() const;
    private:
        void setMetadata(const std::string& name, const ShaderParameters::Metadata& metadata);
        void setParameter(const std::string& name, void* bytes, ParameterType type);
        meta_table _params;
    };


}
}

#endif