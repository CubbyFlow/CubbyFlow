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
#include <Vox/Core/Texture.hpp>
#include <Vox/Scene/VoxScene.hpp>

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
        _textures[slot] = texture;
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
        //! Set the render status of the material to the current context.
        ctx->SetRenderStatus(_renderStatus);

        //! Bind the shader program to the current context.
        _program->BindProgram(ctx);
        
        //! Upload the shader parameters to the current bound context.
        ConfigureShaderParameters(_program->GetParameters());

        //! Bind all attached textures to the specified slot.
        for (const auto& texturePair : _textures)
        {
            texturePair.second->BindTexture(texturePair.first);
        }
    }

    void Material::LoadXMLNode(VoxScene* scene, const pugi::xml_node& node)
    {
        //! Parse the type of the material
        const std::string materialType = node.attribute("type").value();

        //! Get already parsed shader program from this VoxScene instance.
        //! **The required program must precede material.**
        const auto& program = scene->GetSceneObject<Program>(node.child("program").attribute("value").value());
        AttachProgramShader(program);
    }

    void Material::WriteXMLNode(pugi::xml_node& node)
    {
        UNUSED_VARIABLE(node);
    }
}