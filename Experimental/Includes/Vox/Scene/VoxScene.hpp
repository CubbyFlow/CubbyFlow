/*************************************************************************
> File Name: SceneParser.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Vox Scene Object Manager class.
> Created Time: 2020/08/08
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_SCENE_HPP
#define CUBBYFLOW_VOX_SCENE_HPP

#include <Vox/Utils/FileSystem.hpp>
#include <Core/Array/Array1.hpp>
#include <memory>
#include <vector>
#include <pugixml.hpp>
#include <unordered_map>

namespace Vox {

    class VoxSceneObject;
    //!
    //! Vox Scene Object Manager class.
    //!
    class VoxScene final
    {
    public: 
        //! Default Constructor
        VoxScene();

        //! Constructor with scene file.
        VoxScene(const Vox::Path& path);

        //! Default Destructor.
        ~VoxScene();

        //! Scene Factory
        static std::shared_ptr<VoxScene> CreateScene(const Vox::Path& path);

        //! Load Scene
        void LoadScene(const Vox::Path& path);

        //! Create scene object instance and copy parsed data.
        template <typename Type>
        [[nodiscard]] std::shared_ptr<Type> GetSceneObject(const std::string& objName);

        //! Get all object instances with given template type
        //! This makes application slow. Be careful of that.
        //! Loop whole scene instances and check dynamic_pointer_cast.
        //! Collect all instances which dont return nullptr when checking dynamic_pointer_cast.
        template <typename Type>
        [[nodiscard]] CubbyFlow::Array1<std::shared_ptr<Type>> GetSceneObjects();

        //! Deallocate scene object which is no more need.
        void DeallocateObject(const std::string& objName);
    protected:
    private:
        void OnLoadScene(const pugi::xml_document& document);

        template <typename Type>
        void OnLoadSceneObject(const pugi::xml_node& node);

        std::unordered_map<unsigned int, std::shared_ptr<VoxSceneObject>> _metadata;
    };
}

#include <Vox/Scene/VoxScene-Impl.hpp>

#endif