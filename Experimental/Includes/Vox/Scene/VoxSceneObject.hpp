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

#include <Vox/Core/NonCopyable.hpp>
#include <pugixml.hpp>
#include <memory>

namespace Vox {
    
    class VoxScene;

    //!
    //! \brief Abstract base class for scene object
    //!
    class VoxSceneObject : public NonCopyable
    {
    public:
        //! Default constructor.
        VoxSceneObject() = default;

        //! Default destructor.
        virtual ~VoxSceneObject() {};
        
        //! Load scene object attributes from the xml node
        virtual void LoadXMLNode(VoxScene* scene, const pugi::xml_node& node) = 0;

        //! Write this scene object attributes to the given documents.
        virtual void WriteXMLNode(pugi::xml_node& node) = 0;
    };
} 

#endif