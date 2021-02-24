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
    {
        _boundingBox.Reset();
    }

    GeometryCacheManager::GeometryCacheManager(const Vox::Path& format, size_t count, bool scaleToUnitBox)
        : GeometryCacheManager()
    {
        PoolingGeometryCache(format, count, scaleToUnitBox);
    }

    GeometryCacheManager::~GeometryCacheManager()
    {
        //! Do nothing.
    }

    void GeometryCacheManager::PoolingGeometryCache(const Vox::Path& format, size_t count, bool scaleToUnitBox)
    {
        _caches.Resize(count);

        //! Turn off the scaling to unit box for calculating the bounding box of the total animation, not for one cache.
        CubbyFlow::ParallelFor(CubbyFlow::ZERO_SIZE, count, [&](size_t index){
            _caches[index] = std::make_shared<GeometryCache>(format, index, false);
        });

        for (const auto& cache : _caches)
        {
            _boundingBox.Merge(cache->GetBoundingBox());
        }

        if (scaleToUnitBox)
        {
            const auto& minCorner = _boundingBox.lowerCorner;
            const auto& maxCorner = _boundingBox.upperCorner;
            const auto& delta = maxCorner - minCorner;
            const float maxLengthHalf = std::max({ delta.x, delta.y, delta.z }) / 2.0f;

            CubbyFlow::ParallelFor(CubbyFlow::ZERO_SIZE, count, [&](size_t index) {
                auto& cache = _caches[index];
                for (size_t i = 0; i < cache->GetNumberOfShape(); ++i)
                {
                    auto& shape = cache->GetShape(i);
                    for (auto& vertex : shape.positions)
                    {
                        vertex -= minCorner;
                        vertex /= maxLengthHalf;
                        vertex -= 1.0f;

                        //! TODO(snowapril) : below code must be removed after camera modifying
                        vertex.z += 1.5f;
                    }
                }
            });
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