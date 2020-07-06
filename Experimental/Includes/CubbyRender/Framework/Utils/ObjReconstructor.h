/*************************************************************************
> File Name: ObjReconstructor.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Useful macros and functions collection.
> Created Time: 2020/04/04
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_OBJRECONSTRUCTOR_H
#define CUBBYFLOW_OBJRECONSTRUCTOR_H

#include <Framework/Buffer/Vertex.h>
#include <Framework/Utils/Prerequisites.h>
#include <Core/Vector/Vector3.h>
#include <Core/Array/Array1.h>
#include <string>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {

    class ObjReconstructor final
    {
    public:
        //! Default constructor
        ObjReconstructor();

        //! Constructor with obj file path.
        ObjReconstructor(const std::string& objPath, VertexFormat format);

        //! Default destructor
        ~ObjReconstructor();

        //! Set vertex format to be constructed.
        void setVertexFormat(VertexFormat format);

        //! Get vertex format of the obj.
        VertexFormat getVertexFormat() const;

        //! Get immutable vertices
        const Array1<float>& getVertices() const;

        //! Get imuutable indices 
        const Array1<unsigned int>& getIndices() const;

        //! Load obj file from given path and reconstruct it 
        //! If Obj loading is failed, return false. otherwise return true.
        bool loadAndReconstruct(const std::string& objPath);

        //! Get left top and right bottom corner for the bounding box which wrap the object.
        std::tuple<Vector3F, Vector3F> getBoundingBox() const;
    private:
        Array1<float> _vertices;
        Array1<unsigned int> _indices;
        Vector3F _bbMin;
        Vector3F _bbMax;
        VertexFormat _format = VertexFormat::Position3Normal3;
    };
    using ObjReconstructorPtr = std::shared_ptr<ObjReconstructor>;
}
}

#endif