/*************************************************************************
> File Name: DDSUtils.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References: OpenInsights sample codes.
> Purpose: DDS File header structure and data query functions
> Created Time: 2020/08/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_DDS_UTILS_HPP
#define CUBBYFLOW_VOX_DDS_UTILS_HPP

#include <Vox/PixelFmt.hpp>

namespace Vox {
    namespace DDS {
        #define DDS_MAGIC 0x20534444

        typedef struct  {
            unsigned int dwSize;
            unsigned int dwFlags;
            unsigned int dwFourCC;
            unsigned int dwRGBBitCount;
            unsigned int dwRBitMask;
        	unsigned int dwGBitMask;
        	unsigned int dwBBitMask;
            unsigned int dwRGBAlphaBitMask;
        } DDPIXELFORMAT;

        typedef struct  {
            unsigned int dwCaps1;
            unsigned int dwCaps2;
            unsigned int Reserved[2];
        } DDSCAPS2;

        typedef struct  {
            unsigned int dwSize;
            unsigned int dwFlags;
            unsigned int dwHeight;
            unsigned int dwWidth;
            unsigned int dwPitchOrLinearSize;
            unsigned int dwDepth;
            unsigned int dwMipMapCount;
            unsigned int dwReserved1[11];
            DDPIXELFORMAT ddpfPixelFormat;
            DDSCAPS2 ddsCaps;
            unsigned int dwReserved2;
        } DDSURFACEDESC2;

        struct DDSHeader {
        	unsigned int magic;
        	DDSURFACEDESC2 ddsd;
        };

        typedef struct  {
            unsigned int dxgiFormat;
            unsigned int resourceDimension;
            unsigned int miscFlag;
            unsigned int arraySize;
            unsigned int reserved;
        } DDS_HEADER_DXT10;

        struct dds_hdr_dx10 {
        	unsigned int _magic;
        	DDSURFACEDESC2 _ddsd;
        	DDS_HEADER_DXT10 _dx10;
        };

        //  DDS_header.sPixelFormat.dwFlags
        enum EDDSPFFlags {
        	DDPF_ALPHAPIXELS=0x00000001,
            DDPF_ALPHA      =0x00000002,
        	DDPF_FOURCC		=0x00000004,
        	DDPF_INDEXED	=0x00000020,
        	DDPF_RGB		=0x00000040,
            DDPF_YUV        =0x00000200,
            DDPF_LUMINANCE  =0x00020000,
        };

        enum EDDSCaps {
        	DDSCAPS_COMPLEX	=0x000008,
        	DDSCAPS_MIPMAP	=0x400000,
        	DDSCAPS_TEXTURE	=0x001000,
        };

        enum EDDSFlags {
        	DDSD_CAPS		=0x000001,
        	DDSD_HEIGHT		=0x000002,
        	DDSD_WIDTH		=0x000004,
        	DDSD_PITCH		=0x000008,
        	DDSD_PIXELFORMAT=0x001000,
        	DDSD_MIPMAPCOUNT=0x020000,
        	DDSD_LINEARSIZE	=0x080000,
        	DDSD_DEPTH		=0x800000,
        };

        //! Get 2d texture description from the given dds header file.
        bool GetTexture2DDesc(const DDSHeader* dhdr, int* w, int* h, int* mips, Vox::PixelFmt* pixfmt);
        //! Fill the dds pixel format with the given Vox::PixelFmt argument.
        bool FillPixelFormat(DDPIXELFORMAT *ddpf, const Vox::PixelFmt pf);
    };
};

#endif