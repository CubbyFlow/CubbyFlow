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

# Find TBB
include(../Builds/CMake/FindLibunwind.cmake)
find_package(Libunwind REQUIRED)
include_directories(LIBUNWIND_INCLUDE_DIR)

# include header-only stb library for image read & write
include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries)
include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/nlohmann)

set(OPENGL_LIBRARIES "")

# OpenGL
option(USE_GL "Use OpenGL features" ON)
if (USE_GL)
    find_package(OpenGL REQUIRED)
    
    if (OPENGL_FOUND)
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
                if (GLFW_USE_OSMESA)
                    set(GLFW_LIBRARY
                        glfw OSMesa)
                else()
                    set(GLFW_LIBRARY
                        glfw
                        rt m dl Xrandr Xinerama Xcursor Xext Xrender Xfixes X11 pthread
                        xcb Xau Xdmcp)
                endif()
                
            endif()
        endif()
        
        set(OPENGL_LIBRARIES 
            ${GLFW_LIBRARY}
            ${COREFOUNDATION_LIB})

        # Include the other directories
        include_directories(${CMAKE_CURRENT_LIST_DIR}/Includes/Vox)
        include_directories(${CMAKE_CURRENT_LIST_DIR}/Includes)
        include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/glad)

    else()
        set(USE_GL OFF)
    endif()
endif()

# CUDA
set(CUDA_LIBRARIES "")
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

        add_subdirectory(Libraries/gvdb-voxels/gvdb_library)
        include_directories(${CMAKE_CURRENT_LIST_DIR}/Libraries/gvdb-voxels/gvdb_library/src)
        set(GVDB_LIBRARY gvdb)

        set(CUDA_LIBRARIES 
            ${GVDB_LIBRARY})

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
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Sources/Vox)
if (USE_GL)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Examples/Vox/ParticleViewer)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Examples/Vox/FluidMeshViewer)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Tests/RenderTests)
    if (USE_CUDA)
        add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Examples/gvdbExamples/InteractiveFluid)
        add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Examples/gvdbExamples/RenderToFiles)
    endif()
endif()
if (APPLE)
    # add_subdirectory(src/examples/metal_examples)
endif()


if(CMAKE_BUILD_TYPE MATCHES Debug AND CMAKE_COMPILER_IS_GNUCXX)
    include(CodeCoverage)
    setup_target_for_coverage(Experimental_coverage RenderTests coverage)
endif()
