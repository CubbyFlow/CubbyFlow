# CMake version
cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

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

# Set enable output of compile commands during generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build type - Release by default
message("CMake build type: " ${CMAKE_BUILD_TYPE})
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-DDEBUG)
endif()

# CUDA
option(USE_CUDA "Use CUDA features" ON)
if (USE_CUDA)
    find_package(CUDA)

    if (CUDA_FOUND)
        set(CUDA_LINK_LIBRARIES_KEYWORD PUBLIC)
        list(APPEND CUDA_NVCC_FLAGS -Wno-deprecated-gpu-targets)
        add_definitions(-DCUBBYFLOW_USE_CUDA)
        include_directories(${CUDA_INCLUDE_DIRS})

        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
                /wd4515 # -> disable warning: [Thrust issue] 'detail': namespace uses itself
                /wd4996 # -> disable warning: [Thrust issue] 'strerror': This function or variable may be unsafe.
                /wd4800 # -> disable warning: [Thrust issue] forcing value to bool 'true' or 'false' (performance warning)
            )
        else()
            list(APPEND CUDA_NVCC_FLAGS --std=c++17)
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
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
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

    add_subdirectory(Libraries/pybind11)
    if(BUILD_FROM_PIP)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DEFAULT_CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    endif()
    add_subdirectory(Sources/API/Python)
endif()