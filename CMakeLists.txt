# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name (will be the executable name) and language
project(Raylib_App CXX)

# Set the C++ standard to 17 (recommended for raylib-cpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure the build directory for the executable (e.g., build/bin/Raylib_App)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# --- Source Files ---
# Collect all source files (checks 'src/' directory and the root for main.cpp)
file(GLOB_RECURSE PROJECT_SOURCES
    "src/*.cpp"
    "src/*.c"
    "main.cpp"
)

# --- Raylib Dependency Setup ---

# Attempt to find raylib using a configuration file (modern approach)
find_package(raylib CONFIG)

if(NOT TARGET raylib)
    message(STATUS "raylib not found via config, attempting manual search...")
    
    # Manually find the raylib header file and static/shared library
    find_path(RAYLIB_INCLUDE_DIR raylib.h)
    find_library(RAYLIB_LIB raylib)

    if(RAYLIB_INCLUDE_DIR AND RAYLIB_LIB)
        message(STATUS "Found raylib manually: ${RAYLIB_INCLUDE_DIR}")
        
        # Define a reusable Raylib interface target with required link flags
        add_library(raylib INTERFACE)
        target_include_directories(raylib INTERFACE ${RAYLIB_INCLUDE_DIR})
        
        # Add common system libraries required by raylib (OpenGL, math, threads, etc.)
        target_link_libraries(raylib INTERFACE 
            ${RAYLIB_LIB} 
            "-lGL" 
            "-lm" 
            "-lpthread" 
            "-ldl"
            )
        
        # On Linux, link X11 dependencies
        if(CMAKE_SYSTEM_NAME MATCHES "Linux")
            target_link_libraries(raylib INTERFACE X11)
        endif()

    else()
        message(FATAL_ERROR "raylib library or headers not found. Please install raylib (e.g., via a package manager like vcpkg or apt/brew) or manually set include/library paths.")
    endif()
endif()

# --- Create Executable and Link ---
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Link the executable to the raylib target
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
