/*************************************************************************
> File Name: Texture2D.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/02/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Texture2D.h>

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

    void Texture2D::allocateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4F>& data, bool storeData)
    {
        if (storeData)
        {
            data.ParallelForEachIndex([&](size_t i, size_t j){
                _data(i, j) = data(i, j);
            });
        }

        if (data.size() == Size2())
        {
            destroy(renderer);
        }
        else if (data.size() == _size)
        {
            updateTexture(renderer, material, data, false);
        }
        else
        {
            destroy(renderer);
            _size = data.size();
            onAllocateTexture(renderer, material, data);
        }
    }

    void Texture2D::setSize(const Size2& size)
    {
        _size = size;
    }

    const Size2& Texture2D::size() const
    {
        return _size;
    }
} 
}