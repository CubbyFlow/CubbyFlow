# Target name
set(target UnitTests)

# Includes
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EXTERNAL_DIR}/googletest/googlemock/include
    ${EXTERNAL_DIR}/googletest/googletest/include)

# Sources
if (USE_CUDA)
    file(GLOB sources
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cu)
else()
    file(GLOB sources
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
endif()

# Build executable
if (USE_CUDA)
    cuda_add_executable(${target}
        ${sources})
else()
    add_executable(${target}
        ${sources})
endif()

# Project options
set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
)

# Compile options
# GCC and Clang compiler options
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
        # /wd4996       # -> disable warning: non-Standard std::tr1 namespace and TR1-only machinery (because of gtest)		
        -Wno-unused-variable
        )
endif ()
target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)
target_compile_definitions(${target}
    PRIVATE
    RESOURCES_DIR="${RESOURCES_DIR}"
)

# Link libraries
target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
    CubbyFlow
    gmock
    gtest)