/*************************************************************************
> File Name: ScreenRecorder.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Recorder class for storing screenshot or simulation video.
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

//! reference  : https://github.com/cirosantilli/cpp-cheat/blob/70b22ac36f92e93c94f951edb8b5af7947546525/opengl/offscreen.c
#ifdef CUBBYFLOW_RECORDING

#include <Framework/Media/ScreenRecorder.h>
#include <Core/Utils/Logging.h>
#include <Core/Math/MathUtils.h>
#include <Core/Utils/Logging.h>
#include <pystring/pystring.h>
#include <fstream>
#include <cstdio>

extern "C" 
{
    #include <png.h>
}

namespace CubbyFlow {
namespace CubbyRender {
    
        ScreenRecorder::ScreenRecorder()
        {
            //! Do nothing
        }

        ScreenRecorder::~ScreenRecorder()
        {
            if (_pngRows)
            {
                free(_pngRows);
            }
        }

        void ScreenRecorder::setWorkingDirectory(const std::string& directory)
        {
            _rootDir = directory;
        }

        bool ScreenRecorder::EncodeFrame(Size2 dim, const ArrayAccessor1<unsigned char>& pixels)
        {
            constexpr size_t kNumChannels = 4;
            size_t width = dim[0], height = dim[1];

            char baseName[256];
            snprintf(baseName, sizeof(baseName), "frame_%04d.png", _frameCount);
            std::string filename = pystring::os::path::join(_rootDir, baseName);
            FILE *file = fopen(filename.c_str(), "wb");

            if (file)
            {
                _pngRows = static_cast<png_byte**>(realloc(_pngRows, height * sizeof(png_byte*)));
                png_byte* const pixelBytes = pixels.data();
                
                ParallelFor(ZERO_SIZE, height, [&](size_t i){
                    _pngRows[height - i - 1] = &(pixelBytes[i * width * kNumChannels]);
                });

                png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
                if (!png) 
                {
                    CUBBYFLOW_ERROR << "png_create_write_struct error";
                    return false;
                }

                png_infop info = png_create_info_struct(png);
                if (!info)
                {
                    CUBBYFLOW_ERROR << "png_create_info_struct error";
                    return false;
                }

                if (setjmp(png_jmpbuf(png)))
                {
                    CUBBYFLOW_ERROR << "setjmp(png_jmpbuf(png)) error";
                    return false;
                }

                png_init_io(png, file);
                png_set_IHDR(
                    png,
                    info,
                    width,
                    height,
                    8,
                    PNG_COLOR_TYPE_RGBA,
                    PNG_INTERLACE_NONE,
                    PNG_COMPRESSION_TYPE_DEFAULT,
                    PNG_FILTER_TYPE_DEFAULT
                );
                png_write_info(png, info);
                png_write_image(png, _pngRows);
                png_write_end(png, NULL);
                png_destroy_write_struct(&png, &info); 

                fclose(file);
                ++_frameCount;
                return true;
            }
            else
            {
                return false;
            }
        }
} 
}

#endif