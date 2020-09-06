/*************************************************************************
> File Name: Material.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References : https://google.github.io/filament/Materials.html
> Purpose: 
> Created Time: 2020/09/05
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_MATERIAL_HPP
#define CUBBYFLOW_VOX_MATERIAL_HPP

#include <Vox/GLTypes.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/VoxSceneObject.hpp>
#include <Vox/Program.hpp>
#include <Vox/ShaderParameters.hpp>
#include <string>
#include <memory>

namespace Vox {

    class Program;
    class Texture;

    /**
     */
    class Material : public VoxSceneObject
    {
    public:
        //! Constructor with geometry cache.
        Material();
        //! Default destructor
        ~Material();

        //! Attach shader program to this material.
        void AttachProgramShader(const std::shared_ptr<Program>& program);
        //! Set the texture to slot
        void AttachTextureToSlot(const std::string& textureName, unsigned int slot);
        //! Bind this material to the context
        void BindMaterial(const std::shared_ptr<FrameContext>& ctx);
        //! Get rvalue render status structure.
        FrameContext::RenderStatus GetRenderStatus() const;
        //! Set the render status for the material.
        void SetRenderStatus(FrameContext::RenderStatus newRenderStatus);
    private:
        std::shared_ptr<Program> _program;
        FrameContext::RenderStatus _renderStatus;
        CubbyFlow::Array1<std::pair<std::string, unsigned int>> _texturePairs;

    };
}

#endif