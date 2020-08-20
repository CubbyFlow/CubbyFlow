/*************************************************************************
> File Name: DebugDraw.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The debug drawing class for visualizing horizontal grid, x,y,z axis, bounding boxes.
> Created Time: 2020/08/08
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_DEBUG_DRAW_HPP
#define CUBBYFLOW_VOX_DEBUG_DRAW_HPP

#include <Vox/GLTypes.hpp>
#include <Core/Vector/Vector2.h>
#include <Core/Vector/Vector3.h>
#include <Core/BoundingBox/BoundingBox3.h>
#include <vector>

namespace Vox {
    class FrameContext;
    /**
     *  Debug drawing mode (visualize horizontal grid, x,y,z axis, bounding boxes).
     */
    class DebugDraw
    {
    public:
        //! Default Constructor.
        DebugDraw();
        //! Default Destructor.
        ~DebugDraw();

        //! Add debug drawing shader program to frame context.
        void Initialize(const std::shared_ptr<FrameContext>& ctx);

        //! Add horizontal grid for debug drawing.
        //! /param numCell number of cells on on x direction and z direction.
        //! /param cellSize each cell size (assume width and height is equal to cell size).
        void AddFloorGrid(const CubbyFlow::Vector2I numCell, float cellSize, const CubbyFlow::Vector3F color);

        //! Add X, Y, Z Axis
        //! Assume each value of axisX, axisY, axisZ is overall it's axis length, not half length.
        void AddCartesianCoordinateAxis(const float axisXLength, const float axisYLength, const float axisZLength);

        //! Add bounding box.
        //! With bounding box min corner vertex and bounding box max corner vertex,
        void AddBoundingBox(const CubbyFlow::BoundingBox3F& boundingBox, const CubbyFlow::Vector3F color);

        //! Clear the bound static data and re-send new data to GPU.
        void ClearVAO();

        //! Draw the Debug wire frame.
        void DrawFrame(const std::shared_ptr<FrameContext>& ctx) const;

        static const size_t kMaxBufferSize = 0x10000; ////! 8 kB - This is CPU memory
    private:
        struct Renderable
        {
            CubbyFlow::Vector3F color; 
            size_t numVertices;
            GLuint vao;
        };

        std::vector<Renderable> _renderables;
    };

};

#endif