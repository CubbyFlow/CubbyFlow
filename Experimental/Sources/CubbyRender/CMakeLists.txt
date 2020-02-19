# Target name
set(target CubbyRender)

# Define
set(root_dir ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# Includes
set(CUBBYRENDER_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${root_dir}/../Libraries)

if (USE_GL)
    list(APPEND CUBBYRENDER_INCLUDE_DIRS
    ${root_dir}/../Libraries/gl3w)
endif()
if (USE_CUDA)
list(APPEND CUBBYRENDER_INCLUDE_DIRS
        ${CUDA_INCLUDE_DIRECTORIES})
endif()

include_directories(
    ${CUBBYRENDER_INCLUDE_DIRS}
)

# Sources
file(GLOB header_dir
    ${root_dir}/Includes/CubbyRender)

file(GLOB_RECURSE headers
    ${header_dir}/*.h)

file(GLOB_RECURSE sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${root_dir}/../Libraries/obj/*.cpp)

if (USE_GL)
    file(GLOB sources
        ${sources}
        ${root_dir}/../Libraries/gl3w/GL/gl3w.cpp)
endif()
if (USE_CUDA)
    file(GLOB sources
        ${sources}
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cu)
endif()

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
	${DEFAULT_LIBRARIES}
    ${CMAKE_DL_LIBS}
    ${OPENGL_LIBRARIES}
    EGL

    INTERFACE
)

# Install
install(TARGETS ${target} DESTINATION lib)
install(DIRECTORY ${header_dir} DESTINATION include)