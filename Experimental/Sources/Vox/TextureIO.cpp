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
#include <Vox/DDSUtils.hpp>
#include <Vox/TGAUtils.hpp>
#include <cstdio>
#include <cstring>

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
		void* LoadTextureData(const char* file, int* w, int* h, int* mips, Vox::PixelFmt* pixfmt)
		{
    		FILE* fp = fopen(file, "rb");
			VoxAssert(fp, CURRENT_SRC_PATH_TO_STR, "File not found");

    		DDSHeader hdr;
			size_t num_read;
    		num_read = fread(&hdr, sizeof(hdr), 1, fp);
			if (num_read != 1) return nullptr;

    		if(!DDS::GetTexture2DDesc(&hdr, w, h, mips, pixfmt))
        		return 0;

    		int size = GetByteSize(*w, *h, *mips, *pixfmt);
    		void* data = ::malloc(size);

    		num_read = fread(data, size, 1, fp);
			if (num_read != 1) return nullptr;

    		fclose(fp);

    		return data;
		}

		bool WriteTexture(const char* file, int w, int h, int mips, Vox::PixelFmt fmt, const void* data)
		{
			if ((file == nullptr) || (w < 1) || (h < 1) || (data == nullptr))
				return false;

			DDSHeader hdr;
			hdr.magic = DDS_MAGIC;
			DDSURFACEDESC2* surf = &hdr.ddsd;

			int size = GetByteSize(w, h, mips, fmt);
			memset(surf, 0, sizeof(DDSURFACEDESC2));
			surf->dwSize = 124;
			surf->dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_LINEARSIZE;
			if (mips > 1)
				surf->dwFlags |= DDSD_MIPMAPCOUNT;
			surf->dwWidth = w;
			surf->dwHeight = h;
			surf->dwPitchOrLinearSize = size;
			surf->dwMipMapCount = mips;

			if (!FillPixelFormat(&surf->ddpfPixelFormat, fmt))
				return false;

			surf->ddsCaps.dwCaps1 = DDSCAPS_TEXTURE;
			if (mips > 1)
				surf->ddsCaps.dwCaps1 |= DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;

			FILE* fp = fopen(file, "wb");
			(void)fwrite(&hdr, sizeof(hdr), 1, fp);
			(void)fwrite(data, 1, size, fp);
			fclose(fp);

			return true;
		}
	};

	namespace TGA
	{
		void* LoadTextureData(const char* file, int* w, int* h, int* mips, Vox::PixelFmt* pixfmt)
		{
			FILE* fp = fopen(file, "rb");
			//Vox::VoxAssert(fp, CURRENT_SRC_PATH_TO_STR, "File not found");

    		TGAHeader hdr;
			size_t numRead;
    		numRead = fread(&hdr, sizeof(hdr), 1, fp);
			(void)numRead;

    		VoxAssertFailCallback((hdr.bits == 32) && (hdr.imagetype == 2),
						CURRENT_SRC_PATH_TO_STR,
						"Unsupported TGA file format [" + std::string(file) + "]",
						[&fp](){ fclose(fp); });

    		const size_t numBytes = hdr.width * hdr.height * 4;
    		void* data = ::malloc(numBytes);

    		numRead = fread(data, numBytes, 1, fp);
			(void)numRead;

    		fclose(fp);
			return data;
		}

		bool WriteTexture(const char* file, int w, int h, int mips, Vox::PixelFmt fmt, const void* data)
		{
			FILE* fp = fopen(file, "wb");

			const PixelFmtDesc* desc = GetPixelFmtDesc(fmt);
			//! 8 at the last of the below constructor mean byte size. maybe tricky.
			TGAHeader hdr(static_cast<short>(w), static_cast<short>(h), desc->size * 8);
			fwrite(static_cast<const void*>(&hdr), sizeof(hdr), 1, fp);
		
			const unsigned int numBytes = GetByteSize(w, h, 1, fmt);
			fwrite(data, sizeof(unsigned char), numBytes, fp);
			fclose(fp);
			return true;
		}
	};
	
};

