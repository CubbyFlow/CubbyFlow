/*************************************************************************
> File Name: GeometryCacheParser.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Fluid data parser (pos, xyz)
> Created Time: 2020/06/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GEOMETRY_CACHE_PARSER_H
#define CUBBYFLOW_GEOMETRY_CACHE_PARSER_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Scene/SceneObject.h>
#include <Core/Array/Array1.h>
#include <Core/Array/ArrayAccessor1.h>
#include <Core/Vector/Vector3.h>
#include <string>
#include <vector>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    class GeometryCacheParser : public SceneObject
    {
    public:
        //! Default Constructor.
        GeometryCacheParser();

        //! Default Destructor.
        ~GeometryCacheParser();

        //! load multiple fluid files from given directory.
        void loadGeometryCache(const std::string& format, size_t count);

        //! return mutable reference of particle vector of one cache.
        ArrayAccessor1<float> getVertexCache(size_t index);

        //! return immutable reference of particle vector of one cache.
        ConstArrayAccessor1<float> getVertexCache(size_t index) const;

        //! return mutable reference of particle vector of one cache.
        ArrayAccessor1<unsigned int> getIndexCache(size_t index);

        //! return immutable reference of particle vector of one cache.
        ConstArrayAccessor1<unsigned int> getIndexCache(size_t index) const;

        //! return the number of particle state(number of particle files)
        size_t getNumberOfGeometryCache() const;
    protected:
        //! load geometry cache from pos file.
        void loadPosFile(const std::string& path);
        //! load geometry cache from xyz file.
        void loadXyzFile(const std::string& path);
        //! load geometry cache from obj file.
        void loadObjFile(const std::string& path);
    private:
        std::vector<Array1<float>> _vertexCache;
        std::vector<Array1<unsigned int>> _indexCache;
    };

    using GeometryCacheParserPtr = std::shared_ptr<GeometryCacheParser>;
};
};


#endif