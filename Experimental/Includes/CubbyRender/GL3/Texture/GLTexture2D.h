/*************************************************************************
> File Name: GLTexture2D.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/05/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL_TEXTURE2D_H
#define CUBBYFLOW_GL_TEXTURE2D_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Utils/Common.h>
#include <Framework/Texture/Texture2D.h>
#include <cstdint>
#include <memory>

#ifdef CUBBYFLOW_USE_GL
#include <GL3/Utils/GL3Common.h>

namespace CubbyFlow {
namespace CubbyRender {


    //! Abstract base class for textures.
    class GLTexture2D : public GLTexture, Texture2D
    {
    public:
        //! Default constructor.
        GLTexture2D();

        //! Destructor.
        virtual ~GLTexture2D();

        //! Updates current texture with given 128bit color data.
        void updateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4F>& data) override;

        //! Updates current texture with given 32bit color data.
        void updateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4UB>& data) override;

     protected:
        //! Called when sampling mode has changed.
        void onSamplingModeChanged(const TextureSamplingMode& mode) override;

        //! implementation of bind method
        void onBind(RendererPtr renderer, unsigned int slotID) override;
        
        //! implementation of destry method
        void onDestroy() override;

        //! Allocate gpu 
        void onAllocateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4F>& data) override;

        //! Allocate gpu 
        void onAllocateTexture(RendererPtr renderer, MaterialPtr material, const ConstArrayAccessor2<Vector4UB>& data) override;
     private:
    };

    using GLTexture2DPtr = std::shared_ptr<GLTexture2D>;
} 
} 

#endif 
#endif