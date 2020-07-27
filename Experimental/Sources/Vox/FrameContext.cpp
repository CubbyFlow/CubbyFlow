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
#include <Vox/DebugUtils.hpp>
#include <Core/Size/Size2.h>
#include <Core/Utils/Logging.h>
#include <glad/glad.h>

using namespace CubbyFlow;

namespace Vox {

    namespace Detail
    {
        // aux function to translate source to string
        std::string GetStringForSource(GLenum source) {
        
        	switch(source) {
        		case GL_DEBUG_SOURCE_API_ARB: 
        			return("API");
        		case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
        			return("Window System");
        		case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
        			return("Shader Compiler");
        		case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
        			return("Third Party");
        		case GL_DEBUG_SOURCE_APPLICATION_ARB:
        			return("Application");
        		case GL_DEBUG_SOURCE_OTHER_ARB:
        			return("Other");
        		default:
        			return("");
        	}
        }

        // aux function to translate severity to string
        std::string GetStringForSeverity(GLenum severity) {
        
        	switch(severity) {
        		case GL_DEBUG_SEVERITY_HIGH_ARB: 
        			return("High");
        		case GL_DEBUG_SEVERITY_MEDIUM_ARB:
        			return("Medium");
        		case GL_DEBUG_SEVERITY_LOW_ARB:
        			return("Low");
        		default:
        			return("");
        	}
        }

        // aux function to translate type to string
        std::string GetStringForType(GLenum type) {
        
        	switch(type) {
        		case GL_DEBUG_TYPE_ERROR_ARB: 
        			return("Error");
        		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
        			return("Deprecated Behaviour");
        		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
        			return("Undefined Behaviour");
        		case GL_DEBUG_TYPE_PORTABILITY_ARB:
        			return("Portability Issue");
        		case GL_DEBUG_TYPE_PERFORMANCE_ARB:
        			return("Performance Issue");
        		case GL_DEBUG_TYPE_OTHER_ARB:
        			return("Other");
        		default:
        			return("");
        	}
        }
    }

    void FrameContext::DebugLog(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const GLvoid* userParam)
    {
        CUBBYFLOW_INFO << "[Type] : "       << Detail::GetStringForType(type)           << 
                          "[Source] : "     << Detail::GetStringForSource(source)       <<
                          "[ID] : "         <<              id                          <<
                          "[Serverity] : "  << Detail::GetStringForSeverity(severity);
                          
        CUBBYFLOW_INFO << "[Message] : "    << message;
        
        StackTrace::PrintStack();
    }

};