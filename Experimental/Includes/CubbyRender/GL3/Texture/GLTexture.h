/*************************************************************************
> File Name: GLTexture.h
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
    struct GLTextureParameters
    {
        GLuint minFilter = GL_NEAREST;
        GLuint magFilter = GL_NEAREST;
        GLuint wrapS = GL_CLAMP_TO_EDGE;
        GLuint wrapT = GL_CLAMP_TO_EDGE;
        GLuint wrapR = GL_CLAMP_TO_EDGE;
    };

    //! Abstract base class for textures.
    class GLTexture
    {
    public:
        //! Default constructor.
        GLTexture();

        //! Constructor with target id.
        GLTexture(GLuint target);

        //! Destructor.
        virtual ~GLTexture();

        //! Set the parameters of the texture which will be generated.
        void setGLTextureParameters(const GLTextureParameters& glTexParams);

        //! Get teh parameters of the texture.
        GLTextureParameters getGLTextureParameters() const;

        //! Get the id of the opengl texture.
        GLuint getGLTextureID() const;

     protected:
        //! create texture and allocate id value.
        void createTexture();

        //! destroy texture 
        void destroyTexture();

        //! bind texture to the renderer.
        void bindTexture(unsigned int slotID);

        //! get texture target.
        GLuint getTextureTarget() const;

     private:
        GLTextureParameters _glTexParams;
        GLuint _textureID;
        GLuint _target;
    };

    using GLTexturePtr = std::shared_ptr<GLTexture>;

} 
} 

#endif 
#endif