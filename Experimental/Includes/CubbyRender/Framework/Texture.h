/*************************************************************************
> File Name: Texture.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/02/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifndef CUBBYFLOW_TEXTURE_H
#define CUBBYFLOW_TEXTURE_H

#include <cstdint>
#include <memory>
#include <Core/Array/ArrayAccessor2.h>
#include <Core/Size/Size2.h>
#include <Core/Vector/Vector4.h>

#include <Framework/Object.h>
#include <Framework/Prerequisites.h>
#include <Framework/Common.h>

namespace CubbyFlow {
namespace CubbyRender {

    //! Texture sampling modes.
    enum class TextureSamplingMode : uint8_t {
        //! Sample nearest pixel.
        kNearest = 0,

        //! Linear-interpolate nearby pixels.
        kLinear = 1
    };

    //! Abstract base class for 2-D textures.
    class Texture : public Object
    {
    public:
        //! Default constructor.
        Texture();

        //! Destructor.
        virtual ~Texture();

        //! Updates current texture with given 32-bit color data.
        virtual void update(const ConstArrayAccessor2<Vector4F>& data) = 0;

        //! Updates current texture with given 8-bit color data.
        virtual void update(const ConstArrayAccessor2<Vector4B>& data) = 0;

        //! Sets the texture with given 32-bit color data and size.
        void setTexture(const ConstArrayAccessor2<Vector4F>& data);

        //! Sets the texture with given 8-bit color data and size.
        void setTexture(const ConstArrayAccessor2<Vector4B>& data);

        //! Returns the size of the texture.
        const Size2& size() const;

        //! Returns the sampling mode of the texture.
        const TextureSamplingMode& samplingMode() const;

        //! Sets the sampling mode of the texture.
        void setSamplingMode(const TextureSamplingMode& mode);

        //! Set texture slot id.
        void setSlotID(unsigned int slotID);

     protected:

        //! Called when setTexture(...) is invoked.
        virtual void onSetTexture(const ConstArrayAccessor2<Vector4F>& data) = 0;

        //! Called when setTexture(...) is invoked.
        virtual void onSetTexture(const ConstArrayAccessor2<Vector4B>& data) = 0;

        //! Called when sampling mode has changed.
        virtual void onSamplingModeChanged(const TextureSamplingMode& mode) = 0;

        //! Called when bind(...) is invoked
        virtual void onBind(RendererPtr renderer) = 0;
        
        //! Called when unbind(...) is  is invoked
        virtual void onUnbind(RendererPtr renderer) = 0;

        //! Called when destroy(...) is  is invoked
        virtual void onDestroy(RendererPtr renderer) = 0;

     private:
        Size2 _size;
        TextureSamplingMode _samplingMode = TextureSamplingMode::kNearest;
        unsigned int _slotID;
    };

    using TexturePtr = std::shared_ptr<Texture>;

} 
} 

#endif 