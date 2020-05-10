# Target name
set(target ParticleSimulation)

# Includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Utils)
# Sources
file(GLOB sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Utils/*.cpp)

# Build executable
add_executable(${target}
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
target_compile_definitions(${target}
    PRIVATE
    RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../../Resources/"
)

# Link libraries
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_libraries(${target}
        PRIVATE
        ${DEFAULT_LINKER_OPTIONS}
        ${DEFAULT_LIBRARIES}
        CubbyFlow
        CubbyRender
        pystring)
else()
    target_link_libraries(${target}
        PRIVATE
        ${DEFAULT_LINKER_OPTIONS}
        ${DEFAULT_LIBRARIES}
        CubbyFlow
        CubbyRender
        pystring)
endif()