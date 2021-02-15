/*************************************************************************
> File Name: TransparentMaterial.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose:This material uses environment mapping for rendering (transparent object like water
> Created Time: 2021/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_TRANSPARENT_MATERIAL_HPP
#define CUBBYFLOW_VOX_TRANSPARENT_MATERIAL_HPP

#include <Vox/Core/Material.hpp>

namespace Vox {

    /**
     */
    class TransparentMaterial : public Material
    {
    public:
        //! Default constructor
        TransparentMaterial();
        //! Default destructor
        ~TransparentMaterial();

        //! Set the refraction ratio (water is 1.33)
        void SetRefractionRatio(const float ratio);

    protected:
        void ConfigureShaderParameters(ShaderParameters& params) override;

        float _refractionRatio { 1.0f };
    };
}

#endif