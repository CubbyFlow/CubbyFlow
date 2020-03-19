/*************************************************************************
> File Name: Rendererable.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base rendererable for Renderer object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Rendererable.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    Rendererable::Rendererable()
    {
        //! Do nothing
    }

    Rendererable::~Rendererable()
    {
        //! Do nothing
    }

    void Rendererable::render(RendererPtr renderer)
    {
        onRender(renderer);
    }

} 
}