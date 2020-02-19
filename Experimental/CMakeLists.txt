# Target name
set(target Experimental)

# Find Apple-specific libraries
if (APPLE)
    find_library(COCOA_LIB Cocoa)
    find_library(COREFOUNDATION_LIB CoreFoundation)
else()
    set(COCOA_LIB "")
    set(COREFOUNDATION_LIB "")
endif()

# OpenGL
option(USE_GL "Use OpenGL features" ON)
if (USE_GL)
    find_package(OpenGL REQUIRED)
    
    if (OPENGL_FOUND)
        # Ignore warning 4152 ( nonstandard extension, function/data pointer conversion in expression ) for building gl3w library.
        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
                /wd4152
            )
        endif()

        # Define macro CUBBYRENDER_USE_GL. 
        add_definitions(-DCUBBYFLOW_USE_GL)

        # Add GLFW3
        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            find_program(NUGET nuget)
            if (NOT NUGET)
                message(FATAL "Cannot find nuget command line tool.")
            else()
                if ("${CMAKE_GENERATOR}" MATCHES "Win64" OR "${CMAKE_GENERATOR_PLATFORM}" MATCHES "x64")
                    set(MSVC_ARCH "x64")
                else()
                    set(MSVC_ARCH "Win32")
                endif()
                execute_process(COMMAND ${NUGET} install glfw -Version 3.2.1-v141)
                set(MSVC_GLFW_DIR ${PROJECT_BINARY_DIR}/glfw.3.2.1-v141/build/native)
                include_directories(${MSVC_GLFW_DIR}/include)
                link_directories(${MSVC_GLFW_DIR}/lib/${CMAKE_VS_PLATFORM_TOOLSET}/${MSVC_ARCH}/static)
            endif()
            set(GLFW_LIBRARY glfw3)
        else()
            find_package(glfw3 REQUIRED)
            if (APPLE)
                set(GLFW_LIBRARY glfw)
            else()
                # From pkg-config glfw3 --static --libs
                set(GLFW_LIBRARY
                    glfw
                    rt m dl Xrandr Xinerama Xcursor Xext Xrender Xfixes X11 pthread
                    xcb Xau Xdmcp)
            endif()
        endif()

        set(OPENGL_LIBRARIES 
            ${OPENGL_LIBRARIES}
            ${GLFW_LIBRARY}
            ${COREFOUNDATION_LIB})
    else()
        set(USE_GL OFF)
    endif()
endif()

# CUDA
option(USE_CUDA "Use CUDA features" ON)
if (USE_CUDA)
    find_package(CUDA)

    if (CUDA_FOUND)
        set(CUDA_LINK_LIBRARIES_KEYWORD PUBLIC)
        list(APPEND CUDA_NVCC_FLAGS -Wno-deprecated-gpu-targets)
        add_definitions(-DCUBBYFLOW_USE_CUDA)
        include_directories(${CUDA_INCLUDE_DIRS})

        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
                /wd4515 # -> disable warning: [Thrust issue] 'detail': namespace uses itself
                /wd4996 # -> disable warning: [Thrust issue] 'strerror': This function or variable may be unsafe.
                /wd4800 # -> disable warning: [Thrust issue] forcing value to bool 'true' or 'false' (performance warning)

                # Temp
                /wd4505
            )
        else()
            list(APPEND CUDA_NVCC_FLAGS --std=c++14)
        endif()
    else()
        set(USE_CUDA OFF)
    endif()
endif()

# Include directories
include_directories(${CMAKE_CURRENT_LIST_DIR}/../Libraries/gl3w)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../Libraries/glfw/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../Libraries/EGL-Registry/api)
include_directories(${CMAKE_CURRENT_LIST_DIR}/Includes/CubbyRender)
if (WIN32)
    set(USR_LOCAL_INCLUDE_DIR "")
else()
    set(USR_LOCAL_INCLUDE_DIR /usr/local/include)
endif()

# Project modules
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Sources/CubbyRender)
if (USE_GL)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Examples/GL3Examples)
endif()
if (APPLE)
    # add_subdirectory(src/examples/metal_examples)
endif()