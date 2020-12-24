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
#include <Vox/Material.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/Program.hpp>
#include <Vox/Texture.hpp>

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

    void Material::AttachTextureToSlot(const std::shared_ptr<Texture>& texture, unsigned int slot)
    {
        _texturePairs.Append(std::make_pair(texture, slot));
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

        _texturePairs.ForEach([&](const auto& p){
            p.first->BindTexture(p.second);
        });
    }
}