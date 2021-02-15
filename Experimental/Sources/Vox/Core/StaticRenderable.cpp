/*************************************************************************
> File Name: StaticRenderable.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The static object class
> Created Time: 2021/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/StaticRenderable.hpp>
#include <Vox/Core/Material.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Mesh/Mesh.hpp>

namespace Vox {

    StaticRenderable::StaticRenderable()
    {
        //! Do nothing
    }

    StaticRenderable::~StaticRenderable()
    {
        //! Do nothing
    }

    void StaticRenderable::DrawRenderableObject(const std::shared_ptr<FrameContext>& ctx)
    {
        //! Configure render settings before rendering.
        ConfigureRenderSettings(ctx);

        //! Render the mesh.
        for (const auto& mesh : _meshes)
        {
            mesh->DrawMesh(ctx);
        }
    }

    void StaticRenderable::ConfigureRenderSettings(const std::shared_ptr<FrameContext>& ctx)
    {
        //! Bind material to the context.
        _material->BindMaterial(ctx);

        auto& params = _material->GetProgram()->GetParameters();
        params.SetParameter("camera.model", _modelMatrix);
    }
};