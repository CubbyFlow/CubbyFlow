/*************************************************************************
> File Name: VoxSceneObject.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for scene object
> Created Time: 2020/07/05
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_SCENE_OBJECT_HPP
#define CUBBYFLOW_VOX_SCENE_OBJECT_HPP

#include <memory>

namespace Vox {
    
    //!
    //! \brief Abstract base class for scene object
    //!
    class VoxSceneObject
    {
    public:
        //! Default constructor.
        VoxSceneObject() = default;

        //! Default destructor.
        virtual ~VoxSceneObject() {};
    };
} 

#endif