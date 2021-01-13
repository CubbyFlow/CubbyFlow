/*************************************************************************
> File Name: CubeMap.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Cube map class for image-based lighting
> Created Time: 2020/12/30
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_CUBEMAP_HPP
#define CUBBYFLOW_VOX_CUBEMAP_HPP

#include <Vox/Core/Emitter.hpp>

namespace Vox
{
    /**
     * Cube map class for image-based lighting
     */
    class CubeMap : public Emitter
    {
     public:
        CubeMap() = default;
     protected:
        void OnUploadProgram(ShaderParameters& params) override;
     private:
    };

};  // namespace Vox

#endif