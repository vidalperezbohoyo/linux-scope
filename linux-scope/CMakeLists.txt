cmake_minimum_required(VERSION 3.10)
project(linux_scope)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#################################################################
# Compilation flags:
# BUILD_x86: Build for PC, development and testing purposes.
# BUILD_LUCKFOX_PICO: Build for Luckfox Pico, using the OpenCV-Mobile library.

# Example: cmake .. -D BUILD_X86=ON
#################################################################

if(BUILD_X86)
    message(STATUS "Building for x86 (PC)")
    add_definitions(-DBUILD_X86)
elseif(BUILD_LUCKFOX_PICO)
    message(STATUS "Building for Luckfox Pico")
    add_definitions(-DBUILD_LUCKFOX_PICO)

    set(SDK_PATH $ENV{PICO_SDK_PATH})
    if (NOT SDK_PATH)
        message(FATAL_ERROR "PICO_SDK_PATH environment variable is not set. Please set it to the path of the Pico SDK.")
    endif()
    message(STATUS "Using Pico SDK at: ${SDK_PATH}")

    SET(CMAKE_C_COMPILER "${SDK_PATH}/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-gcc")
    SET(CMAKE_CXX_COMPILER "${SDK_PATH}/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-g++")

    set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/opencv-mobile-4.10.0-luckfox-pico/lib/cmake/opencv4")

    #############################################################
    # RGA (Rockchip Raster Graphic Acceleration) para conversion
    # de color por hardware (usado en V4L2Capture.cpp)
    #############################################################
   
    set(RGA_PATH "${SDK_PATH}/media/rga/release_rga_rv1106_arm-rockchip830-linux-uclibcgnueabihf" CACHE PATH "Path to librga")

    find_path(RGA_INCLUDE_DIR
        NAMES rga/im2d.hpp
        PATHS ${RGA_PATH}/include
        NO_DEFAULT_PATH
    )

    find_library(RGA_LIBRARY
        NAMES rga
        PATHS ${RGA_PATH}/lib
        NO_DEFAULT_PATH
    )

    if (NOT RGA_INCLUDE_DIR OR NOT RGA_LIBRARY)
        message(FATAL_ERROR "librga not found. Set RGA_PATH to the correct location "
                             "(expected headers in <RGA_PATH>/include/rga and lib in <RGA_PATH>/lib).")
    endif()

    message(STATUS "Found RGA include dir: ${RGA_INCLUDE_DIR}")
    message(STATUS "Found RGA library: ${RGA_LIBRARY}")

else()
    message(FATAL_ERROR "No build target specified. Please set BUILD_X86 or BUILD_LUCKFOX_PICO.")
endif()


find_package(OpenCV REQUIRED)

add_executable(scope 
    src/main.cpp
    
    src/Camera/VideoProvider.cpp

    src/UserInput/KeyboardController.cpp
    
    src/Utility/Thread.cpp
)

if (BUILD_X86)
    target_sources(scope PRIVATE
        src/Display/CvDisplay.cpp
    )
elseif (BUILD_LUCKFOX_PICO)
    target_sources(scope PRIVATE
        src/Display/FbDisplay.cpp

    )

    add_executable(test_luckfox_display 
        src/test/test_luckfox_display.cpp
        src/Display/FbDisplay.cpp
    )
    add_executable(test_luckfox_camera_fps
        src/test/test_luckfox_camera_fps.cpp
        src/Utility/Thread.cpp
        src/Camera/VideoProviderLuckfox.cpp
        src/Camera/V4L2Capture.cpp
    )
    add_executable(test_luckfox_camera_to_display_manager
        src/test/test_luckfox_camera_to_display_manager.cpp
        src/Display/FbDisplay.cpp
        src/Display/DisplayManager.cpp
        src/Utility/Thread.cpp
        src/Camera/VideoProviderLuckfox.cpp
        src/Camera/V4L2Capture.cpp
    )

    add_executable(test_luckfox_v4l2_raw
        src/test/test_luckfox_v4l2_raw.cpp
        src/Display/FbDisplay.cpp
    )

    target_include_directories(test_luckfox_display PRIVATE 
        include
        ${OpenCV_INCLUDE_DIRS}
    )
    target_include_directories(test_luckfox_camera_fps PRIVATE 
        include
        ${OpenCV_INCLUDE_DIRS}
        ${RGA_INCLUDE_DIR}
    )
    target_include_directories(test_luckfox_camera_to_display_manager PRIVATE 
        include
        ${OpenCV_INCLUDE_DIRS}
        ${RGA_INCLUDE_DIR}
    )
    target_include_directories(test_luckfox_v4l2_raw PRIVATE 
        include
        ${OpenCV_INCLUDE_DIRS}
        ${RGA_INCLUDE_DIR}
    )

    target_link_libraries(test_luckfox_display ${OpenCV_LIBS})
    target_link_libraries(test_luckfox_camera_fps ${OpenCV_LIBS} ${RGA_LIBRARY})
    target_link_libraries(test_luckfox_camera_to_display_manager ${OpenCV_LIBS} ${RGA_LIBRARY})
    target_link_libraries(test_luckfox_v4l2_raw ${OpenCV_LIBS} ${RGA_LIBRARY})

endif()

target_include_directories(scope PRIVATE 
    include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(scope ${OpenCV_LIBS})


# Compiler options
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(scope PRIVATE -Wall -Wextra -O2)
endif()

# Build types
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")