/*************************************************************************
> File Name: Texture2D.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/02/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifndef CUBBYFLOW_TEXTURE2D_H
#define CUBBYFLOW_TEXTURE2D_H

#include <Framework/Prerequisites.h>
#include <Framework/Texture.h>
#include <Framework/Common.h>
#include <Core/Array/Array2.h>
#include <Core/Array/ArrayAccessor2.h>
#include <Core/Size/Size2.h>
#include <Core/Vector/Vector4.h>
#include <cstdint>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {

    //! Abstract base class for textures.
    class Texture2D : public Texture
    {
    public:
        //! Default constructor.
        Texture2D();

        //! Destructor.
        virtual ~Texture2D();

        //! Updates current texture with given 32-bit color data.
        virtual void updateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4F>& data, bool storeData) = 0;

        //! Sets the texture with given 32-bit color data and size.
        void allocateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4F>& data, bool storeData);

        //! Set size of the texture.
        void setSize(const Size2& size);

        //! Returns the size of the texture.
        const Size2& size() const;

     protected:
        //! Called when sampling mode has changed.
        virtual void onSamplingModeChanged(const TextureSamplingMode& mode) override = 0;

        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) override = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) override = 0;

        //! implementation of destry method
        virtual void onDestroy() override = 0;

        //! Allocate gpu 
        virtual void onAllocateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4F>& data) = 0;

     private:
        Array2<Vector4F> _data;
        Size2 _size;
    };

    using Texture2DPtr = std::shared_ptr<Texture2D>;

} 
} 

#endif 