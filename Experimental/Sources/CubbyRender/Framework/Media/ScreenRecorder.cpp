/*************************************************************************
> File Name: ScreenRecorder.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Recorder class for storing screenshot or simulation video.
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Media/ScreenRecorder.h>
#include <Framework/Texture.h>
#include <Core/Math/MathUtils.h>
#include <Core/Utils/Logging.h>
#include <pystring/pystring.h>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace CubbyFlow {
namespace CubbyRender {
    
        ScreenRecorder::ScreenRecorder()
        {
            //! Do nothing
        }

        ScreenRecorder::ScreenRecorder(Size2 frameDimension)
            : _frameDimension(frameDimension)
        {
            //! Do nothing
        }

        ScreenRecorder::~ScreenRecorder()
        {
        }

        void ScreenRecorder::storeFrame(Array2<Vector3B>&& frame)
        {
            _frames.emplace_back(std::move(frame));
        }

        int ScreenRecorder::saveFrame(const std::string& path, size_t frameIndex) const
        {
            assert((0UL <= frameIndex) && (frameIndex < _frames.size()));
            char baseName[256];
            snprintf(baseName, sizeof(baseName), "frame_%06d.png", static_cast<int>(frameIndex));
            std::string filename = pystring::os::path::join(path, baseName);
            
            const auto& frame = _frames[frameIndex];
            Size2 size = frame.size();
            const Vector3B* data = frame.data();

            stbi_write_png(filename.c_str(), static_cast<int>(size[0]), static_cast<int>(size[1]), 3,
                            reinterpret_cast<const void*>(data), 0);

            return 0;
        }

        int ScreenRecorder::saveMultiFrames(const std::string& path, size_t period) const
        {
            ParallelFor(ZERO_SIZE, static_cast<size_t>(_frames.size() / period), [&](size_t i)
		    {
		    	saveFrame(path, i * period);
		    });
            return 0;
        }

        int ScreenRecorder::saveAllFrames(const std::string& path) const
        {
            ParallelFor(ZERO_SIZE, _frames.size(), [&](size_t i)
		    {
		    	saveFrame(path, i);
		    });
            return 0;
        }

        size_t ScreenRecorder::getFrameCount() const
        {
            return _frames.size();
        }

        Size2 ScreenRecorder::getFrameDimension() const
        {
            return _frameDimension;
        }

        void ScreenRecorder::setFrameDimension(Size2 dim)
        {
            _frameDimension = dim;
        }

} 
}