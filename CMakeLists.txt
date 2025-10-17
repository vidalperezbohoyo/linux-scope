cmake_minimum_required(VERSION 3.10)
project(linux_scope)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add executable sources
file(GLOB SOURCES "src/*.cpp")
add_executable(scope ${SOURCES})

set(FBG_PATH "$ENV{HOME}/fbg")

# Include paths
target_include_directories(scope PRIVATE 
    include
    ${OpenCV_INCLUDE_DIRS}
    ${FBG_PATH}/src
    ${FBG_PATH}/custom_backend/fbdev
)

# Link libraries
target_link_libraries(scope 
    ${OpenCV_LIBS}
    ${FBG_PATH}/lib/lib.a
)

# Compiler options
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(scope PRIVATE -Wall -Wextra -O2)
endif()

# Build types
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
