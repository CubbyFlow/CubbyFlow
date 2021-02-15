/*************************************************************************
> File Name: RenderableObject.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Static Object wrapper class.
> Created Time: 2020/08/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/RenderableObject.hpp>
#include <Vox/Utils/DebugUtils.hpp>
#include <Vox/Utils/FileSystem.hpp>
#include <Vox/Core/Material.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Mesh/Mesh.hpp>
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

    void RenderableObject::SetModelMatrix(const CubbyFlow::Matrix4x4F& model)
    {
        this->_modelMatrix = model;
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

    void RenderableObject::ClearGeometryMesh()
    {
        _meshes.Clear();
    }

    void RenderableObject::AttachMaterial(std::shared_ptr<Material> material)
    {
        _material = material;
    }

    const std::shared_ptr<Material> RenderableObject::GetMaterial() const
    {
        return _material;
    }
}