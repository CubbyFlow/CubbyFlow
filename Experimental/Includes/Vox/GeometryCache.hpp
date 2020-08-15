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

#include <Vox/FileSystem.hpp>
#include <Vox/Vertex.hpp>
#include <Core/BoundingBox/BoundingBox3.h>
#include <Core/Vector/Vector2.h>
#include <Core/Vector/Vector3.h>
#include <Core/Array/Array1.h>
#include <Core/Array/ArrayAccessor1.h>

namespace Vox {

    class GeometryCache
    {
    public:
        class Shape 
        {
        public:
            CubbyFlow::Array1<float> interleaved;
            CubbyFlow::Array1<CubbyFlow::Vector3F> positions;
            CubbyFlow::Array1<CubbyFlow::Vector2F> texCoords;
            CubbyFlow::Array1<CubbyFlow::Vector3F> normals;
            CubbyFlow::Array1<unsigned int> indices;
            CubbyFlow::BoundingBox3F boundingBox;
            VertexFormat format { VertexFormat::None };
        };

        //! Default constructor
        GeometryCache();

        //! Constructor with path format and index.
        GeometryCache(const Vox::Path& format, size_t index = 0);

        //! Default destructor
        ~GeometryCache();

        //! Load obj file from given path and indexing for opengl specification.
        void LoadCache(const Vox::Path& format, size_t index = 0);

        //! Take Translation on the stored vertices.
        void TranslateCache(const CubbyFlow::Vector3F t);

        //! Take Scaling on the stored vertices
        //! Scaling must be a rigid-body transformation
        void ScaleCache(const float s);

        //! Returns the number of shapes
        size_t GetNumberOfShape() const;

        //! Return the shape struct of the loaded data.
        const Vox::GeometryCache::Shape& GetShape(size_t index) const;

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

        CubbyFlow::Array1<GeometryCache::Shape> _shapes;
        CubbyFlow::BoundingBox3F _boundingBox;
    };
}

#endif