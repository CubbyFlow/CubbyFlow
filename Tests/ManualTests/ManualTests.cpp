#include "ManualTests.hpp"
#include "pch.hpp"

#include <Core/Utils/Logging.hpp>

#include <fstream>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    CreateDirectory(CUBBYFLOW_TEST_OUTPUT_DIR);

    std::ofstream logFile("manual_tests.log");
    if (logFile)
    {
        CubbyFlow::Logging::SetAllStream(&logFile);
    }

    int ret = RUN_ALL_TESTS();

    return ret;
}
