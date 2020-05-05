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

    bool ImageLoader::loadImage(const std::string& path) noexcept
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *image = stbi_load(path.c_str(), &width, &height, &channels, 0/*STBI_rgb_alpha*/); 
        //i + Width() * j + Width() * Height() * k

        if (image == nullptr || width == 0 || height == 0 || channels == 0)
        {
            return false;
        }

        _data.Resize(width, height, Vector4UB(0, 0, 0, 255));
        _data.ParallelForEachIndex([&](size_t i, size_t j){
            Vector4UB& pixel = _data.At(i, j);
            for (size_t c = 0; c < static_cast<size_t>(channels); ++c)
            {
                pixel.At(c) = image[i + static_cast<size_t>(width) * j + static_cast<size_t>(width) * static_cast<size_t>(height) * c];
            }
        });

        stbi_image_free(static_cast<void*>(image));
        return true;
    }

    Size2 ImageLoader::getImageSize() const
    {
        return _data.size();
    }

    ConstArrayAccessor2<Vector4UB> ImageLoader::getImageAccessor() const
    {
        return _data.ConstAccessor();
    }

}
}