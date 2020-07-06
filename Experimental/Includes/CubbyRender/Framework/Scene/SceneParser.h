/*************************************************************************
> File Name: SceneParser.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: parse scene for get camera, light, etc..
> Created Time: 2020/06/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_SCENEPARSER_H
#define CUBBYFLOW_SCENEPARSER_H

#include <Framework/Scene/SceneObject.h>
#include <Framework/Utils/Prerequisites.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <vector>
#include <unordered_map>

namespace CubbyFlow {
namespace CubbyRender {

    //!
    //! \brief Simulation Scene Parsing Util class
    //!
    class SceneParser final
    {
    public: 
        //! Default Constructor
        SceneParser();

        //! Constructor with scene file path.
        SceneParser(const std::string& path);

        //! Default Destructor.
        ~SceneParser();

        //! Load scene objects' information from the scn file.
        void loadScene(const std::string& path);
        //! Create scene object instance and copy parsed data.
        template <typename Type>
        std::shared_ptr<Type> getSceneObject(const std::string& objName);
    protected:
    private:
        void onLoadScene(const nlohmann::json& json);

        template <typename Type>
        void onLoadSceneObject(const nlohmann::json& json);

        std::unordered_map<std::string, std::shared_ptr<SceneObject>> _metadata;
    };

    using SceneParserPtr = std::shared_ptr<SceneParser>;
} 
}

#include <Framework/Scene/SceneParser-Impl.h>

#endif