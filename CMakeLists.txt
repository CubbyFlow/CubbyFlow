# CMake version
cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Builds/CMake")

# Declare project
project(CubbyFlow)

# Set output directories
set(DEFAULT_CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Set enable output of compile commands during generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Includes
include_directories(Includes)
include_directories(Libraries)
include_directories(Libraries/pybind11/include)
include_directories(Libraries/googletest/googletest/include)
include_directories(Libraries/Clara/include)
include_directories(Libraries/tinyobj)

# Find TBB
include(Builds/CMake/FindTBB.cmake)

# Tasking system options
include(Builds/CMake/TaskingSystemOptions.cmake)

# Compile options
include(Builds/CMake/CompileOptions.cmake)

# Build type - Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

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
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(Libraries/googletest EXCLUDE_FROM_ALL)
cmake_policy(SET CMP0063 NEW)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(Libraries/googlebenchmark)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
add_subdirectory(Libraries/cnpy)
add_subdirectory(Libraries/pystring)
add_subdirectory(Libraries/imgui)
add_subdirectory(Sources/Core)
add_subdirectory(Tests/ManualTests)
add_subdirectory(Tests/UnitTests)
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
add_subdirectory(Examples/ExampleHelper)

add_subdirectory(Libraries/pybind11)
if(BUILD_FROM_PIP)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DEFAULT_CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endif()
add_subdirectory(Sources/API/Python)

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    include(CodeCoverage)
    setup_target_for_coverage(${PROJECT_NAME}_coverage UnitTests coverage)
endif()