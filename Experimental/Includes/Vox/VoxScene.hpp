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

#include <Vox/FileSystem.hpp>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
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
        std::shared_ptr<Type> GetSceneObject(const std::string& objName);
    protected:
    private:
        void OnLoadScene(const nlohmann::json& json);

        template <typename Type>
        void OnLoadSceneObject(const nlohmann::json& json);

        std::unordered_map<std::string, std::shared_ptr<VoxSceneObject>> _metadata;
    };
}

#include <Vox/VoxScene-Impl.hpp>

#endif