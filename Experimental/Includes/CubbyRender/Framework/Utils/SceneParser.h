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

        //! Default Destructor.
        ~SceneParser();

        //! Write scene objects' information to the scn file.
        void writeScene(const std::string& path);
        //! Load scene objects' information from the scn file.
        void loadScene(const std::string& path);
        //! Create scene object instance and copy parsed data.
        template <typename SceneObject>
        std::shared_ptr<SceneObject> getSceneObject(const std::string& objName);
    protected:
    private:
        void onLoadScene(const nlohmann::json& json);

        template <typename SceneObject>
        void onLoadSceneObject(const std::string& name, SceneObject object);

        template <typename SceneObject>
        void parseSceneObject(const nlohmann::json& json);

        std::unordered_map<std::string, std::vector<unsigned char>> _metadata;
    };

    using SceneParserPtr = std::shared_ptr<SceneParser>;
} 
}

#include <Framework/Utils/SceneParser-Impl.h>

#endif