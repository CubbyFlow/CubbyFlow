#include "pch.h"

#include <Framework/Utils/ObjReconstructor.h>

using namespace CubbyFlow;
using namespace CubbyRender;

TEST(ObjReconstructor, LoadObj)
{
	ObjReconstructor objRecon;
    objRecon.setVertexFormat(VertexFormat::Position3Normal3);

    bool result = objRecon.loadAndReconstruct(std::string(RESOURCES_DIR "objects/bunny.obj"));
    EXPECT_EQ(true, result);
}

