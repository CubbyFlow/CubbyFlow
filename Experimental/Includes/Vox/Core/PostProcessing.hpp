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

#include <Vox/Utils/GLTypes.hpp>
#include <memory>

namespace Vox {
    
    class Program;
    class Texture;
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
        void Initialize(const std::shared_ptr<FrameContext>& ctx, GLuint programID);

        //! Draw the Debug wire frame.
        void DrawFrame(const std::shared_ptr<FrameContext>& ctx, const std::shared_ptr<Texture>& screenTexture) const;
    protected:
    private:
        std::shared_ptr<Program> _postProcessingProgram;
        GLuint _screenQuad { 0 };
    };

};

#endif