/*************************************************************************
> File Name: GL3Texture3D.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/05/04
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL_TEXTURE3D_H
#define CUBBYFLOW_GL_TEXTURE3D_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Utils/Common.h>
#include <Framework/Texture/Texture3D.h>
#include <cstdint>
#include <memory>

#ifdef CUBBYFLOW_USE_GL
#include <GL3/Texture/GL3Texture.h>
#include <GL3/Utils/GL3Common.h>

namespace CubbyFlow {
namespace CubbyRender {


    //! Abstract base class for textures.
    class GL3Texture3D : public GL3Texture, public Texture3D
    {
    public:
        //! Default constructor.
        GL3Texture3D();

        //! Destructor.
        virtual ~GL3Texture3D();

        //! Updates current texture with given data.
        //! given image bytes array size must matched with original one.
        void updateTexture(RendererPtr renderer, void* data) override;

     protected:
        //! Called when sampling mode has changed.
        void onSetTextureParams(const TextureParams& mode) override;

        //! implementation of bind method
        void onBind(RendererPtr renderer, unsigned int slotID) override;
        
        //! implementation of destry method
        void onDestroy() override;

        //! Allocate gpu 
        void onAllocateTexture(RendererPtr renderer,  void* data) override;
     private:
    };

    using GL3Texture3DPtr = std::shared_ptr<GL3Texture3D>;
} 
} 

#endif 
#endif