/*************************************************************************
> File Name: TGAUtils.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: TGA File header structure
> Created Time: 2020/08/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_TGA_UTILS_HPP
#define CUBBYFLOW_VOX_TGA_UTILS_HPP

namespace Vox {
namespace TGA {

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

};
};

#endif