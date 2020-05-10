/*************************************************************************
> File Name: GL3EnumHelper.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the texture parameter struct for building texture.
> Created Time: 2020/05/07
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL3_ENUM_HELPER_H
#define CUBBYFLOW_GL3_ENUM_HELPER_H

#include <Framework/Enum/ImageEnums.h>

#ifdef CUBBYFLOW_USE_GL

#include <GL3/Utils/GL3Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    class GL3EnumHelper
    {
    public:
        static GLenum ImageFormatConverter(ImageFormat format);
        static GLenum ImageInternalFormatConverter(ImageInternalFormat internalFormat);
        static GLint ImageSamplingModeConverter(ImageSamplingMode samplingMode);
        static GLint ImageWrapMethodConverter(ImageWrapMethod wrapMethod);
        static GLuint DataTypeConverter(DataType type);
    };

} 
} 

#endif
#endif 