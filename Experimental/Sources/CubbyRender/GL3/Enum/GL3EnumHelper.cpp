/*************************************************************************
> File Name: GL3EnumHelper.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the texture parameter struct for building texture.
> Created Time: 2020/05/07
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <GL3/Enum/GL3EnumHelper.h>

#ifdef CUBBYFLOW_USE_GL
#include <GL3/Utils/GL3Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    const GLenum GL3_IMAGE_FORMAT[] = {
#define X(a) (a),
#include <GL3/Enum/GL3ImageFormatEnums.def>
#undef X
    };
    
    const GLenum GL3_IMAGE_INTERNAL_FORMAT[] = {
#define X(a) (a),
#include <GL3/Enum/GL3ImageInternalFormatEnums.def>
#undef X
    };

    const GLint GL3_IMAGE_SAMPLING_MODE[] = {
#define X(a) (a),
#include <GL3/Enum/GL3ImageSamplingModeEnums.def>
#undef X
    };

    const GLint GL3_IMAGE_WRAP_METHOD[] = {
#define X(a) (a),
#include <GL3/Enum/GL3ImageWrapMethodEnums.def>
#undef X
    };

    const GLuint GL3_DATA_TYPE[] = {
#define X(a) (a),
#include <GL3/Enum/GL3DataTypeEnums.def>
#undef X
    };

    GLenum GL3EnumHelper::ImageFormatConverter(ImageFormat format)
    {
        return GL3_IMAGE_FORMAT[static_cast<size_t>(format)];
    }

    GLenum GL3EnumHelper::ImageInternalFormatConverter(ImageInternalFormat internalFormat)
    {
        return GL3_IMAGE_INTERNAL_FORMAT[static_cast<size_t>(internalFormat)];
    }

    GLint GL3EnumHelper::ImageSamplingModeConverter(ImageSamplingMode samplingMode)
    {
        return GL3_IMAGE_SAMPLING_MODE[static_cast<size_t>(samplingMode)];
    }

    GLint GL3EnumHelper::ImageWrapMethodConverter(ImageWrapMethod wrapMethod)
    {
        return GL3_IMAGE_WRAP_METHOD[static_cast<size_t>(wrapMethod)];
    }

    GLuint GL3EnumHelper::DataTypeConverter(DataType type)
    {
        return GL3_DATA_TYPE[static_cast<size_t>(type)];
    }


} 
} 

#endif