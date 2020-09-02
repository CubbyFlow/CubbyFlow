/*************************************************************************
> File Name: PostProcessing.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL PostProcessing Class
> Created Time: 2020/08/09
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/PostProcessing.hpp>
#include <Vox/Renderer.hpp>
#include <Vox/Texture.hpp>
#include <Vox/ShaderPreset.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/Program.hpp>
#include <glad/glad.h>

namespace Vox {

    PostProcessing::PostProcessing()
    {
        //! Do nothing
    }

    PostProcessing::~PostProcessing()
    {
        if (_screenQuad) glDeleteVertexArrays(1, &_screenQuad);
    }

    void PostProcessing::Initialize(const std::shared_ptr<FrameContext>& ctx)
    {
        GLuint vs = Renderer::CreateShaderFromSource(kScreenShaders[0], GL_VERTEX_SHADER);
        GLuint fs = Renderer::CreateShaderFromSource(kScreenShaders[1], GL_FRAGMENT_SHADER);
        _postProcessingProgram = ctx->CreateProgram("P_PostProcessing", Renderer::CreateProgram(vs, 0, fs));

        if (_screenQuad) glDeleteVertexArrays(1, &_screenQuad);
        glGenVertexArrays(1, &_screenQuad);
    }

    void PostProcessing::DrawFrame(const std::shared_ptr<FrameContext>& ctx, const std::shared_ptr<Texture>& screenTexture) const
    {
        _postProcessingProgram->BindProgram(ctx->GetContextScene());
        glBindVertexArray(_screenQuad);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        screenTexture->BindTexture(0);
        glBindVertexArray(0);
    }


};