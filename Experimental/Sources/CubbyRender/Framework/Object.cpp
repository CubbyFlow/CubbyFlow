/*************************************************************************
> File Name: Object.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Renderer Object
> Created Time: 2020/02/19
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Object.h>

namespace CubbyFlow {
namespace CubbyRender {

    Object::Object()
    {
        //! Do nothing
    }   
    
    Object::~Object()
    {
        //! Do nothing
    }

    void Object::bind(RendererPtr renderer)
    {
        onBind(renderer);
    }

    void Object::unbind(RendererPtr renderer)
    {
        onUnbind(renderer);
    }

    void Object::destroy()
    {
        onDestroy();
    }

} 
}