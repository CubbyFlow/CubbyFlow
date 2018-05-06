#ifndef MEM_PERF_TESTS_UTILS_H
#define MEM_PERF_TESTS_UTILS_H

#include <string>
#include <utility>
#include <vector>

// gtest hack
namespace testing
{
	namespace internal
	{
		enum GTestColor
		{
			COLOR_DEFAULT,
			COLOR_RED,
			COLOR_GREEN,
			COLOR_YELLOW
		};

		extern void ColoredPrintf(GTestColor color, const char* fmt, ...);
	}
}

size_t GetCurrentRSS();

std::pair<double, std::string> MakeReadableByteSize(size_t bytes);

#define CUBBYFLOW_PRINT_INFO(fmt, ...) \
	testing::internal::ColoredPrintf( \
		testing::internal::COLOR_YELLOW,  "[   STAT   ] "); \
	testing::internal::ColoredPrintf( \
		testing::internal::COLOR_YELLOW, fmt, __VA_ARGS__); \

#endif