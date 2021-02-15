/*************************************************************************
> File Name: TransparentMaterial.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose:This material uses environment mapping for rendering (transparent object like water
> Created Time: 2021/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/TransparentMaterial.hpp>

namespace Vox {

	TransparentMaterial::TransparentMaterial()
	{
		//! Do nothing
	}

	TransparentMaterial::~TransparentMaterial()
	{
		//! Do nothing.
	}

	void TransparentMaterial::ConfigureShaderParameters(ShaderParameters& params)
	{
		params.SetParameter("material.refractionRatio", this->_refractionRatio);
	}

	void TransparentMaterial::SetRefractionRatio(const float ratio)
	{
		this->_refractionRatio = ratio;
	}
}