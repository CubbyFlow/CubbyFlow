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
#include <Vox/DDSUtils.hpp>
#include <Vox/FileSystem.hpp>
#include <Vox/DebugUtils.hpp>
#include <cstring>

namespace Vox {    
namespace DDS {

    #define MAKEFOURCC(ch0,ch1,ch2,ch3) \
    	((unsigned int)(unsigned char)(ch0)|((unsigned int)(unsigned char)(ch1)<< 8)| \
    	((unsigned int)(unsigned char)(ch2)<<16)|((unsigned int)(unsigned char)(ch3)<<24))

    #define ISBITMASK( r,g,b,a ) ( ddpf.dwRBitMask==r && ddpf.dwGBitMask==g && ddpf.dwBBitMask==b && ddpf.dwRGBAlphaBitMask==a )

    bool GetTexture2DDesc(const DDSHeader* dhdr, int* w, int* h, int* mips, Vox::PixelFmt* pixfmt)
    {
    	VoxAssert((dhdr->magic == DDS_MAGIC), CURRENT_SRC_PATH_TO_STR, "Bad magic number in DDS file!");
    
        const DDSURFACEDESC2 *hdr = &dhdr->ddsd;
    	const DDPIXELFORMAT &ddpf = hdr->ddpfPixelFormat;
    
    	// fill other stuff in desc
    	*w = hdr->dwWidth;
    	*h = hdr->dwHeight;
    	*mips = hdr->dwMipMapCount>1 ? hdr->dwMipMapCount : 1U;
    
    	// find pixel format
    	*pixfmt = Vox::PixelFmt::PF_UNKNOWN;
    	if( ddpf.dwFlags & DDPF_RGB ) {
    		if( 32==ddpf.dwRGBBitCount ) {
    			if( ISBITMASK(0x00ff0000,0x0000ff00,0x000000ff,0xff000000) )
    				*pixfmt = Vox::PixelFmt::PF_BGRA8;
    			if( ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0xff000000) )
    				*pixfmt = Vox::PixelFmt::PF_RGBA8;
    		}
    	}
    	else if( ddpf.dwFlags & DDPF_FOURCC ) {
    		if( MAKEFOURCC('D','X','T','1')==ddpf.dwFourCC )
    			*pixfmt = Vox::PixelFmt::PF_DXT1;
    		else if( MAKEFOURCC('D','X','T','3')==ddpf.dwFourCC )
    			*pixfmt = Vox::PixelFmt::PF_DXT3;
    		else if( MAKEFOURCC('D','X','T','5')==ddpf.dwFourCC )
    			*pixfmt = Vox::PixelFmt::PF_DXT5;
    		else if( MAKEFOURCC(114,0,0,0)==ddpf.dwFourCC )
    			*pixfmt = Vox::PixelFmt::PF_R32F;
    		else if( MAKEFOURCC(115,0,0,0)==ddpf.dwFourCC )
    			*pixfmt = Vox::PixelFmt::PF_RG32F;
    		else if( MAKEFOURCC(116,0,0,0)==ddpf.dwFourCC )
    			*pixfmt = Vox::PixelFmt::PF_RGBA32F;
    		else if( MAKEFOURCC('D','X','1','0')==ddpf.dwFourCC )
    			return false;
    	}
    	else if( ddpf.dwFlags & DDPF_LUMINANCE ) {
            if( ddpf.dwRGBBitCount==8 ) {
                *pixfmt = Vox::PixelFmt::PF_R8;
            }
        }
    	else
    		return false;
    
    	return true;
    }
    
    struct ddsd2pf {
        Vox::PixelFmt _pf;
    	DDPIXELFORMAT _ddpf;
    };
    
    const ddsd2pf __tbl[]={
    	{ Vox::PixelFmt::PF_BGRA8,		{ 32,DDPF_RGB,0,32,0x00ff0000,0x0000ff00,0x000000ff,0xff000000 } },
    	{ Vox::PixelFmt::PF_RGBA8,		{ 32,DDPF_RGB,0,32,0x000000ff,0x0000ff00,0x00ff0000,0xff000000 } },
    	{ Vox::PixelFmt::PF_RGB8,		{ 32,DDPF_RGB,0,32,0x000000ff,0x0000ff00,0x00ff0000,0x00000000 } },
    	{ Vox::PixelFmt::PF_BGR8,		{ 32,DDPF_RGB,0,32,0x00ff0000,0x0000ff00,0x000000ff,0x00000000 } },
    
    	{ Vox::PixelFmt::PF_DXT1,		{ 32,DDPF_FOURCC,MAKEFOURCC('D','X','T','1'),0,0,0,0,0 } },
    	{ Vox::PixelFmt::PF_DXT3,		{ 32,DDPF_FOURCC,MAKEFOURCC('D','X','T','3'),0,0,0,0,0 } },
    	{ Vox::PixelFmt::PF_DXT5,		{ 32,DDPF_FOURCC,MAKEFOURCC('D','X','T','5'),0,0,0,0,0 } },
    
    	{ Vox::PixelFmt::PF_R32F,		{ 32,DDPF_FOURCC,MAKEFOURCC(114,0,0,0),0,0,0,0,0 } },
    	{ Vox::PixelFmt::PF_RG32F,		{ 32,DDPF_FOURCC,MAKEFOURCC(115,0,0,0),0,0,0,0,0 } },
    	{ Vox::PixelFmt::PF_RGBA32F,	{ 32,DDPF_FOURCC,MAKEFOURCC(116,0,0,0),0,0,0,0,0 } },
    };
    
    bool FillPixelFormat(DDPIXELFORMAT *ddpf, const Vox::PixelFmt pf)
    {
    	const unsigned int n = sizeof(__tbl)/sizeof(ddsd2pf);
    
    	for(unsigned int i=0; i<n; ++i) {
    		if( __tbl[i]._pf==pf ) {
    			std::memcpy(ddpf, &__tbl[i]._ddpf, sizeof(*ddpf));
    			return true;
    		}
    	}
    	return false;
    }
};
};