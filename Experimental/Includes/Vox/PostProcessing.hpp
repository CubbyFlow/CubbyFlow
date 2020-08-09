/*************************************************************************
> File Name: PostProcessing.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL PostProcessing Class
> Created Time: 2020/08/09
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_POSTPROCESSING_HPP
#define CUBBYFLOW_VOX_POSTPROCESSING_HPP

#include <Vox/GLTypes.hpp>
#include <memory>

namespace Vox {
    
    class FrameContext;

    /**
     * OpenGL PostPRocessing class
     */
    class PostProcessing 
    {
    public:
        //! Default Constructor
        PostProcessing();
        //! Default Destructor
        ~PostProcessing();

        //! Add debug drawing shader program to frame context.
        void Initialize(const std::shared_ptr<FrameContext>& ctx);

        //! Draw the Debug wire frame.
        void DrawFrame(const std::shared_ptr<FrameContext>& ctx, const std::string& screenTextureName) const;
    protected:
    private:
        GLuint _screenQuad { 0 };
    };

};

#endif