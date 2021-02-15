/*************************************************************************
> File Name: BRDFMaterial.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose:This material uses brdf material system for shading
> Created Time: 2021/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/BRDFMaterial.hpp>

namespace Vox {

    BRDFMaterial::BRDFMaterial()
    {
        //! Do nothing
    }

    BRDFMaterial::~BRDFMaterial()
    {
        //! Do nothing
    }
    
    void BRDFMaterial::ConfigureShaderParameters(ShaderParameters& params)
    {
        params.SetParameter("material.albedo", _brdfProperties.albedo);
        params.SetParameter("material.metallic", _brdfProperties.metallic);
        params.SetParameter("material.roughness", _brdfProperties.roughness);
        params.SetParameter("material.ao", _brdfProperties.ao);
    }

    void BRDFMaterial::SetBRDF(const BRDF& brdf)
    {
        this->_brdfProperties = brdf;
    }
}