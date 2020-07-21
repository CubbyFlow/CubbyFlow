/*************************************************************************
> File Name: GL3Timer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The timer for measure performance of gpu computation such as DMA.
> Created Time: 2020/07/21
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifdef CUBBYFLOW_USE_GL

#include <GL3/Utils/GL3Timer.h>
#include <GL3/Utils/GL3Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    GL3Timer::GL3Timer()
    {
        glGenQueries(2, _queryID);
        glQueryCounter(_queryID[0], GL_TIMESTAMP);
    }

    GL3Timer::~GL3Timer()
    {
        glDeleteQueries(2, _queryID);
    }
    
    double GL3Timer::DurationInSeconds() const
    {
        int available = 0;
        size_t startTime, endTime;
        glQueryCounter(_queryID[1], GL_TIMESTAMP);
        while (!available)
        {
            glGetQueryObjectiv(_queryID[1], GL_QUERY_RESULT_AVAILABLE, &available);
        }

        glGetQueryObjectui64v(_queryID[0], GL_QUERY_RESULT, &startTime);
        glGetQueryObjectui64v(_queryID[1], GL_QUERY_RESULT, &endTime);
        
        return static_cast<double>(endTime - startTime) / 1000.0;
    }

    void GL3Timer::Reset()
    {
        glQueryCounter(_queryID[0], GL_TIMESTAMP);
    }
}
}

#endif