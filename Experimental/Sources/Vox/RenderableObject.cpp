/*************************************************************************
> File Name: RenderableObject.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Static Object wrapper class.
> Created Time: 2020/08/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/RenderableObject.hpp>
#include <Vox/DebugUtils.hpp>
#include <Vox/FileSystem.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/Mesh.hpp>
#include <glad/glad.h>

namespace Vox {

    RenderableObject::RenderableObject()
    {
        //! Do nothing.
    }

    RenderableObject::~RenderableObject()
    {
        //! Do nothing.
    }

    void RenderableObject::AddGeometryMesh(const std::shared_ptr<Mesh> mesh)
    {
        _meshes.Append(mesh);
    }

    const std::shared_ptr<Mesh> RenderableObject::GetGeometryMesh(size_t index) const
    {
        VoxAssert(index < _meshes.size(), CURRENT_SRC_PATH_TO_STR, "Out Of Range Error");
        return _meshes[index];
    }

    size_t RenderableObject::GetNumberOfGeometryMesh() const
    {
        return _meshes.size();
    }

    void RenderableObject::AttachProgram(const std::string& programName)
    {
        _programName = programName;
    }

    void RenderableObject::AttachTextureToSlot(const std::string& textureName, unsigned int slot)
    {
        _texturePairs.Append(std::make_pair(textureName, slot));
    }

    void RenderableObject::DrawRenderableObject(const std::shared_ptr<FrameContext>& ctx)
    {
        ctx->MakeProgramCurrent(_programName);
        _texturePairs.ForEach([&](const auto& p){
            ctx->BindTextureToSlot(p.first, GL_TEXTURE_2D, p.second);
        });

        //! Configure rendering settings such as uniform variables.
        ConfigureRenderSettings(ctx);
        
        _meshes.ForEach([&](const auto& mesh){
            mesh->DrawMesh(ctx);
        });
    }

}