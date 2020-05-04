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
#include <Core/Array/Array3.h>
#include <Core/Size/Size3.h>
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

        //! load image at the given path. 
        void loadImage(const std::string& path);

        //! Get the width x height of the image.
        Size3 getImageSize() const;

        //! Return the const array accessor of the image data;
        ConstArrayAccessor3<unsigned char> getImageAccessor() const;
    private:
        Array3<unsigned char> _data;
    };

    using ImageLoaderPtr = std::shared_ptr<ImageLoader>;
}
}

#endif