/*************************************************************************
> File Name: SDFVolume.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References: http://graphicsrunner.blogspot.com/2009/01/volume-rendering-101.html
> Purpose: Signed distance field class for volume rendering with ray-casting technique.
> Created Time: 2020/08/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/SDFVolume.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/Renderer.hpp>
#include <Vox/Mesh.hpp>
#include <Vox/RenderableObject.hpp>
#include <glad/glad.h>

namespace Vox {

    SDFVolume::SDFVolume()
    {
        //! Do nothing.
    }

    SDFVolume::~SDFVolume()
    {
        //! Do nothing.
    }

    void SDFVolume::BuildVolume(const std::shared_ptr<FrameContext>& ctx, CubbyFlow::ScalarGrid3Ptr sdf)
    {
        const CubbyFlow::Point3I dim = sdf->Resolution().CastTo<ssize_t>();
        GLuint texture = Renderer::CreateVolumeTexture(dim.x, dim.y, dim.z, PixelFmt::PF_R16, nullptr);
        UNUSED_VARIABLE(texture);
        UNUSED_VARIABLE(ctx);
    }

    void SDFVolume::AttachBoundingBox(const std::shared_ptr<Mesh>& mesh)
    {
        _boundingBox.reset(new RenderableObject());
        _boundingBox->AddGeometryMesh(mesh);
    }
};