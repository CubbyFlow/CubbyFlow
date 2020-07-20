/*************************************************************************
> File Name: ImageLoader.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: provide functions for loading texture image and return it's format and data.
> Created Time: 2020/04/04
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_IMAGELOADER_H
#define CUBBYFLOW_IMAGELOADER_H

#include <Framework/Utils/Prerequisites.h>
#include <Core/Array/Array2.h>
#include <Core/Size/Size2.h>
#include <Core/Vector/Vector4.h>
#include <string>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {

    class ImageLoader final
    {
    public:
        //! Default constructor
        ImageLoader();

        //! Default destructor
        ~ImageLoader();

        //! write image with the given data and size.
        void writeImage(const std::string& path, void* data, Size2 size);

        //! load image at the given path. 
        bool loadImage(const std::string& path) noexcept;

        //! Get the width x height of the image.
        Size2 getImageSize() const;

        //! Return the mutable array accessor of the image data;
        ArrayAccessor2<Vector4UB> getImageAccessor();

        //! Return the const array accessor of the image data;
        ConstArrayAccessor2<Vector4UB> getConstImageAccessor() const;

        //! return the raw pointer of the first element;
        void* data();
    private:
        Array2<Vector4UB> _data;
    };

    using ImageLoaderPtr = std::shared_ptr<ImageLoader>;
}
}

#endif