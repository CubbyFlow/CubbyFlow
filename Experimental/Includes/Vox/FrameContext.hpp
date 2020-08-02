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

struct GLFWwindow;

namespace Vox {
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

        //! Set current program of the context.
        void MakeProgramCurrent(GLuint program);

        //! Send view projection matrix to uniform variable inthe current bounded program.
        void UpdateProgramCamera(const PerspectiveCamera& camera);
    protected:
    private:
        GLFWwindow* _windowCtx;
        GLenum _renderMode;
        GLuint _currentProgram;
    };

};

#endif