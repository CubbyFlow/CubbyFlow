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
#include <Vox/Core/Renderer.hpp>
#include <Vox/Utils/FileSystem.hpp>
#include <Vox/Utils/DebugUtils.hpp>
#include <glad/glad.h>
#include <fstream>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

    void Texture::LoadXMLNode(VoxScene* scene, const pugi::xml_node& node)
    {
        UNUSED_VARIABLE(scene);
        stbi_set_flip_vertically_on_load(true);

        //! Get texture image filename
        const std::string filename = FileSystem::FindPath(node.attribute("filename").value()).ToString();

        //! Get texture image type
        const std::string type = node.attribute("type").value();

        int width, height, numChannel;
        GLuint textureID;
        if (type == "hdr")
        {
            //! Get floating point data pointer from the hdr image.
            float* data = stbi_loadf(filename.c_str(), &width, &height, &numChannel, 0);
            //! Create texture with floating point format.
            textureID = Renderer::CreateTexture(width, height, PixelFmt::PF_RGB32F, data);
            //! Deallocates the image data.
            stbi_image_free(data);
        }
        else
        {
            //! Get data pointer from the hdr image.
            unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numChannel, 0);
            //! Create texture with floating point format.
            textureID = Renderer::CreateTexture(width, height, PixelFmt::PF_RGB8, data);
            //! Deallocates the image data.
            stbi_image_free(data);
        }

        this->_target = GL_TEXTURE_2D;
        this->_id = textureID;
    }

    void Texture::WriteXMLNode(pugi::xml_node& node)
    {
        UNUSED_VARIABLE(node);
    }
};