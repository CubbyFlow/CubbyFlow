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

#include <Vox/VoxSceneObject.hpp>
#include <Core/Array/Array1.h>
#include <string>
#include <memory>

namespace Vox {

    class Mesh;
    class FrameContext;
    class VoxScene;

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
        //! Returns the number of the contained geometry mesh.
        size_t GetNumberOfGeometryMesh() const;
        //! Set the program shader name.
        void AttachProgram(const std::string& programName);
        //! Set the texture name to slot
        void AttachTextureToSlot(const std::string& textureName, unsigned int slot);
        //! Draw this renderable object with configured settings.
        void DrawRenderableObject(const std::shared_ptr<FrameContext>& ctx);
    protected:
        virtual void ConfigureRenderSettings(const std::shared_ptr<FrameContext>& ctx) = 0;
        CubbyFlow::Array1<std::shared_ptr<Mesh>> _meshes;
        CubbyFlow::Array1<std::pair<std::string, unsigned int>> _texturePairs;
        std::string _programName;
    private:
    };
}

#endif