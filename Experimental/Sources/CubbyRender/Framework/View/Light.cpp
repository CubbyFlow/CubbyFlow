/*************************************************************************
> File Name: Light.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The light class which have pos, dir, light color and etc..
> Created Time: 2020/06/27
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/View/Light.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    Light::Light()
    {
        //! Do nothing.
    }

    Light::Light(const Pivot& pivot)
        : OrthogonalCamera(pivot)
    {
        //! Do nothing.
    }

    Light::~Light()
    {
        //! Do nothing.
    }

    inline void Light::setColor(const Vector3F& color)
    {
        _color = color;
    }

    inline Vector3F Light::getColor() const
    {
        return _color;
    }

}
}