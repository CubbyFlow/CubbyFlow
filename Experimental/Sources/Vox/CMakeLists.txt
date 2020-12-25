# Target name
set(target Vox)

# Define
set(root_dir ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# Includes
set(CUBBYRENDER_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${root_dir}/Libraries
    ${root_dir}/../Includes)

if (USE_GL)
    list(APPEND CUBBYRENDER_INCLUDE_DIRS
    ${root_dir}/Libraries/glad)
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
    ${root_dir}/Includes/Vox)

file(GLOB_RECURSE headers
    ${header_dir}/*.hpp)

file(GLOB_RECURSE sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

if (USE_GL)
    file(GLOB sources
        ${sources}
        ${root_dir}/Libraries/glad/glad/glad.cpp)
endif()
if (USE_CUDA)
    file(GLOB sources
        ${sources}
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cu)
endif()

# Build library
add_library(${target} ${sources})

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

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(${target}
    PRIVATE
    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}
	${DEFAULT_LIBRARIES}
    ${CMAKE_DL_LIBS}
    ${OPENGL_LIBRARIES}
    ${PNG_LIBRARIES}
    CubbyFlow
    pystring
    unwind
    pugixml
    INTERFACE
)
else()
    target_link_libraries(${target}
        PRIVATE
        PUBLIC
        ${DEFAULT_LINKER_OPTIONS}
    	${DEFAULT_LIBRARIES}
        ${CMAKE_DL_LIBS}
        ${OPENGL_LIBRARIES}
        ${PNG_LIBRARIES}
        CubbyFlow
        pystring
        pugixml
        INTERFACE
    )
endif()

# Install
install(TARGETS ${target} DESTINATION lib)
install(DIRECTORY ${header_dir} DESTINATION include)