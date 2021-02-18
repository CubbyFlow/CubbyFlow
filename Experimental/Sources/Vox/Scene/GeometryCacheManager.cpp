/*************************************************************************
> File Name: GeometryCacheManager.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Resource manager class.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Scene/GeometryCacheManager.hpp>
#include <Vox/Utils/DebugUtils.hpp>
#include <Vox/Scene/GeometryCache.hpp>
#include <Core/Utils/Parallel.hpp>

namespace Vox {

    GeometryCacheManager::GeometryCacheManager()
        : VoxSceneObject()
    {
        _boundingBox.Reset();
    }

    GeometryCacheManager::GeometryCacheManager(const Vox::Path& format, size_t count)
        : GeometryCacheManager()
    {
        PoolingGeometryCache(format, count);
    }

    GeometryCacheManager::~GeometryCacheManager()
    {
        //! Do nothing.
    }

    void GeometryCacheManager::PoolingGeometryCache(const Vox::Path& format, size_t count)
    {
        _caches.Resize(count);
        CubbyFlow::ParallelFor(CubbyFlow::ZERO_SIZE, count, [&](size_t index){
            _caches[index] = std::make_shared<GeometryCache>(format, index);
        });

        for (const auto& cache : _caches)
        {
            _boundingBox.Merge(cache->GetBoundingBox());
        }
    }

    size_t GeometryCacheManager::GetNumberOfCache() const
    {
        return _caches.size();
    }

    const std::shared_ptr<GeometryCache>& GeometryCacheManager::GetGeometryCache(size_t index) const
    {
        VoxAssert((index < _caches.size()), CURRENT_SRC_PATH_TO_STR, "Out Of Range Error");
        return _caches[index];
    }

    const CubbyFlow::BoundingBox3F& GeometryCacheManager::GetBoundingBox() const
    {
        return _boundingBox;
    }

    void GeometryCacheManager::SetVertexFormat(VertexFormat format)
    {
        this->_format = format;
    }

    VertexFormat GeometryCacheManager::GetVertexFormat() const
    {
        return this->_format;
    }
};