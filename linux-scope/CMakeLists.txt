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
        src/Camera/VideoProvider.cpp
        src/Utility/Thread.cpp
    )
    add_executable(test_luckfox_camera_to_display
        src/test/test_luckfox_camera_to_display.cpp
        src/Camera/VideoProvider.cpp
        src/Display/FbDisplay.cpp
        src/Utility/Thread.cpp
    )

    target_include_directories(test_luckfox_display PRIVATE 
        include
        ${OpenCV_INCLUDE_DIRS}
    )
    target_include_directories(test_luckfox_camera_fps PRIVATE 
        include
        ${OpenCV_INCLUDE_DIRS}
    )
    target_include_directories(test_luckfox_camera_to_display PRIVATE 
        include
        ${OpenCV_INCLUDE_DIRS}
    )

    target_link_libraries(test_luckfox_display ${OpenCV_LIBS})
    target_link_libraries(test_luckfox_camera_fps ${OpenCV_LIBS})
    target_link_libraries(test_luckfox_camera_to_display ${OpenCV_LIBS})

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
