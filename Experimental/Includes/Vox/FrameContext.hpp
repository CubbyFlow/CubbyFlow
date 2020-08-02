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
    /**
     * OpenGL Context wrapper which will be used for rendering one frame.
     */
    class FrameContext 
    {
    public:
        FrameContext(GLFWwindow* windowCtx);
        ~FrameContext();

        //! Make this instance as opengl current context
        void MakeContextCurrent() const;

        //! Get Render Mode Primitive
        GLenum GetRenderMode() const;

        //! Add Shader Program to frame context with human-readable text.
        void AddShaderProgram(const std::string& name, GLuint program);

        //! Set current program of the context.
        void MakeProgramCurrent(const std::string& name);

        //! Add Shader Program to frame context with human-readable text.
        void AddTexture(const std::string& name, GLenum texture);

        //! Set current program of the context.
        void BindTextureToSlot(const std::string& name, GLenum target, GLuint slot);

        //! Send view projection matrix to uniform variable inthe current bounded program.
        void UpdateProgramCamera(const PerspectiveCamera& camera);

        //! Push Frame Buffer to the vector
        //! Binding will be occurred sequentially.
        void AddFrameBuffer(std::shared_ptr<FrameBuffer> fbo);

        //! Bind framebuffers sequentially.
        void BindNextFrameBuffer(GLenum target);
    protected:
    private:
        std::vector<std::shared_ptr<FrameBuffer>> _fbos;
        std::vector<std::shared_ptr<FrameBuffer>>::iterator _fboIterator;
        std::unordered_map<std::string, GLuint> _programMap;
        std::unordered_map<std::string, GLuint> _textures;
        GLFWwindow* _windowCtx;
        GLenum _renderMode;
        GLuint _currentProgram;
    };

};

#endif