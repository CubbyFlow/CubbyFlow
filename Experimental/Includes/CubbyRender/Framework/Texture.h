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

/*

#include <cstdint>
#include <memory>
#include <Core/Array/ArrayAccessor2.h>
#include <Core/Vector/Vector2.h>
#include <Core/Vector/Vector4.h>
#include <Core/Matrix/Matrix.h>

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
    class Texture 
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

        //! Clears the contents.
        void clear();

        //! Sets the texture with given 32-bit color data and size.
        void setTexture(const ConstArrayAccessor2<Vector4F>& data);

        //! Sets the texture with given 8-bit color data and size.
        void setTexture(const ConstArrayAccessor2<Vector4B>& data);

        //! Binds the texture to given renderer with slot ID.
        void bind(Renderer* renderer, unsigned int slotId);

        //! Returns the size of the texture.
        const Vector2UZ& size() const;

        //! Returns the sampling mode of the texture.
        const TextureSamplingMode& samplingMode() const;

        //! Sets the sampling mode of the texture.
        void setSamplingMode(const TextureSamplingMode& mode);

     protected:
        //! Called when clear() is invoked.
        virtual void onClear() = 0;

        //! Called when setTexture(...) is invoked.
        virtual void onSetTexture(const ConstArrayAccessor2<Vector4F>& data) = 0;

        //! Called when setTexture(...) is invoked.
        virtual void onSetTexture(const ConstArrayAccessor2<Vector4B>& data) = 0;

        //! Called when bind(...) is invoked.
        virtual void onBind(Renderer* renderer, unsigned int slotId) = 0;

        //! Called when sampling mode has changed.
        virtual void onSamplingModeChanged(const TextureSamplingMode& mode) = 0;

     private:
        Vector2UZ _size;
        TextureSamplingMode _samplingMode = TextureSamplingMode::kNearest;
    };

    using TexturePtr = std::shared_ptr<Texture>;

} 
} 
*/

#endif 