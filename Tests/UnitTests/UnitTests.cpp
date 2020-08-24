#include "pch.hpp"

#include <Core/Utils/Logging.hpp>

#include <fstream>

int main(int argc, char* argv[])
{
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    std::ofstream logFile("UnitTests.log");
    if (logFile)
    {
        CubbyFlow::Logging::SetAllStream(&logFile);
    }

    int ret = RUN_ALL_TESTS();

    return ret;
}