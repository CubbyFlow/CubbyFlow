/*************************************************************************
> File Name: ImageLoader.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: provide functions for loading texture image and return it's format and data.
> Created Time: 2020/04/04
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Utils/ImageLoader.h>
#include <Framework/Utils/Common.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cstring>
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {

    ImageLoader::ImageLoader()
    {
        //! Do nothing
    }

    ImageLoader::~ImageLoader()
    {
        //! Do nothing
    }

    void ImageLoader::loadImage(const std::string& path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *image = stbi_load(path.c_str(), &width, &height, &channels, 0/*STBI_rgb_alpha*/); 
        
        if (image == nullptr || width == 0 || height == 0 || channels == 0)
        {
            CUBBYFLOW_ERROR << "Failed to loading image [" << path << "]";
            std::abort();
        }

        _data.Resize(width, height, channels);

        unsigned char* dst = _data.data();
        const size_t size = width * height * channels * sizeof(unsigned char);
        std::memcpy(static_cast<void*>(dst), static_cast<void*>(image), size);
    }

    Size3 ImageLoader::getImageSize() const
    {
        return _data.size();
    }

    ConstArrayAccessor3<unsigned char> ImageLoader::getImageAccessor() const
    {
        return _data.ConstAccessor();
    }

}
}