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

# Recording On
option(RECORDING "Use libpng for recording simulation result" OFF)
if (RECORDING)
    message("-- Using libpng for recording simulation result")
    add_definitions(-DCUBBYFLOW_RECORDING)
    add_custom_target(pnglibconfig_copy COMMAND cp scripts/pnglibconf.h.prebuilt ./pnglibconf.h
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/libpng
    )
    add_subdirectory(Libraries/libpng)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/libpng)
    set(FFMPEG_LIBRARIES 
        png)
else()
    set(FFMPEG_LIBRARIES )
endif()

# include header-only stb library for image read & write
include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/stb)

# OpenGL
option(USE_GL "Use OpenGL features" ON)
if (USE_GL)
    find_package(OpenGL REQUIRED)
    
    if (OPENGL_FOUND)
        # Define macro CUBBYRENDER_USE_GL. 
        add_definitions(-DCUBBYFLOW_USE_GL)

        # Ignore warning 4152 ( nonstandard extension, function/data pointer conversion in expression ) for building gl3w library.
        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
                /wd4152
                /wd4244 # While building googlebenchmark, Warning C4244: '=': conversion from 'int' to 'char', possible loss of data occurred
            )
        endif()

        include(GNUInstallDirs)
        include(CMakeDependentOption)
        
        if (GLFW_USE_OSMESA)
            find_package(OSMesa REQUIRED)
            add_definitions(-DCUBBYFLOW_HEADLESS)
        endif()
        
        # Add GLFW
        add_subdirectory(Libraries/glfw)
        include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/glfw/include)
        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            set(GLFW_LIBRARY glfw)
        else()
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

        # Add Imgui
        add_subdirectory(Libraries/imgui)
        include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui)
        set(IMGUI_LIBRARY imgui)

        set(OPENGL_LIBRARIES 
            ${OPENGL_LIBRARIES}
            ${GLFW_LIBRARY}
            ${GLEW_LIBRARY}
            ${EGL_LIBRARY}
            ${IMGUI_LIBRARY}
            ${COREFOUNDATION_LIB})

        # Add GLI which is header only opengl image loader library
        include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/gli)

        set(OPENGL_LIBRARIES 
            ${OPENGL_LIBRARIES}
            ${GLFW_LIBRARY}
            ${GLEW_LIBRARY}
            ${EGL_LIBRARY}
            ${IMGUI_LIBRARY}
            ${COREFOUNDATION_LIB})

        # Include other directories
        include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/glad)
        include_directories(${CMAKE_CURRENT_LIST_DIR}/Includes/CubbyRender)

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

# Project dependencies
