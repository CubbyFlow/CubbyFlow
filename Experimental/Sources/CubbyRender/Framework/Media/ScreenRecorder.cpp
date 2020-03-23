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

        int ScreenRecorder::saveFrame(const std::string& path, int frameIndex) const
        {
            char baseName[256];
            snprintf(baseName, sizeof(baseName), "frame_%06d.tga", frameIndex);
            std::string filename = pystring::os::path::join(path, baseName);
            std::ofstream file(filename.c_str(), std::ofstream::binary);
            if (file)
            {
                CUBBYFLOW_INFO << "Writing " << filename.c_str() << "...";

                std::array<char, 18> header;
                header.fill(0);

                const int imgWidth = static_cast<int>(_frameDimension.x);
                const int imgHeight = static_cast<int>(_frameDimension.y);

                header[2] = 2;
                header[12] = static_cast<char>(imgWidth & 0xff);
                header[13] = static_cast<char>((imgWidth & 0xff00) >> 8);
                header[14] = static_cast<char>(imgHeight & 0xff);
                header[15] = static_cast<char>((imgHeight & 0xff00) >> 8);
                header[16] = 24;

                file.write(header.data(), header.size());
                
                /*
                const auto& frame = _frames[frameIndex];

                std::vector<char> img(3 * _frameDimension.x * _frameDimension.y);
                for (size_t i = 0; i < _frameDimension.x * _frameDimension.y; ++i)
                {
                    const char val = static_cast<char>(Clamp(hdrImg[i], 0.0, 1.0) * 255.0);
                    img[3 * i + 0] = val;
                    img[3 * i + 1] = val;
                    img[3 * i + 2] = val;
                }
                file.write(img.data(), img.size());
                */
                file.close();
            }
            else
            {
                return 1;
            }

            return 0;
        }

        int ScreenRecorder::saveAllFrames(const std::string& directory) const
        {
            UNUSED_VARIABLE(directory);
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