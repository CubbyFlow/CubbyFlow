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
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief The Viewport class matched with camera view
    //!
    class Viewport 
    {
    public:
        //! Default constructor.
        Viewport();

        //! Default destructor.
        ~Viewport();

    protected:
    private:
    };

    using ViewportPtr = std::shared_ptr<Viewport>;
} 
}

#endif