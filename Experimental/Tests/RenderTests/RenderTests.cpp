#include "gtest/gtest.h"
#include <Vox/Utils/FileSystem.hpp>
#include <Core/Utils/Logging.hpp>
#include <fstream>

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	std::ofstream logFile("RenderTests.log");
	if (logFile)
	{
		CubbyFlow::Logging::SetAllStream(&logFile);
	}

	Vox::FileSystem::AddDirectory(RESOURCES_DIR);
	Vox::FileSystem::AddDirectory(VOX_RESOURCES_DIR);
	int ret = RUN_ALL_TESTS();

	return ret;
}