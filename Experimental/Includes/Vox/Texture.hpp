/*************************************************************************
> File Name: Texture.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Texture wrapper class
> Created Time: 2020/08/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_TEXTURE_HPP
#define CUBBYFLOW_VOX_TEXTURE_HPP

#include <Vox/NonCopyable.hpp>
#include <Vox/PixelFmt.hpp>
#include <Vox/GLTypes.hpp>

namespace Vox {

    //! Texture sampling modes.
    struct TextureParameters
    {
        GLuint minFilter;
        GLuint magFilter;
        GLuint wrapS;
        GLuint wrapT;
        GLuint wrapR;

        TextureParameters();
    };

    /**
     * OpenGL Texture wrapper class 
     */
    class Texture : public NonCopyable
    {
    public:
        //! Default Constructor
        Texture(GLuint target, GLuint textureID);
        //! Default Destructor
        ~Texture();

        //! Bind texture to the gpu.
        void BindTexture(unsigned int slot);
        //! Set Texture target
        void SetTextureTarget(GLuint target);
        //! Get immutable texture parameters
        const TextureParameters& GetParameters() const;
        //! Set the texture parameters 
        void SetParameters(const TextureParameters& params);
        //! Returns texture id
        GLuint GetTextureID() const;

    private:
        //! Pass texture parameters to the driver setting.
        void OnSetParameters();

        TextureParameters _params;
        GLuint _target;
        GLuint _id;
    };

};

#endif