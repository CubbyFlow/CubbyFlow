# Target name
set(target RenderToFiles)
project(${target} LANGUAGES CUDA CXX C)

# Includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Utils)
include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/gvdb/gvdb_library/src)

# Sources
file(GLOB sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

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
    PUBLIC
    RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../../Resources/"
)

# Link libraries
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_libraries(${target}
        PRIVATE
        ${DEFAULT_LINKER_OPTIONS}
        ${DEFAULT_LIBRARIES}
        ${CUDA_LIBRARIES}
        CubbyFlow
        pystring)
else()
    target_link_libraries(${target}
        PRIVATE
        ${DEFAULT_LINKER_OPTIONS}
        ${DEFAULT_LIBRARIES}
        ${CUDA_LIBRARIES}
        CubbyFlow
        pystring)
endif()

# Tell Visual Studio to set its debugger working directory to the executable directory:
set_property(TARGET ${target}
    PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${target}>)


# Sample-specific requirements
set ( REQUIRE_OPENGL "1" )
set ( REQUIRE_PNG "1" )
set ( REQUIRE_TGA "0" )
set ( REQUIRE_GLEW "1" )
set ( REQUIRE_MAIN "0" )
set ( REQUIRE_NVGUI "0" )
# Tell NVGUI to use GVDB's vector and camera classes:
target_compile_definitions(${target}
    PRIVATE USE_GVDB="1"
            BUILD_OPENGL="1") # Build with GVDB's OpenGL utilities

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../Utils")
find_package(Utils REQUIRED)
# Then add the utils to the list of source files to build:
target_sources(${target}
    PRIVATE ${UTIL_SOURCE_FILES})

# Finally, copy the GVDB library itself to the executable directory:
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/copiedGVDB.stamp
    COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:gvdb> $<TARGET_FILE_DIR:${target}>
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/copiedGVDB.stamp
    DEPENDS gvdbCopy)
set_property(SOURCE main.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/copiedGVDB.stamp)

# Optionally, install the application into the BIN_INSTALL_PATH set by gvdb_library's CMakeLists.
get_filename_component(BIN_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/bin REALPATH)
install(DIRECTORY "$<TARGET_FILE_DIR:${target}>/" DESTINATION ${BIN_INSTALL_PATH} FILES_MATCHING PATTERN "*.dll" PATTERN "*.glsl" PATTERN "*.ptx" PATTERN "*.so" PATTERN "*.exe" REGEX "/[^.]+$")
