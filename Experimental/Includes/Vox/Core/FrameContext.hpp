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

#include <Vox/Utils/GLTypes.hpp>
#include <Vox/Core/Vertex.hpp>
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
        std::weak_ptr<VoxScene> _scene;
        GLFWwindow* _windowCtx;
        RenderStatus _renderStatus;
    };
};

#endif