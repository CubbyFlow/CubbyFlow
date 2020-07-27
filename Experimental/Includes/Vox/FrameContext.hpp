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
#include <Core/Size/Size2.h>
#include <memory>

namespace Vox {

    /**
     * OpenGL Context wrapper which will be used for rendering one frame.
     */
    class FrameContext 
    {
    public:
        static void DebugLog(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const GLvoid* userParam);
    protected:
    private:
    };

};

#endif