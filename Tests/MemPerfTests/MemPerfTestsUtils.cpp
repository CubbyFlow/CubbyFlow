#include "MemPerfTestsUtils.h"

#include <string>
#include <utility>

std::pair<double, std::string> MakeReadableByteSize(size_t bytes)
{
    double s = static_cast<double>(bytes);
    std::string unit = "B";

    if (s > 1024)
    {
        s /= 1024;
        unit = "kB";
    }

    if (s > 1024)
    {
        s /= 1024;
        unit = "MB";
    }

    if (s > 1024)
    {
        s /= 1024;
        unit = "GB";
    }

    if (s > 1024)
    {
        s /= 1024;
        unit = "TB";
    }

    return std::make_pair(s, unit);
}