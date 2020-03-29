/*************************************************************************
> File Name: ScreenRecorder.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Recorder class for storing screenshot or simulation video.
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_SCREEN_RECORDER_H
#define CUBBYFLOW_SCREEN_RECORDER_H

#ifdef CUBBYFLOW_RECORDING

#include <Core/Array/ArrayAccessor1.h>
#include <Core/Size/Size2.h>
#include <memory>
#include <string>

using png_byte = unsigned char;

namespace CubbyFlow {
namespace CubbyRender {

    //!
    //! \brief Recorder class for storing rendering result as image or video.
    //!
    class ScreenRecorder final
    {
    public: 
        //! Default Constructor.
        ScreenRecorder();

        //! Default Destructor. 
        virtual ~ScreenRecorder();

        //! Start encoding
        void setWorkingDirectory(const std::string& directory);

        //! Encode frame into context.
        bool EncodeFrame(Size2 dim, const ArrayAccessor1<unsigned char>& pixels);

    protected:
    private:
        std::string _rootDir;
        png_byte **_pngRows = nullptr;
        int _frameCount = 0;
    };

    using ScreenRecorderPtr = std::shared_ptr<ScreenRecorder>;
} 
}

#endif
#endif