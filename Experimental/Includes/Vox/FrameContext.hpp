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
#include <Vox/PerspectiveCamera.hpp>
#include <Core/Matrix/Matrix4x4.h>
#include <memory>
#include <vector>
#include <unordered_map>

struct GLFWwindow;

namespace Vox {
    class FrameBuffer;
    class Program;
    class VoxScene;
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

        //! Get Render Mode Primitive
        GLenum GetRenderMode() const;

        //! Add Shader Program to frame context with human-readable text.
        void AddShaderProgram(const std::string& name, GLuint program);

        //! Returns the current program.
	    const std::weak_ptr<Program>& GetCurrentProgram() const;

        //! Set current program of the context.
        void MakeProgramCurrent(const std::string& name);

        //! Add Shader Program to frame context with human-readable text.
        void AddTexture(const std::string& name, GLenum texture);

        //! Set current program of the context.
        void BindTextureToSlot(const std::string& name, GLenum target, GLuint slot);

        //! Send view projection matrix to uniform variable inthe current bounded program.
        void UpdateProgramCamera(const std::shared_ptr<PerspectiveCamera>& camera);

        //! Push Frame Buffer to the vector
        //! Binding will be occurred sequentially.
        void AddFrameBuffer(const std::string& name);

        //! Bind framebuffers sequentially.
        const std::shared_ptr<FrameBuffer>& BindFrameBuffer(const std::string& name, GLenum target);

        //! Return the GLFWwindow context instance.
        GLFWwindow* GetWindowContext();

    protected:
    private:
        std::unordered_map<std::string, std::shared_ptr<FrameBuffer>> _fboMap;
        std::unordered_map<std::string, std::shared_ptr<Program>> _programMap;
        std::unordered_map<std::string, GLuint> _textureMap;
        GLFWwindow* _windowCtx;
        GLenum _renderMode;
        std::weak_ptr<Program> _currentProgram;
    };

};

#endif