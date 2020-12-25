/*************************************************************************
> File Name: GeometryCache.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Geometry Cache Data Wrapper Class
> Created Time: 2020/08/11
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_GEOMETRY_CACHE_HPP
#define CUBBYFLOW_VOX_GEOMETRY_CACHE_HPP

#include <Vox/Utils/FileSystem.hpp>
#include <Vox/Scene/VoxSceneObject.hpp>
#include <Vox/Core/Vertex.hpp>
#include <Vox/Core/Mesh.hpp>
#include <Core/Geometry/BoundingBox3.hpp>
#include <Core/Vector/Vector3.hpp>
#include <Core/Array/Array1.hpp>

namespace Vox {

    class GeometryCache : public VoxSceneObject
    {
    public:
        //! Default constructor
        GeometryCache();

        //! Constructor with path format and index.
        GeometryCache(const Vox::Path& format, size_t index = 0, bool scaleToUnitBox = true);

        //! Default destructor
        ~GeometryCache();

        //! Load obj file from given path and indexing for opengl specification.
        void LoadCache(const Vox::Path& format, size_t index = 0, bool scaleToUnitBox = true);

        //! Take transform on the stored vertices.
        void TransformCache(const CubbyFlow::Vector3F t, const CubbyFlow::Vector3F s, const CubbyFlow::Vector3F axis, const float rad);

        //! Returns the number of shapes
        size_t GetNumberOfShape() const;

        //! Return the shape struct of the loaded data.
        const MeshShape& GetShape(size_t index) const;

        //! Return the bounding box of the geometry cache
        const CubbyFlow::BoundingBox3F& GetBoundingBox() const;

        //! Interleaving Vertex Buffer with given format.
        void InterleaveData(VertexFormat format);

    private:
        //! Load particle geometry cache from pos file.
        void LoadPosCache(const Vox::Path& format);

        //! Load particle geometry cache from xyz file.
        void LoadXyzCache(const Vox::Path& format);

        //! Load Obj geometry cache from obj file.
        void LoadObjCache(const Vox::Path& format);

        //! Scale loaded shape to unit box 
        void ScaleToUnitBox();

        CubbyFlow::Array1<MeshShape> _shapes;
        CubbyFlow::BoundingBox3F _boundingBox;
    };
}

#endif