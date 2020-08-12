/*************************************************************************
> File Name: S3TextureCompression.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References: https://github.com/OpenGLInsights/OpenGLInsightsCode
> Purpose: Video capture class using Texture Compression technique
> Created Time: 2020/08/01
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_S3_TEXTURE_COMPRESSION
#define CUBBYFLOW_VOX_S3_TEXTURE_COMPRESSION

#include <Vox/GLTypes.hpp>
#include <memory>

namespace Vox {
    class FrameBuffer;
    class FrameContext;

    /**
     * OpenGL Context wrapper which will be used for rendering one frame.
     */
    class S3TextureCompression 
    {
    public:
        //! Default Constructor
        S3TextureCompression(GLsizei width, GLsizei height);
        //! Default Destructor
        ~S3TextureCompression();

        //! Compress given texture argument with S3TC_DXT5 and return.
        GLuint DXT5Compress(const std::shared_ptr<FrameContext>& ctx, const std::string& textureName);
        
    protected:
    private:
        std::unique_ptr<FrameBuffer> _s3tcFramebuffer;
        GLuint _vao;
        GLuint _vboDXT;
        GLuint _texIm;
        GLuint _texDXT;
        GLuint _texFinal;
        GLuint _s3tcProgram;
        GLsizei _width, _height;
    };

};

#endif