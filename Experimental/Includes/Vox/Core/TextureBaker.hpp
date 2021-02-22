/*************************************************************************
> File Name: TextureBaker.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Texture baker class for baking texture in advance rendering loop
> Created Time: 2020/12/30
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_TEXTURE_BAKER_HPP
#define CUBBYFLOW_VOX_TEXTURE_BAKER_HPP

#include <Vox/Utils/GLTypes.hpp>
#include <memory>

namespace Vox
{
    class Texture;
    class FrameContext;

    /**
     * Texture baker class
     */
    class TextureBaker
    {
     public:
         [[nodiscard]] static std::shared_ptr<Texture> BakingCubeMap(const std::shared_ptr<FrameContext>& ctx, 
                                                                     const std::shared_ptr<Texture>& equirect,
                                                                     GLsizei resolutionX, GLsizei resolutionY);
    };

};  // namespace Vox

#endif