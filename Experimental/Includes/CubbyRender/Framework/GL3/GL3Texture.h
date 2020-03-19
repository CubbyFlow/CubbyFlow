/*************************************************************************
> File Name: GL3Texture.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: texture class opengl implementation
> Created Time: 2020/02/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifndef CUBBYFLOW_GL3TEXTURE_H
#define CUBBYFLOW_GL3TEXTURE_H

#include <Framework/Texture.h>

namespace CubbyFlow {
namespace CubbyRender {

    //! Abstract base class for 2-D textures.
    class GL3Texture : public Texture
    {
    public:
        //! Default constructor.
        GL3Texture();

        //! Destructor.
        virtual ~GL3Texture();

        //! Updates current texture with given 32-bit color data.
        void update(const ConstArrayAccessor2<Vector4F>& data) override;

        //! Updates current texture with given 8-bit color data.
        void update(const ConstArrayAccessor2<Vector4B>& data) override;

     protected:

        //! Called when setTexture(...) is invoked.
        void onSetTexture(const ConstArrayAccessor2<Vector4F>& data) override;

        //! Called when setTexture(...) is invoked.
        void onSetTexture(const ConstArrayAccessor2<Vector4B>& data) override;

        //! Called when sampling mode has changed.
        void onSamplingModeChanged(const TextureSamplingMode& mode) override;

        //! Called when bind(...) is invoked
        void onBind(RendererPtr renderer) override;
        
        //! Called when unbind(...) is  is invoked
        void onUnbind(RendererPtr renderer) override;

        //! Called when destroy(...) is  is invoked
        void onDestroy(RendererPtr renderer) override;

     private:
    };

    using GL3TexturePtr = std::shared_ptr<GL3Texture>;

} 
} 

#endif 