/*************************************************************************
> File Name: BRDFMaterial.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose:This material uses brdf material system for shading
> Created Time: 2021/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_BRDF_MATERIAL_HPP
#define CUBBYFLOW_VOX_BRDF_MATERIAL_HPP

#include <Vox/Core/Material.hpp>

namespace Vox {

    /**
     */
    class BRDFMaterial : public Material
    {
    public:
        struct BRDF
        {
            CubbyFlow::Vector3F albedo;
            float metallic;
            float roughness;
            float ao;
        };

        //! Default constructor
        BRDFMaterial();
        //! Default destructor
        ~BRDFMaterial();

        //! Set BRDF properties of the material
        void SetBRDF(const BRDF& brdf);
    protected:
        void ConfigureShaderParameters(ShaderParameters& params) override;
        
        BRDFMaterial::BRDF _brdfProperties;
    };
}

#endif