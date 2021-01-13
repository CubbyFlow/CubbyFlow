/*************************************************************************
> File Name: Material.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References : https://google.github.io/filament/Materials.html
> Purpose: 
> Created Time: 2020/09/05
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/Material.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Core/Texture.hpp>

namespace Vox {

    Material::Material()
    {
        //! Do nothing
    }

    Material::~Material()
    {
        //! Do nothing
    }

    void Material::AttachProgramShader(const std::shared_ptr<Program>& program)
    {
        _program = program;
    }

    void Material::SetBRDF(const BRDF& brdf)
    {
        _brdfProperties = brdf;
        auto& params = _program->GetParameters();
        params.SetParameter("material.albedo", _brdfProperties.albedo);
        params.SetParameter("material.metallic", _brdfProperties.metallic);
        params.SetParameter("material.roughness", _brdfProperties.roughness);
        params.SetParameter("material.ao", _brdfProperties.ao);
    }

    FrameContext::RenderStatus Material::GetRenderStatus() const
	{
		return _renderStatus;
	}

    void Material::SetRenderStatus(FrameContext::RenderStatus newRenderStatus)
	{	
		_renderStatus = newRenderStatus;
	}

    void Material::BindMaterial(const std::shared_ptr<FrameContext>& ctx)
    {
        ctx->SetRenderStatus(_renderStatus);
        _program->BindProgram(ctx->GetContextScene());
    }
}