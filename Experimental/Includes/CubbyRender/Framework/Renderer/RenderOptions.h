/*************************************************************************
> File Name: RenderOptions.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Provide several render option wrappers 
           such as render state, primitive type, etc..
> Created Time: 2020/03/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_RENDEROPTIONS_H
#define CUBBYFLOW_RENDEROPTIONS_H

namespace CubbyFlow {
namespace CubbyRender {

    //! Primitive types
    enum class PrimitiveType
    {
        Point         = 0,
        Line          = 1,
        LineStrip     = 2,
        Triangle      = 3,
        TriangleStrip = 4,
    };

    //! Rendering state representation.
    struct RenderState final
    {
        //! Cull modes.
        enum class CullMode 
        {
            //! No culling.
            None = 0,

            //! Front-face culling.
            Front,

            //! Back-face culling.
            Back,
        };

        //! Alpha-blending factors.
        enum class BlendFactor 
        {
            //! Use 0 blend factor.
            Zero = 0,

            //! Use 1 blend factor.
            One,

            //! Use source color blend factor.
            SrcColor,

            //! Use 1 - source color blend factor.
            OneMinusSrcColor,

            //! Use source alpha blend factor.
            SrcAlpha,

            //! Use 1 - source alpha blend factor.
            OneMinusSrcAlpha,

            //! Use destination alpha blend factor.
            DestAlpha,

            //! Use 1 - destination alpha blend factor.
            OneMinusDestAlpha,

            //! Use destination color blend factor.
            DestColor,

            //! Use 1 - destination color blend factor.
            OneMinusDestColor,
        };

        //! True if front face is defined as clock-wise order.
        bool isFrontFaceClockWise = true;

        //! True if blending is enabled.
        bool isBlendEnabled = true;

        //! True if depth test is enabled.
        bool isDepthTestEnabled = true;

        //! The cull mode.
        CullMode cullMode = CullMode::None;

        //! The blend factor for the source.
        BlendFactor sourceBlendFactor = BlendFactor::SrcAlpha;

        //! The blend factor for the destination.
        BlendFactor destinationBlendFactor = BlendFactor::OneMinusSrcAlpha;
    };
};
};

#endif