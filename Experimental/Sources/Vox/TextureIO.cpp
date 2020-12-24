/*************************************************************************
> File Name: TextureIO.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References: OpenInsights sample codes.
> Purpose: Texture write and load functions for dds and tga file.
> Created Time: 2020/08/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/TextureIO.hpp>
#include <Vox/FileSystem.hpp>
#include <Vox/DebugUtils.hpp>
#include <Vox/PixelFmt.hpp>
#include <Vox/Renderer.hpp>
#include <fstream>

namespace Vox {

	unsigned int GetByteSize(int w, int h, int mips, Vox::PixelFmt fmt)
    {
        int wm=w, hm=h, size=0;
        for(int i=0; i<mips; ++i)
        {
            size += wm*hm;
    
            wm >>= 1;
            hm >>= 1;
            if(wm==0) wm=1;
            if(hm==0) hm=1;
        }
    
    	const Vox::PixelFmtDesc* pf = Vox::GetPixelFmtDesc(fmt);
        
        return size * pf->size;
    }

	namespace DDS
	{
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

		void* LoadTextureData(const char* file, int* w, int* h, int* mips, Vox::PixelFmt* pixfmt)
		{
            std::ifstream f(file, std::ios::in | std::ios::binary);
            VoxAssert(f.is_open(), CURRENT_SRC_PATH_TO_STR, std::string("Failed to open ") + file);

    		DDSHeader hdr;
            f.read(reinterpret_cast<char*>(&hdr), sizeof(hdr));

    		if(!DDS::GetTexture2DDesc(&hdr, w, h, mips, pixfmt))
        		return 0;

    		const size_t numBytes = GetByteSize(*w, *h, *mips, *pixfmt);
            void* data = ::malloc(numBytes);

            f.read(reinterpret_cast<char*>(data), numBytes);
            f.close();
    		return data;
		}

		bool WriteTexture(const char* file, int w, int h, int mips, Vox::PixelFmt fmt, const void* data)
		{
			if ((file == nullptr) || (w < 1) || (h < 1) || (data == nullptr))
				return false;

			DDSHeader hdr;
			hdr.magic = DDS_MAGIC;
			DDSURFACEDESC2* surf = &hdr.ddsd;

			const size_t bytesSize = GetByteSize(w, h, mips, fmt);
			memset(surf, 0, sizeof(DDSURFACEDESC2));
			surf->dwSize = 124;
			surf->dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_LINEARSIZE;
			if (mips > 1)
				surf->dwFlags |= DDSD_MIPMAPCOUNT;
			surf->dwWidth = w;
			surf->dwHeight = h;
            surf->dwPitchOrLinearSize = static_cast<unsigned int>(bytesSize);
			surf->dwMipMapCount = mips;

			if (!FillPixelFormat(&surf->ddpfPixelFormat, fmt))
				return false;

			surf->ddsCaps.dwCaps1 = DDSCAPS_TEXTURE;
			if (mips > 1)
				surf->ddsCaps.dwCaps1 |= DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;

			std::ofstream f(file, std::ios::out | std::ios::binary);
            VoxAssert(f.is_open(), CURRENT_SRC_PATH_TO_STR, std::string("Failed to open ") + file);
            f.write(reinterpret_cast<const char*>(&hdr), sizeof(hdr));
            f.write(reinterpret_cast<const char*>(data), bytesSize);
            f.close();

			return true;
		}
	};

	namespace TGA
	{
		#pragma pack(push,1)
		struct TGAHeader
		{
			char identsize;			//< size of ID field that follows 18 byte header (0 usually)
			char colormaptype;		//< type of colour map 0=none, 1=has palette
			char imagetype;			//< type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

			short colormapstart;	//< first colour map entry in palette
			short colormaplength;	//< number of colours in palette
			char colormapbits;		//< number of bits per palette entry 15,16,24,32

			short xstart;           //< image x origin
			short ystart;           //< image y origin
			short width;            //< image width in pixels
			short height;           //< image height in pixels
			char bits;              //< image bits per pixel 8,16,24,32
			char descriptor;        //< image descriptor bits (vh flip bits)

			TGAHeader() = default;
			TGAHeader(const short width, const short height, const char bits)
				: identsize(0)
				, colormaptype(0)
				, imagetype(2)
				, colormapstart(0)
		        , colormaplength(0)
		        , colormapbits(0)
		        , xstart(0)
		        , ystart(0)
		        , width(width)
		        , height(height)
		        , bits(bits)
		        , descriptor(0)
			{}
		};
		#pragma pack(pop)
		
		void* LoadTextureData(const char* file, int* w, int* h, int* mips, Vox::PixelFmt* pixfmt)
		{
            UNUSED_VARIABLE(pixfmt);
            std::ifstream f(file, std::ios::in | std::ios::binary);

            VoxAssert(f.is_open(), CURRENT_SRC_PATH_TO_STR, std::string("Failed to open ") + file);

    		TGAHeader hdr;
            f.read(reinterpret_cast<char*>(&hdr), sizeof(hdr));

    		VoxAssertFailCallback((hdr.bits == 32) && (hdr.imagetype == 2),
						CURRENT_SRC_PATH_TO_STR,
						"Unsupported TGA file format [" + std::string(file) + "]",
						[&f](){ f.close(); });

            *w = hdr.width;
            *h = hdr.height;
            *mips = 1;

    		const size_t numBytes = hdr.width * hdr.height * 4;
    		void* data = ::malloc(numBytes);
            f.read(static_cast<char*>(data), numBytes);
    		f.close();
			return data;
		}

		bool WriteTexture(const char* file, int w, int h, int mips, Vox::PixelFmt fmt, const void* data)
		{
            UNUSED_VARIABLE(mips);
            std::ofstream f(file, std::ios::out | std::ios::binary);
            VoxAssert(f.is_open(), CURRENT_SRC_PATH_TO_STR, std::string("Failed to open ") + file);

			const PixelFmtDesc* desc = GetPixelFmtDesc(fmt);
			//! 8 at the last of the below constructor mean byte size. maybe tricky.
			TGAHeader hdr(static_cast<short>(w), static_cast<short>(h), desc->size * 8);
            f.write(reinterpret_cast<const char*>(&hdr), sizeof(hdr));
		
			const unsigned int numBytes = GetByteSize(w, h, 1, fmt);
            f.write(reinterpret_cast<const char*>(&data), numBytes);
            f.close();
			return true;
		}
	};
	
};

