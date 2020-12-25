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

#include <Vox/Utils/GLTypes.hpp>
#include <memory>

namespace Vox {
    class FrameContext;
    class Texture;
    class FrameBuffer;
    class Program;

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

        //! Initialize buffers, textures and framebuffers.
        //! After initializing resources, add them to frame context.
        //! initialized textures : DXTexture5, EncodingTexture, CompressedTexture
        //! initialzied frame buffer : S3TCPass, YCoCgDecodingPass
        void Initialize(const std::shared_ptr<FrameContext>& ctx);

        //! Compress given texture argument with S3TC_DXT5.
        void CompressionPass(const std::shared_ptr<FrameContext>& ctx, const std::shared_ptr<Texture>& compressTarget);

        //! Decompress the dxt5 texture and decode ycocg color space into rgb color space.
        std::shared_ptr<Texture> DecodingPass(const std::shared_ptr<FrameContext>& ctx);
        
    protected:
    private:
        std::shared_ptr<Texture> _texIm;
        std::shared_ptr<Texture> _texDXT;
        std::shared_ptr<Texture> _texFinal;
        std::shared_ptr<Program> _s3tcProgram;
        std::shared_ptr<Program> _decodingProgram;
        std::shared_ptr<FrameBuffer> _s3tcPass;
        std::shared_ptr<FrameBuffer> _decodingPass;
        GLuint _vao     { 0 };
        GLuint _vboDXT  { 0 };
        GLsizei _width  { 0 };
        GLsizei _height { 0 };
    };

};

#endif