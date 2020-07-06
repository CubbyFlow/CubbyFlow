#include "pch.h"
#include "RenderTestsUtils.h"

#include <Framework/Scene/SceneParser.h>
#include <Framework/View/PerspectiveCamera.h>
#include <Framework/View/Camera.h>
#include <Core/Vector/Vector3.h>
#include <fstream>

using namespace CubbyFlow;
using namespace CubbyRender;

TEST(SceneParser, SceneParsing)
{
	SceneParser parser;
    parser.loadScene(RESOURCES_DIR "testscene.json");
    auto camera = parser.getSceneObject<PerspectiveCamera>("camera");
    
    EXPECT_NEAR(0.01f, camera->getPivot().zNear, 0.00001f);
}

