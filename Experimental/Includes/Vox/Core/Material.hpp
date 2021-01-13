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
#include <Core/Vector/Vector3.hpp>
#include <string>
#include <memory>

namespace Vox {

    class Program;

    /**
     */
    class Material : public VoxSceneObject
    {
    public:
        struct BRDF
        {
            CubbyFlow::Vector3F albedo;
            float metallic;
            float roughness;
            float ao;
        };

        //! Constructor with geometry cache.
        Material();
        //! Default destructor
        ~Material();

        //! Attach shader program to this material.
        void AttachProgramShader(const std::shared_ptr<Program>& program);
        //! Bind this material to the context
        void BindMaterial(const std::shared_ptr<FrameContext>& ctx);
        //! Get rvalue render status structure.
        FrameContext::RenderStatus GetRenderStatus() const;
        //! Set the render status for the material.
        void SetRenderStatus(FrameContext::RenderStatus newRenderStatus);
        //! Set BRDF properties of the material
        void SetBRDF(const BRDF& brdf);
    private:
        std::shared_ptr<Program> _program;
        FrameContext::RenderStatus _renderStatus;
        Material::BRDF _brdfProperties;
    };
}

#endif