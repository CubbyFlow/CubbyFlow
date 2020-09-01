/*************************************************************************
> File Name: FrameContext.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Frame Context class for containing context information in one frame.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_FRAME_CONTEXT_HPP
#define CUBBYFLOW_VOX_FRAME_CONTEXT_HPP

#include <Vox/GLTypes.hpp>
#include <Vox/Vertex.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>

struct GLFWwindow;

namespace Vox {

    class Program;
    class FrameBuffer;
    class Texture;
    class VoxScene;
    class VoxResource;
    class Mesh;
    struct MeshShape;

    /**
     * OpenGL Context wrapper which will be used for rendering one frame.
     */
    class FrameContext 
    {
    public:
        //! Default Constructor
        FrameContext(GLFWwindow* windowCtx);

        //! Default Destructor
        ~FrameContext();

        //! Set glfw window should close
        void SetWindowContextShouldClose(bool bShutdown);

        //! Make this instance as opengl current context
        void MakeContextCurrent() const;

        //! Return the GLFWwindow context instance.
        GLFWwindow* GetWindowContext();

        //! Creates Resources and Returns
        std::shared_ptr<FrameBuffer> CreateFrameBuffer(const std::string& name, GLuint id);
        std::shared_ptr<Mesh> CreateMesh(const std::string& name, const MeshShape& shape, VertexFormat format, bool bInterleaved=false);
        std::shared_ptr<Texture> CreateTexture(const std::string& name, GLuint target, GLuint id);
        std::shared_ptr<Program> CreateProgram(const std::string& name, GLuint program);

        //! Returns Resources which have matched name argument.
        std::shared_ptr<FrameBuffer> GetFrameBuffer(const std::string& name);
        std::shared_ptr<Mesh> GetMesh(const std::string& name);
        std::shared_ptr<Texture> GetTexture(const std::string& name);
        std::shared_ptr<Program> GetProgram(const std::string& name);

        //! Bind Scene instance to the frame context.
        void BindSceneToContext(const std::shared_ptr<VoxScene>& scene);

        //! Returns shared_pointer of the scene.
        std::shared_ptr<VoxScene> GetContextScene();

        struct RenderStatus 
        {
            //! True if front face is defined as clock-wise order.
            bool isFrontFaceClockWise;
            //! True if blending is enabled.
            bool isBlendEnabled;
            //! True if depth test is enabled.
            bool isDepthTestEnabled;
            //! The cull mode.
            GLuint cullMode;
            //! The blend factor for the source.
            GLuint sourceBlendFactor;
            //! The blend factor for the destination.
            GLuint destinationBlendFactor;
            //! The render primitive
            GLuint primitive;

            RenderStatus();
        };

        //! Get rvalue render status structure.
        RenderStatus GetRenderStatus() const;

        //! Set new render status to the framecontext.
        void SetRenderStatus(RenderStatus newRenderStatus);

    protected:
    private:
        std::unordered_map<unsigned int, std::weak_ptr<FrameBuffer>> _frameBufferMap;
        std::unordered_map<unsigned int, std::weak_ptr<Mesh>> _meshMap;
        std::unordered_map<unsigned int, std::weak_ptr<Texture>> _textureMap;
        std::unordered_map<unsigned int, std::weak_ptr<Program>> _programMap;
        std::shared_ptr<FrameBuffer> _defaultPass;
        std::weak_ptr<VoxScene> _scene;
        GLFWwindow* _windowCtx;
        RenderStatus _renderStatus;
        std::mutex _m;
    };
};

#endif