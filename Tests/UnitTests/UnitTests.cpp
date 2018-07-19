#include "pch.h"

#include <Utils/Logging.h>

#include <fstream>

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	std::ofstream logFile("UnitTests.log");
	if (logFile)
	{
		CubbyFlow::Logging::SetAllStream(&logFile);
	}

	int ret = RUN_ALL_TESTS();

	return ret;
}