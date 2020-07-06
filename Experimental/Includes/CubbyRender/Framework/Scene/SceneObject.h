/*************************************************************************
> File Name: SceneObject.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for scene object
> Created Time: 2020/07/05
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_SCENE_OBJECT_H
#define CUBBYFLOW_SCENE_OBJECT_H

#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Abstract base class for scene object
    //!
    class SceneObject
    {
    public:
        //! Default constructor.
        SceneObject() = default;

        //! Default destructor.
        virtual ~SceneObject() = 0;

    using SceneObjectPtr = std::shared_ptr<SceneObject>;
    };
} 
}

#endif