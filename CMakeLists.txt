# CMake version
cmake_minimum_required(VERSION 3.18.6 FATAL_ERROR)

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Builds/CMake")

# Declare project
project(CubbyFlow)

# Useful paths
set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libraries)

# Set output directories
set(DEFAULT_CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Includes
include_directories(Includes)
include_directories(Libraries)
include_directories(Libraries/pybind11/include)
include_directories(Libraries/Clara/include)
include_directories(Libraries/tinyobj)
include_directories(Libraries/doctest/doctest)

# Set enable output of compile commands during generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build type - Release by default
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-DDEBUG)
endif()
message(STATUS "CMake build type: ${CMAKE_BUILD_TYPE}")

# CUDA
option(USE_CUDA "Use CUDA features" ON)
set(CUDA_CRT_LINKAGE "dynamic" CACHE STRING 
                     "CUDA targets' CRT options. For 'static', select /MT. For 'dynamic', select /MD")
if (USE_CUDA)
    message(STATUS "Using CUDA CRT: ${CUDA_CRT_LINKAGE}")
    find_package(CUDA)
    if (CUDA_FOUND)
        set(CUDA_LINK_LIBRARIES_KEYWORD PUBLIC)
        list(APPEND CUDA_TARGET_OPTIONS
            "--std=c++17" "-Wno-deprecated-gpu-targets")
        add_definitions(-DCUBBYFLOW_USE_CUDA)
        include_directories(${CUDA_INCLUDE_DIRS})

        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
                /wd4515 # -> disable warning: [Thrust issue] 'detail': namespace uses itself
                /wd4996 # -> disable warning: [Thrust issue] 'strerror': This function or variable may be unsafe.
                /wd4800 # -> disable warning: [Thrust issue] forcing value to bool 'true' or 'false' (performance warning)
            )
        endif()
    else()
        set(USE_CUDA OFF)
    endif()
endif()

# Find TBB
include(Builds/CMake/FindTBB.cmake)

# Tasking system options
include(Builds/CMake/TaskingSystemOptions.cmake)

# Compile options
include(Builds/CMake/CompileOptions.cmake)

# Modify project options for CUDA build
if (USE_CUDA)
    enable_language(CUDA)
    set(CUDA_STANDARD 17)
    set(DEFAULT_PROJECT_OPTIONS ${DEFAULT_PROJECT_OPTIONS}
            CUDA_SEPARABLE_COMPILATION ON)
endif()

# Code coverage - Debug only
# NOTE: Code coverage results with an optimized (non-Debug) build may be misleading
option(BUILD_COVERAGE "Build code coverage" OFF)
if (CMAKE_BUILD_TYPE MATCHES Debug AND CMAKE_COMPILER_IS_GNUCXX AND BUILD_COVERAGE)
    include(CodeCoverage)
    setup_target_for_coverage(${PROJECT_NAME}_coverage UnitTests coverage)
endif()

# Some project don't build - SonarCloud only
option(BUILD_SONARCLOUD "Build for SonarCloud" OFF)

# Overrides
set(CMAKE_MACOSX_RPATH ON)

# Set resources
set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources)
set(RESOURCES_OBJS
        ${RESOURCES_DIR}/bunny.obj
        ${RESOURCES_DIR}/cube.obj
        ${RESOURCES_DIR}/dragon.obj)

# Project modules
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "")
if (MSVC AND USE_CUDA)
    message(STATUS "Google.Test Shared CRT: ${gtest_force_shared_crt}")
    if (gtest_force_shared_crt AND CUDA_CRT_LINKAGE STREQUAL "static")
        message(WARNING "CRT option mismatch. 'gtest_force_shared_crt' must be OFF for CUDA targets")
    elseif (NOT gtest_force_shared_crt AND CUDA_CRT_LINKAGE STREQUAL "dynamic")
        message(WARNING "CRT option mismatch. 'gtest_force_shared_crt' must be ON for CUDA targets")
    endif()
endif()

add_subdirectory(Libraries/googletest EXCLUDE_FROM_ALL)
cmake_policy(SET CMP0063 NEW)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(Libraries/googlebenchmark)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
add_subdirectory(Libraries/cnpy)
add_subdirectory(Libraries/pystring)
add_subdirectory(Sources/Core)
add_subdirectory(Tests/ManualTests)
add_subdirectory(Tests/UnitTests)
if (NOT BUILD_SONARCLOUD)
    add_subdirectory(Tests/MemPerfTests)
    add_subdirectory(Tests/TimePerfTests)
    add_subdirectory(Examples/HelloFluidSim)
    add_subdirectory(Examples/HybridLiquidSim)
    add_subdirectory(Examples/LevelSetLiquidSim)
    add_subdirectory(Examples/Obj2Sdf)
    add_subdirectory(Examples/Particles2Obj)
    add_subdirectory(Examples/Particles2Xml)
    add_subdirectory(Examples/SmokeSim)
    add_subdirectory(Examples/SPHSim)
endif()

if (NOT USE_CUDA AND NOT BUILD_SONARCLOUD)
    add_subdirectory(Libraries/pybind11)
    if (BUILD_FROM_PIP)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DEFAULT_CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    endif()
    add_subdirectory(Sources/API/Python)
endif()