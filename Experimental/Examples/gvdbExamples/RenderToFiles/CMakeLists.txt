# Target name
set(target RenderToFiles)
project(${target} LANGUAGES CUDA CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../sample_utils")
set ( REQUIRE_OPENGL "0" )
set ( REQUIRE_PNG "1" )
set ( REQUIRE_TGA "0" )
set ( REQUIRE_GLEW "0" )
set ( REQUIRE_MAIN "0" )
set ( REQUIRE_NVGUI "0" )
set ( REQUIRE_OPTIX "0" )
find_package(Utils REQUIRED)
find_package(Optix REQUIRED)

# Includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../sample_utils)

# Sources
file(GLOB_RECURSE sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Build executable
add_executable(${target}
    ${sources})

target_sources(${target}
    PRIVATE 
    ${UTIL_SOURCE_FILES}
)

# Project options
set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
)

# Compile options
target_compile_options(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_COMPILE_OPTIONS}

    INTERFACE
)
target_compile_definitions(${target}
    PRIVATE
    PUBLIC
    RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../../Resources/"
    USE_GVDB="1"
)

add_dependencies(${target} gvdbCopy)

# Link libraries
target_link_libraries(${target}
    PUBLIC
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
    ${DEFAULT_LIBRARIES}
    ${CUDA_LIBRARIES}
    CubbyFlow
    pystring)


# Finally, copy the GVDB library itself to the executable directory:
add_custom_command(OUTPUT copiedGVDB.stamp
    COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:gvdb> $<TARGET_FILE_DIR:${target}>
    COMMAND ${CMAKE_COMMAND} -E touch copiedGVDB.stamp)