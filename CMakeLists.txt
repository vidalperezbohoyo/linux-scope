cmake_minimum_required(VERSION 3.10)
project(linux_scope)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add executable sources
file(GLOB SOURCES "src/*.cpp")
add_executable(scope ${SOURCES})

# Include paths
target_include_directories(scope PRIVATE 
    include
    ${OpenCV_INCLUDE_DIRS}
    ~/fbg/src/fbgraphics.h
    ~/fbg/custom_backend/fbdev/fbg_fbdev.h
)

# Link libraries
target_link_libraries(scope 
    ${OpenCV_LIBS}
    ~/fbg/lib/lib.a
)

# Compiler options
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(opti PRIVATE -Wall -Wextra -O2)
endif()

# Build types
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
