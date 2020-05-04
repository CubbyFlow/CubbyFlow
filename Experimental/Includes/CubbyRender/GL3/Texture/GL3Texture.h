/*************************************************************************
> File Name: GL3Texture.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/05/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL_TEXTURE_H
#define CUBBYFLOW_GL_TEXTURE_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Utils/Common.h>
#include <cstdint>
#include <memory>

#ifdef CUBBYFLOW_USE_GL
#include <GL3/Utils/GL3Common.h>

namespace CubbyFlow {
namespace CubbyRender {

    //! Texture sampling modes.
    struct GL3TextureParameters
    {
        GLuint minFilter = GL_NEAREST;
        GLuint magFilter = GL_NEAREST;
        GLuint wrapS = GL_CLAMP_TO_EDGE;
        GLuint wrapT = GL_CLAMP_TO_EDGE;
        GLuint wrapR = GL_CLAMP_TO_EDGE;
    };

    //! Abstract base class for textures.
    class GL3Texture
    {
    public:
        //! Default constructor.
        GL3Texture();

        //! Constructor with target id.
        GL3Texture(GLuint target);

        //! Destructor.
        virtual ~GL3Texture();

        //! Get the id of the opengl texture.
        GLuint getGLTextureID() const;

     protected:
        //! create texture and allocate id value.
        void createGLTexture();

        //! destroy texture 
        void destroyGLTexture();

        //! bind texture to the renderer.
        void bindGLTexture(unsigned int slotID);

        //! get texture target.
        GLuint getTextureTarget() const;

        //! Set the parameters of the texture which will be generated.
        void setGLTextureParameters(const GL3TextureParameters& glTexParams);

        //! Get teh parameters of the texture.
        GL3TextureParameters getGLTextureParameters() const;

     private:
        GL3TextureParameters _glTexParams;
        GLuint _textureID;
        GLuint _target;
    };

    using GL3TexturePtr = std::shared_ptr<GL3Texture>;

} 
} 

#endif 
#endif