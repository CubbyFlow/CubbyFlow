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
        # Define macro CUBBYRENDER_USE_GL. 
        add_definitions(-DCUBBYFLOW_USE_GL)

        # Ignore warning 4152 ( nonstandard extension, function/data pointer conversion in expression ) for building gl3w library.
        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
                /wd4152
            )
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

        # Add glew
        SET(glew-cmake_BUILD_SHARED OFF CACHE BOOL "turn off generating shared library")
        SET(glew-cmake_BUILD_MULTI_CONTEXT OFF CACHE BOOL "turn off multi context")
        add_subdirectory(Libraries/glew-cmake)
        include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/glew-cmake/include)
        set(GLEW_LIBRARY libglew_static)

        # Add EGL
        #include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/EGL-Registry/api)
        #set(EGL_LIBRARY EGL)

        # Add EGL
        find_library(
            EGL_FOUND
            NAMES EGL.so
            HINTS /usr/lib /usr/lib64)
        if(EGL_FOUND)
            set(EGL_LIBRARY EGL)
        else()
            message("-- Could not find EGL")
            include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/EGL/EGL/include)
            add_subdirectory(Libraries/EGL/EGL)
            set(EGL_LIBRARY egl)
        endif()
        # 

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

        # Include other directories
        include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/gl3w)
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
