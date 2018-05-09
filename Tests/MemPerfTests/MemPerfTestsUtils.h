#ifndef MEM_PERF_TESTS_UTILS_H
#define MEM_PERF_TESTS_UTILS_H

#include <string>
#include <utility>
#include <vector>

size_t GetCurrentRSS();

std::pair<double, std::string> MakeReadableByteSize(size_t bytes);

#endif