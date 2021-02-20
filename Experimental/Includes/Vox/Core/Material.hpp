/*************************************************************************
> File Name: Material.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 
> Created Time: 2021/01/06
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_MATERIAL_HPP
#define CUBBYFLOW_VOX_MATERIAL_HPP

#include <Vox/Utils/GLTypes.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Scene/VoxSceneObject.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Core/ShaderParameters.hpp>
#include <unordered_map>
#include <memory>

namespace Vox {

    class Program;
    class Texture;

    /**
     */
    class Material : public VoxSceneObject
    {
    public:
        //! Default constructor
        Material();
        //! Default destructor
        virtual ~Material();
        //! Attach shader program to this material.
        void AttachProgramShader(const std::shared_ptr<Program>& program);
        //! Attach shader program to this material.
        void AttachTextureToSlot(const std::shared_ptr<Texture>& texture, unsigned int slot);
        //! Bind this material to the context
        void BindMaterial(const std::shared_ptr<FrameContext>& ctx);
        //! Get rvalue render status structure.
        FrameContext::RenderStatus GetRenderStatus() const;
        //! Set the render status for the material.
        void SetRenderStatus(FrameContext::RenderStatus newRenderStatus);
        //! Returns the program.
        inline const std::shared_ptr<Program> GetProgram()
        {
            return _program;
        }
    protected:
        //! Send material attributes to shader program.
        virtual void ConfigureShaderParameters(ShaderParameters& params) { (void)params; };

    private:
        std::shared_ptr<Program> _program;
        std::unordered_map<unsigned int, std::shared_ptr<Texture>> _textures;
        FrameContext::RenderStatus _renderStatus;
    };
}

#endif