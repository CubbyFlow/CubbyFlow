/*************************************************************************
> File Name: Viewport.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The Viewport class matched with camera view
> Created Time: 2020/04/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VIEWPORT_H
#define CUBBYFLOW_VIEWPORT_H

#include <Framework/Utils/Prerequisites.h>
#include <Core/Vector/Vector3.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief The Viewport class matched with camera view
    //!
    struct Viewport 
    {
    public:
        float getWidth() const;

        float getHeight() const;

        bool operator==(const Viewport& other) const;
        bool operator!=(const Viewport& other) const;

        Vector2F leftTop;      
        Vector2F rightBottom;  

    protected:
    private:
    };
} 
}

#endif