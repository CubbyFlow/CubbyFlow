/*************************************************************************
> File Name: Texture.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Texture wrapper class
> Created Time: 2020/08/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/Texture.hpp>
#include <Vox/Utils/FileSystem.hpp>
#include <Vox/Utils/DebugUtils.hpp>
#include <glad/glad.h>
#include <fstream>
#include <cstring>

namespace Vox {

    //! Texture sampling modes.
    TextureParameters::TextureParameters()
        : minFilter(GL_LINEAR), 
          magFilter(GL_LINEAR),
          wrapS(GL_CLAMP_TO_EDGE),
          wrapT(GL_CLAMP_TO_EDGE), 
          wrapR(GL_CLAMP_TO_EDGE){};

    Texture::Texture(GLuint target, GLuint textureID)
        : _target(target), _id(textureID)
    {
        //! Do nothing.
    }

    Texture::~Texture()
    {
        if(_id) glDeleteTextures(1, &_id);
    }

    void Texture::BindTexture(unsigned int slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(_target, _id);
    }

    void Texture::SetTextureTarget(GLuint target)
    {
        _target = target;
    }

    const TextureParameters& Texture::GetParameters() const
    {
        return _params;
    }

    void Texture::SetParameters(const TextureParameters& params)
    {
        _params = params;
        OnSetParameters();
    }

    void Texture::OnSetParameters()
    {
        glBindTexture(_target, _id);
		glTexParameteri(_target, GL_TEXTURE_WRAP_S, _params.wrapS);
		glTexParameteri(_target, GL_TEXTURE_WRAP_T, _params.wrapT);
		glTexParameteri(_target, GL_TEXTURE_WRAP_R, _params.wrapR);
		glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, _params.minFilter);
		glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, _params.magFilter);
        glBindTexture(_target, 0);
    }

    GLuint Texture::GetTextureID() const
    {
        return _id;
    }

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
};