# Target name
set(target imgui)

# Sources
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../glad)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../glfw/include)
file(GLOB headers
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../glad/glad/*.h)

file(GLOB sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../glad/glad/glad.cpp)

# Build library
add_library(${target}
    ${sources})

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

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}
    ${GLFW_LIBRARY}

    INTERFACE
)