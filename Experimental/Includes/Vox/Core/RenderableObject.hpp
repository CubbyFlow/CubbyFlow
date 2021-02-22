/*************************************************************************
> File Name: RenderableObject.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Renderable object wrapper which will be rendered in main frame.
> Created Time: 2020/08/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_RENDERABLE_OBJECT_HPP
#define CUBBYFLOW_VOX_RENDERABLE_OBJECT_HPP

#include <Vox/Scene/VoxSceneObject.hpp>
#include <Core/Geometry/BoundingBox3.hpp>
#include <Core/Matrix/Matrix4x4.hpp>
#include <Core/Array/Array1.hpp>
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
        virtual ~RenderableObject();
        //! Returns the geometry mesh of the object with given index.
        const std::shared_ptr<Mesh> GetGeometryMesh(size_t index) const;
        //! Returns the number of the contained geometry mesh.
        size_t GetNumberOfGeometryMesh() const;
        //! Returns the material of the object.
        const std::shared_ptr<Material> GetMaterial() const;
        //! Returns the bounding box of this renderable object.
        CubbyFlow::BoundingBox3F GetBoundingBox() const;
        //! Set model matrix
        void SetModelMatrix(const CubbyFlow::Matrix4x4F& model);
        //! Append Geometry mesh instance to the array..
        void AddGeometryMesh(const std::shared_ptr<Mesh> mesh);
        //! Clear the geometry mesh list
        void ClearGeometryMesh();
        //! Set the material of the renderable object.
        void AttachMaterial(std::shared_ptr<Material> material);
        //! Draw this renderable object with configured settings.
        virtual void DrawRenderableObject(const std::shared_ptr<FrameContext>& ctx) { (void)ctx; };
    protected:
        //! Configure render settings, such as material bounding, buffer binding and etc..
        virtual void ConfigureRenderSettings(const std::shared_ptr<FrameContext>& ctx) {(void)ctx;};
        
        CubbyFlow::Matrix4x4F _modelMatrix; // local-to-world matrix which will be calculated in shader
        CubbyFlow::BoundingBox3F _boundingBox; // bounding box of the whole meshes
        CubbyFlow::Array1<std::shared_ptr<Mesh>> _meshes; // whole meshes of this renderable object
        std::shared_ptr<Material> _material; // material of this renderable object
    };
}

#endif