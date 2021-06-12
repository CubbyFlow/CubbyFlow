# Target name
set(target CUDATests)

# Sources
file(GLOB_RECURSE sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cu)

# Build executable
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if (CUDA_CRT_LINKAGE STREQUAL "dynamic")
        if (CMAKE_BUILD_TYPE MATCHES Debug)
            list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/MDd" "--debug" "-O0")
        else()
            list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/MD" "-O2")
        endif()
    elseif (CUDA_CRT_LINKAGE STREQUAL "static")
        if(CMAKE_BUILD_TYPE MATCHES Debug)
            list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/MTd" "--debug" "-O0")
        else()
            list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/MT" "-O2")
        endif()
    endif()
endif()
cuda_add_executable(${target} ${sources}
    OPTIONS ${CUDA_TARGET_OPTIONS}
)

# Project options
set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    CUDA_ARCHITECTURES "75"
    CUDA_SEPARABLE_COMPILATION ON
)

# Compile options
# GCC and Clang compiler options
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
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
    CubbyFlow)