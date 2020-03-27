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

#include <iostream>

extern "C" 
{
    #include <libavcodec/avcodec.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/opt.h>
    #include <libswscale/swscale.h>
}

namespace CubbyFlow {
namespace CubbyRender {
    
        ScreenRecorder::ScreenRecorder()
        {
            //! Do nothing
        }

        ScreenRecorder::ScreenRecorder(size_t width, size_t height)
        {
            SetSize(width, height);
        }

        ScreenRecorder::ScreenRecorder(Size2 frameDimension)
        {
            SetSize(frameDimension);
        }

        ScreenRecorder::~ScreenRecorder()
        {
            std::cout << "Destruncted screenrecorder" << std::endl;
        }

        bool ScreenRecorder::StartEncode(const std::string& filename, int fps)
        {
            AVCodec *codec;
            int ret;

            codec = avcodec_find_encoder(AV_CODEC_ID_MPEG1VIDEO);
            if (!codec)
            {
                CUBBYFLOW_ERROR << "Codec not found";
                return false;
            }

            _codecContext = avcodec_alloc_context3(codec);
            if (!_codecContext)
            {
                CUBBYFLOW_ERROR << "Could not allocate video codec context";
                return false;
            }

            _codecContext->bit_rate = 400000;
            _codecContext->width = Width();
            _codecContext->height = Height();
            _codecContext->time_base.num = 1;
            _codecContext->time_base.den = fps;
            _codecContext->gop_size = 10;
            _codecContext->max_b_frames = 1;
            _codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
            
            if(avcodec_open2(_codecContext, codec, NULL) < 0)
            {
                CUBBYFLOW_ERROR << "Could not open codec";
                return false;
            }

            _pkt = av_packet_alloc();
            if (!_pkt)
            {
                CUBBYFLOW_ERROR << "Could not allocate packet";
                return false;
            }

            _file = fopen(filename.c_str(), "wb");
            if (!_file) 
            {
                CUBBYFLOW_ERROR << "Could not open " << filename;
                return false;
            }

            _avFrame = av_frame_alloc();
            if (!_avFrame)
            {
                CUBBYFLOW_ERROR << "ould not allocate video frame";
                return false;    
            }

            _avFrame->format = _codecContext->pix_fmt;
            _avFrame->width  = _codecContext->width;
            _avFrame->height = _codecContext->height;
            ret = av_image_alloc(_avFrame->data, _avFrame->linesize, _codecContext->width, _codecContext->height, _codecContext->pix_fmt, 32);
            if (ret < 0)
            {
                CUBBYFLOW_ERROR << "Could not allocate raw picture buffer";
                return false;
            }

            std::cout << "Start Encode" << std::endl;
            return true;
        }

        bool ScreenRecorder::FinishEncode()
        {
            uint8_t endcode[] = { 0, 0, 1, 0xb7 };
            int ret;

            fflush(stdout);
            ret = avcodec_send_frame(_codecContext, nullptr);
            if (ret < 0) 
            {
                CUBBYFLOW_ERROR << "Error sending a frame for encoding";
                return false;
            }
               
            while (ret >= 0) 
            {
                ret = avcodec_receive_packet(_codecContext, _pkt);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                {
                    break;
                }
                else if (ret < 0) 
                {
                    CUBBYFLOW_ERROR << "Error during encoding";
                    return false;
                }
                fwrite(_pkt->data, 1, _pkt->size, _file);
                av_packet_unref(_pkt);
            }

            fwrite(endcode, 1, sizeof(endcode), _file);
            fclose(_file);
            avcodec_free_context(&_codecContext);
            av_frame_free(&_avFrame);
            av_packet_free(&_pkt);

            std::cout << "Finish Encode" << std::endl;
            return true;
        }

        bool ScreenRecorder::EncodeFrame(const ConstArrayAccessor2<Vector4UB>& frame)
        {
            //!const int in_linesize[1] = { 4 * _codecContext->width };
            int ret;
            
            /*
            const uint8_t* data = &(frame.data()[0][0]);
std::cout << "test" << std::endl;
            //! Convert RGB data to YUV data
            _swsContext = sws_getCachedContext(_swsContext,
                    _codecContext->width, _codecContext->height, AV_PIX_FMT_RGB32,
                    _codecContext->width, _codecContext->height, AV_PIX_FMT_YUV420P,
                    0, nullptr, nullptr, nullptr);
            sws_scale(_swsContext, (const uint8_t* const*)data, in_linesize, 0,
                    _codecContext->height, _avFrame->data, _avFrame->linesize);
              */      
            av_init_packet(_pkt);
            _pkt->data = nullptr;
            _pkt->size = 0;
            
            ret = avcodec_send_frame(_codecContext, _avFrame);
            if (ret < 0) 
            {
                CUBBYFLOW_ERROR << "Error sending a frame for encoding";
                return false;
            }
               
            while (ret >= 0) 
            {
                ret = avcodec_receive_packet(_codecContext, _pkt);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                    return false;
                else if (ret < 0) 
                {
                    CUBBYFLOW_ERROR << "Error during encoding";
                    return false;
                }
                fwrite(_pkt->data, 1, _pkt->size, _file);
                av_packet_unref(_pkt);
            }

            return true;
        }

        Size2 ScreenRecorder::size() const
        {
            return _size;
        }

        size_t ScreenRecorder::Width() const
        {
            return _size[0];
        }

        size_t ScreenRecorder::Height() const
        {
            return _size[1];
        }
        
        void ScreenRecorder::SetSize(Size2 dim)
        {
            _size = dim;
        }

        void ScreenRecorder::SetSize(size_t width, size_t height)
        {
            SetSize(Size2(width, height));
        }
} 
}

#endif