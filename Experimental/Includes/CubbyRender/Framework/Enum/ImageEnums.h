/*************************************************************************
> File Name: ImageEnums.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the enums for image which will be converted to api specific enum
> Created Time: 2020/05/08
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifndef CUBBYFLOW_IMAGE_ENUMS_H
#define CUBBYFLOW_IMAGE_ENUMS_H

#include <Framework/Utils/Prerequisites.h>
#include <cstdint>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {

    enum class ImageFormat : unsigned char
    {
#define X(a, b) a = b,
#include <Framework/Enum/ImageFormatEnums.def>
#undef X
    };

    const std::string IMAGE_FORMAT_STR[] = {
#define X(a, b) #a,
#include <Framework/Enum/ImageFormatEnums.def>
#undef X
    };
    
    enum class ImageInternalFormat : unsigned char
    {
#define X(a, b) a = b,
#include <Framework/Enum/ImageInternalFormatEnums.def>
#undef X
    };

    const std::string IMAGE_INTERNAL_FORMAT_STR[] = {
#define X(a, b) #a,
#include <Framework/Enum/ImageInternalFormatEnums.def>
#undef X
    };

    enum class ImageSamplingMode : unsigned char
    {
#define X(a, b) a = b,
#include <Framework/Enum/ImageSamplingModeEnums.def>
#undef X
    };

    const std::string IMAGE_SAMPLING_MODE_STR[] = {
#define X(a, b) #a,
#include <Framework/Enum/ImageSamplingModeEnums.def>
#undef X
    };

    enum class ImageWrapMethod : unsigned char
    {
#define X(a, b) a = b,
#include <Framework/Enum/ImageWrapMethodEnums.def>
#undef X
    };

    const std::string IMAGE_WRAP_METHOD_STR[] = {
#define X(a, b) #a,
#include <Framework/Enum/ImageWrapMethodEnums.def>
#undef X
    };

    enum class DataType : unsigned char
    {
#define X(a, b) a = b,
#include <Framework/Enum/DataTypeEnums.def>
#undef X
    };

    const std::string DATA_TYPE_STR[] = {
#define X(a, b) #a,
#include <Framework/Enum/DataTypeEnums.def>
#undef X
    };

} 
} 

#endif 