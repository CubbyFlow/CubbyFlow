/*************************************************************************
> File Name: Texture3D.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/05/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifndef CUBBYFLOW_TEXTURE3D_H
#define CUBBYFLOW_TEXTURE3D_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Texture/Texture.h>
#include <Framework/Utils/Common.h>
#include <Core/Array/ArrayAccessor3.h>
#include <Core/Size/Size3.h>
#include <Core/Vector/Vector4.h>
#include <cstdint>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {

    //! Abstract base class for textures.
    class Texture3D : public Texture
    {
    public:
        //! Default constructor.
        Texture3D();

        //! Destructor.
        virtual ~Texture3D();

        //! Updates current texture with given data.
        //! given image bytes array size must matched with original one.
        virtual void updateTexture(RendererPtr renderer, void* data) = 0;

        //! Sets the texture with given data and size.
        void allocateTexture(RendererPtr renderer, Size3 size, void* data);

        //! Get the size of texture.
        Size3 getTextureSize() const;
     protected:
        //! Called when sampling mode has changed.
        virtual void onSetTextureParams(const TextureParams& mode) override = 0;

        //! implementation of bind method
        virtual void onBind(RendererPtr renderer, unsigned int slotID) override = 0;
        
        //! implementation of destry method
        virtual void onDestroy() override = 0;

        //! Allocate gpu 
        virtual void onAllocateTexture(RendererPtr renderer, void* data) = 0;

        Size3 _textureSize;
    private:
    };

    using Texture3DPtr = std::shared_ptr<Texture3D>;

} 
} 

#endif 