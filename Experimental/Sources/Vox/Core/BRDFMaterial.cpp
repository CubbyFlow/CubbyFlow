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
#include <Vox/Scene/VoxScene.hpp>
#include <Vox/Utils/VectorUtils.hpp>

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

    void BRDFMaterial::LoadXMLNode(VoxScene* scene, const pugi::xml_node& node)
    {
        //! Parse the type of the material
        const std::string materialType = node.attribute("type").value();

        //! Get already parsed shader program from this VoxScene instance.
        //! **The required program must precede material.**
        const auto& program = scene->GetSceneObject<Program>(node.child("program").attribute("value").value());
        AttachProgramShader(program);

        //! Parse brdf material properties
        BRDFMaterial::BRDF brdf;
        brdf.albedo = ParseFromString<float, 3>(node.child("brdf").child("albedo").attribute("value").value());
        brdf.metallic = node.child("brdf").child("metallic").attribute("value").as_float();
        brdf.roughness = node.child("brdf").child("roughness").attribute("value").as_float();
        brdf.ao = node.child("brdf").child("ao").attribute("value").as_float();

        //! Send brdf properties to the material
        SetBRDF(brdf);
    }

    void BRDFMaterial::WriteXMLNode(pugi::xml_node& node)
    {
        UNUSED_VARIABLE(node);
    }
}