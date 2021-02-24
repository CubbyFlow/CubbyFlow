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
#include <Vox/Scene/VoxScene.hpp>

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

	void TransparentMaterial::LoadXMLNode(VoxScene* scene, const pugi::xml_node& node)
	{
		//! Parse the type of the material
		const std::string materialType = node.attribute("type").value();

		//! Get already parsed shader program from this VoxScene instance.
		//! **The required program must precede material.**
		const auto& program = scene->GetSceneObject<Program>(node.child("program").attribute("value").value());
		AttachProgramShader(program);

		//! Parse refraction ratio
		const float refractionRatio = node.child("refraction").attribute("value").as_float();
		SetRefractionRatio(refractionRatio);
	}

	void TransparentMaterial::WriteXMLNode(pugi::xml_node& node)
	{
		UNUSED_VARIABLE(node);
	}
}