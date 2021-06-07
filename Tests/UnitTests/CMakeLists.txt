# Target name
set(target UnitTests)

# Includes
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LIBRARY_DIR}/googletest/googlemock/include
    ${LIBRARY_DIR}/googletest/googletest/include)

# Sources
if (USE_CUDA)
    file(GLOB sources
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CUDA/*.cu)
else()
    file(GLOB sources
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
endif()

# Build executable
if (USE_CUDA)
    if (MSVC)
        if (CUDA_CRT_LINKAGE STREQUAL "dynamic")
            if (CMAKE_BUILD_TYPE MATCHES Debug)
                list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/MDd" "--debug" "-O0")
            else()
                list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/MD" "-O2")
            endif()
        elseif (CUDA_CRT_LINKAGE STREQUAL "static")
            # TODO: We have to use /MT here in general, 
            #       but using /MD because of Google.Test source build
            if(CMAKE_BUILD_TYPE MATCHES Debug)
                list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/MDd" "--debug" "-O0") # /MTd
            else()
                list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/MD" "-O2") # /MT
            endif()
        endif()
    endif()
    # See https://cmake.org/cmake/help/latest/module/FindCUDA.html
    cuda_add_executable(${target} ${sources}
        OPTIONS ${CUDA_TARGET_OPTIONS}
    )
else()
    add_executable(${target}
        ${sources})
endif()

# Project options
set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    CUDA_ARCHITECTURES "75"
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
    CubbyFlow
    gmock
    gtest)