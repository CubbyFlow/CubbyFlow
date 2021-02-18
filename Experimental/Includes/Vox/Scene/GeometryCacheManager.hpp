/*************************************************************************
> File Name: GeometryCacheManager.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Resource manager class.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_GEOMETRY_CACHE_MANAGER_HPP
#define CUBBYFLOW_VOX_GEOMETRY_CACHE_MANAGER_HPP

#include <Vox/Scene/VoxSceneObject.hpp>
#include <Vox/Core/Vertex.hpp>
#include <Vox/Utils/FileSystem.hpp>
#include <Core/Array/Array1.hpp>
#include <Core/Geometry/BoundingBox3.hpp>
#include <string>

namespace Vox {

    class GeometryCache;

    class GeometryCacheManager : public VoxSceneObject
    {
    public:
        //! Default Constructor.
        GeometryCacheManager();

        //! Constructor with geometry cache path format and count
        GeometryCacheManager(const Vox::Path& format, size_t count);

        //! Default Destructor.
        ~GeometryCacheManager();

        //! Make geometry cache with the given count argument,
        //! call cache loading method to them
        void PoolingGeometryCache(const Vox::Path& format, size_t count);

        //! return the number of the resources.
        size_t GetNumberOfCache() const;

        //! Return the const accessor of the geometry cache.
        const std::shared_ptr<GeometryCache>& GetGeometryCache(size_t index) const;

        //! Return the bounding box of the whole geometry caches
        const CubbyFlow::BoundingBox3F& GetBoundingBox() const;

        //! Set vertex format of the geometry caches.
        void SetVertexFormat(VertexFormat format);

        //! Get vertex format of the geometry caches.
        VertexFormat GetVertexFormat() const;

    protected:
    private:
        CubbyFlow::Array1<std::shared_ptr<Vox::GeometryCache>> _caches;
        CubbyFlow::BoundingBox3F _boundingBox;
        VertexFormat _format;
    };
};

#endif