/*************************************************************************
> File Name: Texture.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/02/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Texture.h>

namespace CubbyFlow {
namespace CubbyRender {

    Texture::Texture()
    {
        //! Do nothing.
    }

    Texture::~Texture()
    {
        //! Do nothing.
    }

    void Texture::setTexture(const ConstArrayAccessor2<Vector4F>& data)
    {
        onSetTexture(data);
    }

    void Texture::setTexture(const ConstArrayAccessor2<Vector4B>& data)
    {
        onSetTexture(data);
    }

    const Size3& Texture::size() const
    {
        return _size;
    }

    const TextureSamplingMode& Texture::samplingMode() const
    {
        return _samplingMode;
    }

    void Texture::setSamplingMode(const TextureSamplingMode& mode)
    {
        _samplingMode = mode;
    }

    void Texture::setSlotID(unsigned int slotID)
    {
        _slotID = slotID;
    }

    unsigned int Texture::getSlotID() const
    {
        return _slotID;
    }
} 
}