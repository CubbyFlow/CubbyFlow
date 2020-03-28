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

#include <Framework/Prerequisites.h>
#include <Framework/Object.h>
#include <Framework/Common.h>
#include <cstdint>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {

    //! Texture sampling modes.
    enum class TextureSamplingMode : uint8_t 
    {
        //! Sample nearest pixel.
        kNearest = 0,

        //! Linear-interpolate nearby pixels.
        kLinear = 1
    };

    //! Abstract base class for textures.
    class Texture : public Object
    {
    public:
        //! Default constructor.
        Texture();

        //! Destructor.
        virtual ~Texture();

        //! Returns the sampling mode of the texture.
        const TextureSamplingMode& samplingMode() const;

        //! Sets the sampling mode of the texture.
        void setSamplingMode(const TextureSamplingMode& mode);

        //! Sets the texture slot id.
        void setSlotID(unsigned int slotID);

        //! Get the texture slot id.
        unsigned int getSlotID() const;

     protected:
        //! Called when sampling mode has changed.
        virtual void onSamplingModeChanged(const TextureSamplingMode& mode) = 0;

        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) override = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) override = 0;

        //! implementation of destry method
        virtual void onDestroy() override = 0;

     private:
        TextureSamplingMode _samplingMode = TextureSamplingMode::kNearest;
        unsigned int _slotID;
    };

    using TexturePtr = std::shared_ptr<Texture>;

} 
} 

#endif 