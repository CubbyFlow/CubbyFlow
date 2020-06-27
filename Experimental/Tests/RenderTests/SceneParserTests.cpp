#include "pch.h"

#include <Framework/Utils/SceneParser.h>
#include <fstream>

using namespace CubbyFlow;
using namespace CubbyRender;

TEST(SceneParser, SceneParsing)
{
	SceneParser parser;
    parser.loadScene(RESOURCES_DIR "testscene.json");
}

