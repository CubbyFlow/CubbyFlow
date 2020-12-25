/*************************************************************************
> File Name: RenderableObject.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Static Object wrapper class.
> Created Time: 2020/08/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_RENDERABLE_OBJECT_HPP
#define CUBBYFLOW_VOX_RENDERABLE_OBJECT_HPP

#include <Vox/Scene/VoxSceneObject.hpp>
#include <Core/Array/Array1.hpp>
#include <string>
#include <memory>

namespace Vox {

    class Mesh;
    class FrameContext;
    class Material;

    class RenderableObject : public VoxSceneObject
    {
    public:
        //! Default Constructor
        RenderableObject();
        //! Default Destructor.
        ~RenderableObject();
        
        //! Append Geometry mesh instance to the array..
        void AddGeometryMesh(const std::shared_ptr<Mesh> mesh);
        //! Returns the geometry mesh of the object with given index.
        const std::shared_ptr<Mesh> GetGeometryMesh(size_t index) const;
        //! Clear the geometry mesh list
        void ClearGeometryMesh();
        //! Returns the number of the contained geometry mesh.
        size_t GetNumberOfGeometryMesh() const;
        //! Set the material of the renderable object.
        void AttachMaterial(std::shared_ptr<Material> material);
        //! Returns the material of the object.
        const std::shared_ptr<Material> GetMaterial() const;
        //! Draw this renderable object with configured settings.
        void DrawRenderableObject(const std::shared_ptr<FrameContext>& ctx);
    protected:
        virtual void ConfigureRenderSettings(const std::shared_ptr<FrameContext>& ctx) {(void)ctx;};
        
        CubbyFlow::Array1<std::shared_ptr<Mesh>> _meshes;
        std::shared_ptr<Material> _material;
    private:
    };
}

#endif