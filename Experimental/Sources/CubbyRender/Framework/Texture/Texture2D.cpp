/*************************************************************************
> File Name: Texture2D.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/02/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Texture/Texture2D.h>

namespace CubbyFlow {
namespace CubbyRender {

    Texture2D::Texture2D()
    {
        //! Do nothing.
    }

    Texture2D::~Texture2D()
    {
        //! Do nothing.
    }

    void Texture2D::allocateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4F>& data)
    {
        if (data.size() == Size2())
        {
            destroy();
        }
        else if (data.size() == _textureSize)
        {
            updateTexture(renderer, material, data);
        }
        else
        {
            destroy();
            _textureSize = data.size();
            onAllocateTexture(renderer, material, data);
        }
    }

    void Texture2D::allocateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4UB>& data)
    {
        if (data.size() == Size2())
        {
            destroy();
        }
        else if (data.size() == _textureSize)
        {
            updateTexture(renderer, material, data);
        }
        else
        {
            destroy();
            _textureSize = data.size();
            onAllocateTexture(renderer, material, data);
        }
    }

    Size2 Texture2D::getTextureSize() const
    {
        return _textureSize;
    }
} 
}