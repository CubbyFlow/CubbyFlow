#include "benchmark/benchmark.h"

#include <Utils/Logging.h>

#include <fstream>

int main(int argc, char** argv)
{
    ::benchmark::Initialize(&argc, argv);

    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
    {
        return 1;
    }

    std::ofstream logFile("mem_perf_tests.log");
    if (logFile)
    {
        CubbyFlow::Logging::SetAllStream(&logFile);
    }

    ::benchmark::RunSpecifiedBenchmarks();
}