cmake_minimum_required(VERSION 3.10)
project(linux_scope)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add executable sources

# Define FBG_PATH and check if it exists
set(FBG_PATH "$ENV{HOME}/fbg")

if(EXISTS "${FBG_PATH}")
    message(STATUS "Using FBG path: ${FBG_PATH}")

    add_executable(scope 
        src/main.cpp
        src/VideoProcessor.cpp
        src/VideoProvider.cpp
        src/Thread.cpp
        src/FbDisplay.cpp
    )

    # Include FBG headers
    target_include_directories(scope PRIVATE 
        ${FBG_PATH}/src
        ${FBG_PATH}/custom_backend/fbdev
    )

    # Link FBG static library
    target_link_libraries(scope 
        ${FBG_PATH}/lib/lib.a
    )

    # Define macro for FG display
    target_compile_definitions(scope PRIVATE USE_FG_DISPLAY)

else()

    add_executable(scope 
        src/main.cpp
        src/VideoProcessor.cpp
        src/VideoProvider.cpp
        src/Thread.cpp
        src/CvDisplay.cpp
    )

    message(WARNING "FBG path not found at ${FBG_PATH}, using OpenCV display instead.")

    # Define macro for OpenCV display
    target_compile_definitions(scope PRIVATE USE_CV_DISPLAY)
endif()

# Include standard headers
target_include_directories(scope PRIVATE 
    include
    ${OpenCV_INCLUDE_DIRS}
)

# Link OpenCV libraries
target_link_libraries(scope ${OpenCV_LIBS})

# Compiler options
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(scope PRIVATE -Wall -Wextra -O2)
endif()

# Build types
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
