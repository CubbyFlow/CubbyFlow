/*************************************************************************
> File Name: Texture3D.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/05/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Texture/Texture3D.h>

namespace CubbyFlow {
namespace CubbyRender {

    Texture3D::Texture3D()
    {
        //! Do nothing.
    }

    Texture3D::~Texture3D()
    {
        //! Do nothing.
    }

    void Texture3D::allocateTexture(RendererPtr renderer, const ConstArrayAccessor3<Vector4F>& data)
    {
        if (data.size() == Size3())
        {
            destroy();
        }
        else if (data.size() == _textureSize)
        {
            updateTexture(renderer, data);
        }
        else
        {
            destroy();
            _textureSize = data.size();
            onAllocateTexture(renderer, data);
        }
    }

    void Texture3D::allocateTexture(RendererPtr renderer, const ConstArrayAccessor3<Vector4UB>& data)
    {
        if (data.size() == Size3())
        {
            destroy();
        }
        else if (data.size() == _textureSize)
        {
            updateTexture(renderer, data);
        }
        else
        {
            destroy();
            _textureSize = data.size();
            onAllocateTexture(renderer, data);
        }
    }

    Size3 Texture3D::getTextureSize() const
    {
        return _textureSize;
    }
} 
}