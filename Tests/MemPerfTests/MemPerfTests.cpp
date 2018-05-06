#include "gtest/gtest.h"

#include <Core/Utils/Logging.h>

#include <fstream>

using namespace CubbyFlow;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    std::ofstream logFile("MemPerfTests.log");
    if (logFile)
    {
        Logging::SetAllStream(&logFile);
    }

    int ret = RUN_ALL_TESTS();

    return ret;
}