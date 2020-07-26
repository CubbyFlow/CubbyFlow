/*************************************************************************
> File Name: FrameContext.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Frame Context class for containing context information in one frame.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/FrameContext.hpp>
#include <Vox/GLDebug.hpp>
#include <Core/Size/Size2.h>
#include <Core/Utils/Logging.h>
#include <glad/glad.h>

using namespace CubbyFlow;

namespace Vox {

    void FrameContext::SetLoader(const std::shared_ptr<ParticleLoader>& loader)
    {
        _loader = loader;
    }

    const std::shared_ptr<ParticleLoader>& FrameContext::GetLoader()
    {
        return _loader;
    }

    void FrameContext::DebugLog(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const GLvoid* userParam)
    {
        CUBBYFLOW_INFO << "[Type] : "       << Detail::GetStringForType(type)           << 
                          "[Source] : "     << Detail::GetStringForSource(source)       <<
                          "[ID] : "         <<              id                          <<
                          "[Serverity] : "  << Detail::GetStringForSeverity(severity);
                          
        CUBBYFLOW_INFO << "[Message] : "    << message;
        
        #if (defined _WIN32 || defined _LINUX)
        	Detail::PrintStack();
        #endif
    }

};